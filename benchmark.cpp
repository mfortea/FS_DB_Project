#include <iostream>
#include <chrono>

// For time
using namespace std;
using namespace std::chrono;

// For MariaDB
#include <mysql/mysql.h>
#include <curl/curl.h>

// Include unistd.h before LittleD to avoid name conflicts
#include <unistd.h>

// For MongoDB
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/document/view.hpp>


// Funciones para medir el tiempo
#define START_TIMER { auto __start__ = high_resolution_clock::now();
#define END_TIMER(msg) auto __end__ = high_resolution_clock::now(); \
                       auto __duration__ = duration_cast<microseconds>(__end__ - __start__).count(); \
                       cout << msg << " took " << __duration__ << " microseconds." << endl; }


void test_mongodb() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    auto db = conn["testdb"];
    auto collection = db["testcollection"];

    // Insert
    START_TIMER
    bsoncxx::builder::stream::document insert_document;
    insert_document << "name" << "test" << "value" << 1;
    collection.insert_one(insert_document.view());
    END_TIMER("\n-> \n-> MongoDB Insert")

    // Read
    START_TIMER
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        cout << bsoncxx::to_json(doc) << endl;
    }
    END_TIMER("\n-> MongoDB Read")

    // Update
    bsoncxx::builder::stream::document update_filter_document;
    update_filter_document << "name" << "test";

    bsoncxx::builder::stream::document update_set_document;
    update_set_document << "$set" << bsoncxx::builder::stream::open_document << "value" << 2 << bsoncxx::builder::stream::close_document;

    START_TIMER
    collection.update_one(update_filter_document.view(), update_set_document.view());
    END_TIMER("\n-> MongoDB Update")

    // Delete
    START_TIMER
    collection.delete_one(update_filter_document.view());
    END_TIMER("\n-> MongoDB Delete")
}

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
    END_TIMER("\n-> MariaDB Insert")

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
    END_TIMER("\n-> MariaDB Read")

    // Update
    START_TIMER
    if (mysql_query(conn, "UPDATE testtable SET value=2 WHERE name='test'")) {
        cerr << "UPDATE failed: " << mysql_error(conn) << endl;
    }
    END_TIMER("\n-> MariaDB Update")

    // Delete
    START_TIMER
    if (mysql_query(conn, "DELETE FROM testtable WHERE name='test'")) {
        cerr << "DELETE failed: " << mysql_error(conn) << endl;
    }
    END_TIMER("\n-> MariaDB Delete")

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
    cout << "\n**** Testing MongoDB *****" << endl;
    test_mongodb();
    cout << "\n-----------------------------------------" << endl;

    cout << "\n**** Testing MariaDB ****" << endl;
    test_mariadb();
    cout << "\n-----------------------------------------" << endl;

    cout << "\n**** Testing InfluxDB ****" << endl;
    test_influxdb();
    cout << "\n-----------------------------------------" << endl;
    return 0;
}
