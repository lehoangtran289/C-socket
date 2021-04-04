#include "client-utils.h"
#include "../../common/utils.h"
#include "../../common/constant.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

extern int sockfd;

char *process(int signal, char *data) {
    char message[MAXLINE];
    char *recvline = (char *)malloc(1000 * sizeof(char));
    strcpy(message, "");
    sprintf(message, "%d", signal);
    strcat(message, " ");
    strcat(message, data);

    //log
    printf("data send to server: %s\n", message);

    send(sockfd, message, strlen(message), 0);
    if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
        // error: server terminated prematurely
        perror("The server terminated prematurely");
        exit(4);
    }
    // printf("-%s-\n", recvline);
    return recvline;
}