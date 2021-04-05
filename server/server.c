#include <arpa/inet.h>
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

extern socklen_t len;
extern fd_set masterfds;  // tập readfds để check các socket, 1 tập để lưu lại nhưng thay đổi của tập readfds.
extern fd_set readfds;    // file descriptor list for select()
extern int max_fd;
extern struct timeval timeout;

int main(int argc, char **argv) {
    char *buf = (char *)malloc(MAXLINE * sizeof(char));  // received message
    int n, connfd;
    pid_t childpid, pid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;

    // read file
    readAccountFile();
    readRegistrationFile();
    readCourseScheduleFile();

    // server initialization config
    initMultiplexingServer(argc, argv);

    printf("%s\n", "Server is running... waiting for connections...");
    while (1) {
        memcpy(&readfds, &masterfds, sizeof(masterfds));  // Copy masterfds into readfds -> wait for events

        int n_select;
        if ((n_select = select(max_fd + 1, &readfds, NULL, NULL, NULL)) < 0) {  // Hàm này sẽ block chương trình đến khi có 1 sự kiên ready to read xảy ra
            perror("Server-select() error...");
            exit(0);
        } else if (n_select == 0) {
            printf("Time out...\n");
        } else {
            for (int i = 0; i <= max_fd; i++) {  // run through the existing connections looking for data to be read
                if (FD_ISSET(i, &readfds)) {     // Check if serverSock has ready to read event -> new connection
                    int close_fd = 0;            // check should socket be close after handled
                    if (i == listenfd) {
                        int newCon = accept(listenfd, (struct sockaddr *)&cliaddr, &len);  // handle new connections
                        printf("\nNew connection \n");
                        printf("%s %s:%i\n", "Receiving request at", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                        FD_SET(newCon, &masterfds);    // add to master set
                        max_fd = max(newCon, max_fd);  // keep track of maximum fd
                    } else {                           // handle data from a client
                        printf("\nReceive data in socket %d\n", i);
                        int n = recv(i, buf, MAXLINE, 0);
                        if (n < 0) {
                            perror("Receive error...\n");
                            close_fd = 1;
                        } else if (n == 0) {
                            printf("socket %d hung up\n", i);
                            close_fd = 1;
                        } else {
                            strcpy(buf, trim(buf));
                            printf("%s %s\n", "String received from client: ", buf);

                            /* handle request here */
                            handleClientRequest(i, buf);
                        }
                    }

                    if (close_fd == 1) {
                        printf("Closing socket %d\n", i);
                        FD_CLR(i, &masterfds);
                        close(i);
                    }
                }
            }
        }
    }
}