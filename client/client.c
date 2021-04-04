#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../struct/reader.h"
#include "../struct/struct.h"
#include "../common/constant.h"
#include "../common/utils.h"
#include "./utils/client-init.h"
#include "./utils/client-utils.h"

extern int sockfd;

int main(int argc, char **argv) {
    initClient(argc, argv);

    // login
    while (1) {
        int choice, isLogin = 0;
        printf("1. Login\n");
        printf("2. Exit\n");
        char *recvline = (char *)malloc(MAXLINE * sizeof(char));
        Account acc;
        do {
            printf("\nInput a choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    do {
                        printf("Enter student id: ");
                        fflush(stdin);
                        scanf("%s", acc.student_id);

                        printf("Enter password: ");
                        fflush(stdin);
                        scanf("%s", acc.password);

                        char data[MAXLINE];
                        strcat(data, acc.student_id);
                        strcat(data, " ");
                        strcat(data, acc.password);

                        strcpy(recvline, process(LOGIN, data));  // send to server
                        printf("receive from server: %s\n", recvline);

                        isLogin = strstr(recvline, SUCCESS) != NULL ? 1 : 0;
                    } while (isLogin == 0);
                    break;

                case 2:
                    printf("Exit\n");
                    exit(0);
                    break;

                default:
                    printf("Invalid task Try again!\n\n");
                    break;
            }
        } while (choice != 2 && isLogin != 1);

        // login success, handle request
        printf("\n\n----Login Success----\n\n\tMENU\n");
        printf("1. Search course registration by day\n");
        printf("2. Display all registrations\n");
        printf("3. Display busy schedule\n");
        printf("4. Exit\n");
        do {
            printf("\nInput a choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1: {
                    char *day = (char *)malloc(20 * sizeof(char));
                    int check;
                    do {
                        check = 0;
                        printf("Enter day in week: ");
                        fflush(stdin);
                        scanf("%s", day);
                        if (isValidDay(day) == 0) {
                            printf("Invalid!\n");
                            check = 1;
                        }
                    } while (check == 1);

                    char data[MAXLINE];
                    strcpy(data, acc.student_id);
                    strcat(data, " ");
                    strcat(data, day);
                    strcpy(recvline, process(SEARCHBYDAY, data));  // send to server & recv response
                    printf("receive from server: \n%s\n", recvline);
                } break;

                case 2: {
                    char data[MAXLINE];
                    strcpy(data, acc.student_id);
                    strcpy(recvline, process(SEARCHALL, data));  // send to server & recv response
                    printf("receive from server: \n%s\n", recvline);
                } break;

                case 3: {
                    char data[MAXLINE];
                    strcpy(data, acc.student_id);
                    strcpy(recvline, process(DISPLAYBUSY, data));  // send to server & recv response
                    printf("receive from server: \n%s\n", recvline);
                } break;

                case 4:
                    printf("Exit...\n");
                    break;

                default:
                    printf("Invalid task Try again!\n\n");
                    break;
            }
        } while (choice != 4);
        return 0;
    }
    exit(0);
}