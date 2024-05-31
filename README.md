# FS & BD Project 
File Systems and DB on Linux Project for the course Operating Systems of the HMU

# 👨🏻‍💻 Used software 
- LittleD Database ([Github Repository](https://github.com/graemedouglas/LittleD))
- MongoDB & MongoDB CXX Driver ([Tutorial](https://www.mongodb.com/docs/languages/cpp/cpp-driver/current/installation/linux/#std-label-cpp-installation-linux))
- MariaDB & libmysqlclient-dev (C++ client)
- InfluxDB & libcurl4-openssl-dev (for C++ client)

# 🧪 Makefile
You need to run the makefile of LittleD before, the library has to be in the same folder as the rest of the code.
For compile the test.c (LittleD test code)
```
make test
```

For compile the benchmark.c (all the databases benchmark)
```
make benchmark
```
