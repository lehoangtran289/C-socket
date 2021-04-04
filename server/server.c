#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../common/constant.h"
#include "../common/utils.h"
#include "../struct/reader.h"
#include "../struct/struct.h"
#include "./utils/server-init.h"
#include "./utils/server-utils.h"

extern int listenfd;
extern struct sockaddr_in servaddr;

/*
WNOHANG: waitpid does not block
while loop: waitpid repeatedly until there is no child
process change status, i.e until waitpid returns 0.
*/
void sig_chld(int signo) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

char *choppy(char *s) {
    char *n = malloc(strlen(s ? s : "\n"));
    if (s)
        strcpy(n, s);
    n[strlen(n) - 1] = '\0';
    return n;
}

int main(int argc, char **argv) {
    // char buf[MAXLINE];
    char *buf = (char *)malloc(MAXLINE * sizeof(char));
    int n, connfd;
    pid_t childpid, pid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;

    // read file
    readAccountFile();
    readRegistrationFile();
    readCourseScheduleFile();

    // server config
    initServer(argc, argv);

    printf("%s\n", "Server running...waiting for connections.");
    while (1) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        char ip[100];
        inet_ntop(AF_INET, &(cliaddr.sin_addr), ip, 100);
        printf("%s %s:%d\n", "Received request at ", ip, cliaddr.sin_port);

        if ((pid = fork() == 0)) {
            close(listenfd); /* child closes listening socket */

            /* process the request */
            while ((n = recv(connfd, buf, MAXLINE, 0)) > 0) {
                // receive signal from client
                strcpy(buf, trim(buf));
                printf("%s %s\n", "String received from client: ", buf);

                char tokens[100][100];
                int token_cnt = 0;
                char *pch;
                pch = strtok(buf, " ");
                while (pch != NULL) {
                    strcpy(tokens[token_cnt++], pch);
                    pch = strtok(NULL, " ");
                }

                logArray(&tokens, token_cnt);

                int signal = atoi(tokens[0]);
                switch (signal) {
                    case LOGIN: {
                        Account acc;
                        if (token_cnt != 3) {
                            send(connfd, "server error, login failed", 28, 0);
                            break;
                        }
                        strcpy(acc.student_id, tokens[1]);
                        strcpy(acc.password, tokens[2]);
                        if (checkLogin(acc) == 0) {
                            send(connfd, "login failed", 14, 0);
                            break;
                        }
                        send(connfd, "login success", 15, 0);
                    } break;

                    case SEARCHBYDAY: {
                        if (token_cnt != 3) {
                            send(connfd, "server error", 14, 0);
                            break;
                        }
                        char *day = (char *)malloc(100 * sizeof(char));
                        char student_id[MAXLINE];
                        strcpy(student_id, tokens[1]);
                        strcpy(day, tokens[2]);

                        char *result = (char *)malloc(MAXLINE * sizeof(char));
                        strcpy(result, searchCoursesByDay(student_id, day));

                        send(connfd, result, strlen(result), 0);
                    } break;

                    case SEARCHALL: {
                        if (token_cnt != 2) {
                            send(connfd, "server error", 14, 0);
                            break;
                        }
                        char student_id[MAXLINE];
                        strcpy(student_id, tokens[1]);

                        char *result = (char *)malloc(MAXLINE * sizeof(char));
                        strcpy(result, getAllCourses(student_id));

                        send(connfd, result, strlen(result), 0);
                    } break;

                    case DISPLAYBUSY: {
                        if (token_cnt != 2) {
                            send(connfd, "server error", 14, 0);
                            break;
                        }
                        char student_id[MAXLINE];
                        strcpy(student_id, tokens[1]);

                        char *result = (char *)malloc(MAXLINE * sizeof(char));
                        strcpy(result, getBusyWeek(student_id));

                        send(connfd, result, strlen(result), 0);

                    } break;

                    default:
                        break;
                }
                fflush(stdout);
                bzero(buf, 2000);
            }
            if (n < 0) {
                perror("Read error");
                exit(1);
            }
            close(connfd);
            exit(0);
        }
        signal(SIGCHLD, sig_chld);
    }
    // close listening socket
    close(listenfd);
}