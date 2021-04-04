#include "server-init.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int listenfd;
struct sockaddr_in servaddr;

void initServer(int argc, char **argv) {
    // creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) 
        listen(listenfd, LISTENQ);

    // while (1) {
    //     if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
    //         listen(listenfd, LISTENQ);
    //         break;
    //     } else {
    //         system("fuser -n tcp -k 3000");
    //         break;
    //     }
    // }
}