#include <iostream>
#include <chrono>
#include <fstream>

// For time
using namespace std;
using namespace std::chrono;

// For MariaDB
#include <mysql/mysql.h>
#include <curl/curl.h>

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

// For CPU and RAM usage
#include <sys/resource.h>
#include <sys/time.h>

ofstream result_file("results.txt");

// Funciones para medir el tiempo, el consumo de CPU y RAM
#define START_TIMER_AND_CPU_RAM { \
    struct rusage __rusage_start__; \
    getrusage(RUSAGE_SELF, &__rusage_start__); \
    auto __start__ = high_resolution_clock::now();

#define END_TIMER_AND_CPU_RAM(msg) \
    auto __end__ = high_resolution_clock::now(); \
    struct rusage __rusage_end__; \
    getrusage(RUSAGE_SELF, &__rusage_end__); \
    auto __duration__ = duration_cast<microseconds>(__end__ - __start__).count(); \
    long __cpu_user__ = (__rusage_end__.ru_utime.tv_sec - __rusage_start__.ru_utime.tv_sec) * 1000000 + (__rusage_end__.ru_utime.tv_usec - __rusage_start__.ru_utime.tv_usec); \
    double __elapsed_seconds__ = __duration__ / 1000000.0; \
    double __cpu_user_percentage__ = (__cpu_user__ / __elapsed_seconds__) / 10000.0; \
    long __max_rss_kb__ = __rusage_end__.ru_maxrss; \
    result_file << msg << " " << __duration__ << " ms, CPU (user): " << __cpu_user_percentage__ << " ,RAM: " << __max_rss_kb__ << " KB." << endl; \
}

void test_mongodb() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    auto db = conn["testdb"];
    auto collection = db["testcollection"];

    // Create
    START_TIMER_AND_CPU_RAM
    bsoncxx::builder::stream::document insert_document;
    insert_document << "name" << "test" << "value" << 1;
    collection.insert_one(insert_document.view());
    END_TIMER_AND_CPU_RAM("MongoDB Create")

    // Read
    START_TIMER_AND_CPU_RAM
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        result_file << bsoncxx::to_json(doc) << endl;
    }
    END_TIMER_AND_CPU_RAM("MongoDB Read")

    // Update
    bsoncxx::builder::stream::document update_filter_document;
    update_filter_document << "name" << "test";

    bsoncxx::builder::stream::document update_set_document;
    update_set_document << "$set" << bsoncxx::builder::stream::open_document << "value" << 2 << bsoncxx::builder::stream::close_document;

    START_TIMER_AND_CPU_RAM
    collection.update_one(update_filter_document.view(), update_set_document.view());
    END_TIMER_AND_CPU_RAM("MongoDB Update")

    // Delete
    START_TIMER_AND_CPU_RAM
    collection.delete_one(update_filter_document.view());
    END_TIMER_AND_CPU_RAM("MongoDB Delete")
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

    // Create
    START_TIMER_AND_CPU_RAM
    if (mysql_query(conn, "INSERT INTO testtable (name, value) VALUES('test', 1)")) {
        cerr << "INSERT failed: " << mysql_error(conn) << endl;
    }
    END_TIMER_AND_CPU_RAM("MariaDB Create")

    // Read
    START_TIMER_AND_CPU_RAM
    if (mysql_query(conn, "SELECT * FROM testtable")) {
        cerr << "SELECT failed: " << mysql_error(conn) << endl;
    } else {
        MYSQL_RES *res = mysql_store_result(conn);
        if (res == NULL) {
            cerr << "mysql_store_result() failed: " << mysql_error(conn) << endl;
        } else {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                result_file << row[0] << " " << row[1] << endl;
            }
            mysql_free_result(res);
        }
    }
    END_TIMER_AND_CPU_RAM("MariaDB Read")

    // Update
    START_TIMER_AND_CPU_RAM
    if (mysql_query(conn, "UPDATE testtable SET value=2 WHERE name='test'")) {
        cerr << "UPDATE failed: " << mysql_error(conn) << endl;
    }
    END_TIMER_AND_CPU_RAM("MariaDB Update")

    // Delete
    START_TIMER_AND_CPU_RAM
    if (mysql_query(conn, "DELETE FROM testtable WHERE name='test'")) {
        cerr << "DELETE failed: " << mysql_error(conn) << endl;
    }
    END_TIMER_AND_CPU_RAM("MariaDB Delete")

    mysql_close(conn);
}

// InfluxDB CRUD
void test_influxdb() {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Create
        START_TIMER_AND_CPU_RAM
        curl_easy_setopt(curl, CURLOPT_URL, "http://project:project@localhost:8086/write?db=testdb");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "testmeasurement,name=test value=1i");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "INSERT failed: " << curl_easy_strerror(res) << endl;
        }
        END_TIMER_AND_CPU_RAM("InfluxDB Create")

        // Read
        START_TIMER_AND_CPU_RAM
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8086/query?db=testdb&q=SELECT+*+FROM+testmeasurement");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "SELECT failed: " << curl_easy_strerror(res) << endl;
        }
        END_TIMER_AND_CPU_RAM("InfluxDB Read")

        // Update (Doesn't support it)
        // Delete
        START_TIMER_AND_CPU_RAM
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8086/query?db=testdb&q=DROP+SERIES+FROM+testmeasurement+WHERE+name='test'");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "DELETE failed: " << curl_easy_strerror(res) << endl;
        }
        END_TIMER_AND_CPU_RAM("InfluxDB Delete")

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main() {
    result_file << "\n**** Testing MongoDB *****" << endl;
    test_mongodb();
    result_file << "\n-----------------------------------------" << endl;

    result_file << "\n**** Testing MariaDB ****" << endl;
    test_mariadb();
    result_file << "\n-----------------------------------------" << endl;

    result_file << "\n**** Testing InfluxDB ****" << endl;
    test_influxdb();
    result_file << "\n-----------------------------------------" << endl;

    result_file.close();
    return 0;
}
