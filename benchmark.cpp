#include <iostream>
#include <chrono>
/**
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
*/
#include <mysql/mysql.h>
#include <curl/curl.h>

using namespace std;
using namespace std::chrono;

// Funciones para medir el tiempo
#define START_TIMER { auto __start__ = high_resolution_clock::now();
#define END_TIMER(msg) auto __end__ = high_resolution_clock::now(); \
                       auto __duration__ = duration_cast<microseconds>(__end__ - __start__).count(); \
                       cout << msg << " took " << __duration__ << " microseconds." << endl; }

// MongoDB CRUD
/*
void test_mongodb() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    auto db = conn["testdb"];
    auto collection = db["testcollection"];

    // Insert
    START_TIMER
    bsoncxx::builder::stream::document document{};
    document << "name" << "test" << "value" << 1;
    collection.insert_one(document.view());
    END_TIMER("MongoDB Insert")

    // Read
    START_TIMER
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        cout << bsoncxx::to_json(doc) << endl;
    }
    END_TIMER("MongoDB Read")

    // Update
    START_TIMER
    collection.update_one(bsoncxx::builder::stream::document{} << "name" << "test" << bsoncxx::builder::stream::finalize,
                          bsoncxx::builder::stream::document{} << "$set" << bsoncxx::builder::stream::open_document << "value" << 2 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);
    END_TIMER("MongoDB Update")

    // Delete
    START_TIMER
    collection.delete_one(bsoncxx::builder::stream::document{} << "name" << "test" << bsoncxx::builder::stream::finalize);
    END_TIMER("MongoDB Delete")
}
*/

// MariaDB CRUD
void test_mariadb() {
    MYSQL *conn;
    conn = mysql_init(NULL);
    if (conn == NULL) {
        cerr << "mysql_init() failed\n";
        return;
    }

    if (mysql_real_connect(conn, "localhost", "project", "project", "testdb", 0, NULL, 0) == NULL) {
        cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return;
    }

    // Insert
    START_TIMER
    if (mysql_query(conn, "INSERT INTO testtable (name, value) VALUES('test', 1)")) {
        cerr << "INSERT failed: " << mysql_error(conn) << endl;
    }
    END_TIMER("MariaDB Insert")

    // Read
    START_TIMER
    if (mysql_query(conn, "SELECT * FROM testtable")) {
        cerr << "SELECT failed: " << mysql_error(conn) << endl;
    } else {
        MYSQL_RES *res = mysql_store_result(conn);
        if (res == NULL) {
            cerr << "mysql_store_result() failed: " << mysql_error(conn) << endl;
        } else {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                printf("%s %s\n", row[0], row[1]);
            }
            mysql_free_result(res);
        }
    }
    END_TIMER("-> MariaDB Read")

    // Update
    START_TIMER
    if (mysql_query(conn, "UPDATE testtable SET value=2 WHERE name='test'")) {
        cerr << "UPDATE failed: " << mysql_error(conn) << endl;
    }
    END_TIMER("-> MariaDB Update")

    // Delete
    START_TIMER
    if (mysql_query(conn, "DELETE FROM testtable WHERE name='test'")) {
        cerr << "DELETE failed: " << mysql_error(conn) << endl;
    }
    END_TIMER("-> MariaDB Delete")

    mysql_close(conn);
}

// InfluxDB CRUD
void test_influxdb() {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Insert
        START_TIMER
        curl_easy_setopt(curl, CURLOPT_URL, "http://project:project@localhost:8086/write?db=testdb");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "testmeasurement,name=test value=1i");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "INSERT failed: " << curl_easy_strerror(res) << endl;
        }
        END_TIMER("\n-> InfluxDB Insert")

        // Read
        START_TIMER
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8086/query?db=testdb&q=SELECT+*+FROM+testmeasurement");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "SELECT failed: " << curl_easy_strerror(res) << endl;
        }
        END_TIMER("\n-> InfluxDB Read")

        // Update (Doesn't support it)
        // Delete
        START_TIMER
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8086/query?db=testdb&q=DROP+SERIES+FROM+testmeasurement+WHERE+name='test'");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "DELETE failed: " << curl_easy_strerror(res) << endl;
        }
        END_TIMER("\n-> InfluxDB Delete")

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main() {
    cout << "Testing MongoDB..." << endl;
    //test_mongodb();
    cout << "\nTesting MariaDB..." << endl;
    test_mariadb();
    cout << "\nTesting InfluxDB..." << endl;
    test_influxdb();
    return 0;
}
