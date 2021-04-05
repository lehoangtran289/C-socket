#include "server-init.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int listenfd;
struct sockaddr_in servaddr;

socklen_t len;
fd_set masterfds;  // tập readfds để check các socket, 1 tập để lưu lại nhưng thay đổi của tập readfds.
fd_set readfds;    // file descriptor list for select()
int max_fd;
struct timeval timeout;

void initMultiplexingServer(int argc, char **argv) {
    // creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
        listen(listenfd, LISTENQ);
    } else {
        perror("Bind error!");
        exit(0);
    }

    // socket-select config
    len = sizeof(struct sockaddr_in);
    FD_ZERO(&masterfds);           // clear master set
    FD_ZERO(&readfds);             // clear temp set
    FD_SET(listenfd, &masterfds);  // add the listenerfd to the master set
    max_fd = listenfd;         // keep track of the biggest file descriptor

    timeout.tv_sec = 90;  // Server listens in 90s, if timeout = NULL then select() runs forever
    timeout.tv_usec = 0;
}