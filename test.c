#include "LittleD/src/dbparser/dbparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTES_LEN 400

void create_table(db_query_mm_t* mm) {
    char table_definition[200];
    printf("Tabla creation query: ");
    fgets(table_definition, sizeof(table_definition), stdin);
    parse(table_definition, mm);
}

void insert_data(db_query_mm_t* mm) {
    char insert_values[200];
    printf("Data insert query: ");
    fgets(insert_values, sizeof(insert_values), stdin);
    parse(insert_values, mm);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
    char memseg[BYTES_LEN];
    db_query_mm_t mm;
    db_op_base_t* root;
    db_tuple_t tuple;

    init_query_mm(&mm, memseg, BYTES_LEN);

    int choice;
    char table_name[100]; // Variable para almacenar el nombre de la tabla
    do {
        printf("\nMENU:\n");
        printf("1. Create table\n");
        printf("2. Insert data\n");
        printf("3. Print data\n");
        printf("4. Exit\n");
        printf("Select an option: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                create_table(&mm);
                break;
            case 2:
                insert_data(&mm);
                break;
            case 3:
                printf("Enter table name: ");
                fgets(table_name, sizeof(table_name), stdin);
                char select_query[100];
                snprintf(select_query, sizeof(select_query), "SELECT * FROM %s;", table_name);
                init_query_mm(&mm, memseg, BYTES_LEN);
                root = parse(select_query, &mm);

                if (root == NULL) {
                    printf("NULL root\n");
                } else {
                    init_tuple(&tuple, root->header->tuple_size, root->header->num_attr, &mm);

                    while (next(root, &tuple, &mm) == 1) {
                        printf("Row: ");
                        for (int i = 0; i < root->header->num_attr; i++) {
                            char* attr_name = root->header->names[i];
                            printf("%s: %s, ", attr_name, getstringbyname(&tuple, attr_name, root->header));
                        }
                        printf("\n");
                    }
                }
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option\n");
        }
    } while (choice != 4);

    return 0;
}
