# FS & BD Project 
File Systems and DB on Linux Project for the course Operating Systems of the HMU

# 👨🏻‍💻 Used software 
- LittleD Database ([Github Repository](https://github.com/graemedouglas/LittleD))
- MongoDB 4.4.6 & MongoDB CXX Driver ([Tutorial](https://www.mongodb.com/docs/languages/cpp/cpp-driver/current/installation/linux/#std-label-cpp-installation-linux))
- MariaDB & libmysqlclient-dev (C++ client)
- InfluxDB & libcurl4-openssl-dev (for C++ client)

# 🧪 Makefile
You need to run the makefile of LittleD before, the library has to be in the same folder as the rest of the code.

For compile the benchmark for LittleD, you have to run:
```
make littled
```

For compile the benchmark for the rest of the databases, you have to run:
```
make benchmark
```

# 🚀 Run the code
For run the benchmark for LittleD, you have to run:
```bash
./littled
```

And for the benchmark for MongoDB, MariaDB & InfluxDB:
```bash
./benchmark
```
This will generate a file called results.txt with the results of the test for the three databases


# 📊 Databases benchmark data
## For MongoDB
```bash
$ mongo
> use testdb
> db.createCollection("testcollection")
```

## For MariaDB
```sql
CREATE DATABASE testdb;
USE testdb;
CREATE TABLE testtable (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255),
    value INT
);
```

## For InfluxDB
```bash
$ influx
Connected to http://localhost:8086 version 1.x.x
InfluxDB shell version: 1.x.x
> CREATE DATABASE testdb
> SHOW DATABASES
```
