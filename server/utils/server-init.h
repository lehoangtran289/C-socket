#ifndef __SERVER_INIT_H__
#define __SERVER_INIT_H__

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections */

void initMultiplexingServer(int argc, char **argv);

#endif