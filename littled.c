#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include "LittleD/src/dbparser/dbparser.h"

#define BYTES_LEN 400

// Macros for measuring time, CPU, and RAM
#define START_TIMER_AND_CPU_RAM                    \
    {                                              \
        struct rusage __rusage_start__;            \
        getrusage(RUSAGE_SELF, &__rusage_start__); \
        struct timespec __start__;                 \
        clock_gettime(CLOCK_MONOTONIC, &__start__);

#define END_TIMER_AND_CPU_RAM(msg)                                                                                                                                             \
    struct timespec __end__;                                                                                                                                                   \
    clock_gettime(CLOCK_MONOTONIC, &__end__);                                                                                                                                  \
    struct rusage __rusage_end__;                                                                                                                                              \
    getrusage(RUSAGE_SELF, &__rusage_end__);                                                                                                                                   \
    long __duration__ = (__end__.tv_sec - __start__.tv_sec) * 1000000 + (__end__.tv_nsec - __start__.tv_nsec) / 1000;                                                          \
    long __cpu_user__ = (__rusage_end__.ru_utime.tv_sec - __rusage_start__.ru_utime.tv_sec) * 1000000 + (__rusage_end__.ru_utime.tv_usec - __rusage_start__.ru_utime.tv_usec); \
    double __elapsed_seconds__ = __duration__ / 1000000.0;                                                                                                                     \
    double __cpu_user_percentage__ = (__cpu_user__ / __elapsed_seconds__) / 100000.0;                                                                                           \
    long __max_rss_kb__ = __rusage_end__.ru_maxrss;                                                                                                                            \
    printf("%s %ld us, CPU (user): %.2f%%, RAM: %ld KB\n", msg, __duration__, __cpu_user_percentage__, __max_rss_kb__);                                                        \
    }

int main(void)
{
    char memseg[BYTES_LEN];
    db_query_mm_t mm;
    db_op_base_t *root;
    db_tuple_t tuple;

    // Create table
    START_TIMER_AND_CPU_RAM
    init_query_mm(&mm, memseg, BYTES_LEN);
    parse("CREATE TABLE sensors (id int, temp int);", &mm);
    END_TIMER_AND_CPU_RAM("-> Little D Create")

    // Insert into table
    START_TIMER_AND_CPU_RAM
    init_query_mm(&mm, memseg, BYTES_LEN);
    parse("INSERT INTO sensors VALUES (1, 221);", &mm);
    END_TIMER_AND_CPU_RAM("-> Little D Insert")

    // Select from table
    START_TIMER_AND_CPU_RAM
    init_query_mm(&mm, memseg, BYTES_LEN);
    root = parse("SELECT * FROM sensors;", &mm);
    if (root == NULL)
    {
        printf("NULL root\n");
    }
    else
    {
        init_tuple(&tuple, root->header->tuple_size, root->header->num_attr, &mm);
        while (next(root, &tuple, &mm) == 1)
        {
            int id = getintbyname(&tuple, "id", root->header);
            int sensor_val = getintbyname(&tuple, "temp", root->header);
        }
    }
    END_TIMER_AND_CPU_RAM("-> Little D Read")

    // Drop the table if it exists
    START_TIMER_AND_CPU_RAM
    init_query_mm(&mm, memseg, BYTES_LEN);
    parse("DROP TABLE IF EXISTS sensors;", &mm);
    END_TIMER_AND_CPU_RAM("-> Little D Drop Table")

    // Remove sensors file after all the tests
    const char *filename = "sensors";
    if (remove(filename) == 0)
    {
        printf("\nDatabase file deleted\n", filename);
    }
    else
    {
        perror("Error");
    }

    return 0;
}
