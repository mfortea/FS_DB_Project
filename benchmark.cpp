#include <iostream>
#include <chrono>

// For time
using namespace std;
using namespace std::chrono;

// For MariaDB
#include <mysql/mysql.h>
#include <curl/curl.h>

// For MongoDB
/**
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;

// For LittleD
#include "LittleD/src/dbparser/dbparser.h" 
#include "LittleD/src/dbops/db_ops.h"
*/

// Funciones para medir el tiempo
#define START_TIMER { auto __start__ = high_resolution_clock::now();
#define END_TIMER(msg) auto __end__ = high_resolution_clock::now(); \
                       auto __duration__ = duration_cast<microseconds>(__end__ - __start__).count(); \
                       cout << msg << " took " << __duration__ << " microseconds." << endl; }

/**
void test_mongodb() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    auto db = conn["testdb"];
    auto collection = db["testcollection"];

    // Insert
    START_TIMER
    document document{};
    document << "name" << "test" << "value" << 1;
    collection.insert_one(document.view());
    END_TIMER("\n-> \n-> MongoDB Insert")

    // Read
    START_TIMER
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        cout << bsoncxx::to_json(doc) << endl;
    }
    END_TIMER("\n-> MongoDB Read")

    // Update
    START_TIMER
    collection.update_one(document{} << "name" << "test" << finalize,
                          document{} << "$set" << open_document << "value" << 2 << close_document << finalize);
    END_TIMER("\n-> MongoDB Update")

    // Delete
    START_TIMER
    collection.delete_one(document{} << "name" << "test" << finalize);
    END_TIMER("\n-> MongoDB Delete")
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

// LittleD CRUD
/*
void test_littledb() {
    const int BYTES_LEN = 400;
    char memseg[BYTES_LEN];
    db_query_mm_t mm;
    db_op_base_t* root;
    db_tuple_t tuple;

    init_query_mm(&mm, memseg, BYTES_LEN);
    // Create Table
    START_TIMER
    parse(const_cast<char*>("CREATE TABLE testtable (name STRING, value INT);"), &mm);
    END_TIMER("\n-> LittleD Create Table")

    // Insert
    START_TIMER
    parse(const_cast<char*>("INSERT INTO testtable VALUES ('test', 1);"), &mm);
    END_TIMER("\n-> LittleD Insert")

    // Read
    START_TIMER
    init_query_mm(&mm, memseg, BYTES_LEN);
    root = parse(const_cast<char*>("SELECT * FROM testtable;"), &mm);

    if (root != NULL) {
        init_tuple(&tuple, root->header->tuple_size, root->header->num_attr, &mm);

        while (next(root, &tuple, &mm) == 1) {
            cout << "Row: ";
            for (int i = 0; i < root->header->num_attr; i++) {
                char* attr_name = root->header->names[i];
                cout << attr_name << ": " << getstringbyname(&tuple, attr_name, root->header) << ", ";
            }
            cout << endl;
        }
    } else {
        cerr << "Read failed: NULL root" << endl;
    }
    END_TIMER("\n-> LittleD Read")

    // Delete
    START_TIMER
    parse(const_cast<char*>("DELETE FROM testtable WHERE name = 'test';"), &mm);
    END_TIMER("\n-> LittleD Delete")
}
*/

int main() {
    cout << "\n**** Testing MongoDB *****" << endl;
    //test_mongodb();
    cout << "\n-----------------------------------------" << endl;

    cout << "\n**** Testing MariaDB ****" << endl;
    test_mariadb();
    cout << "\n-----------------------------------------" << endl;

    cout << "\n**** Testing InfluxDB ****" << endl;
    test_influxdb();
    cout << "\n-----------------------------------------" << endl;

    cout << "\n**** Testing LittleD ****" << endl;
    //test_littledb();
    cout << "\n-----------------------------------------" << endl;
    return 0;
}
