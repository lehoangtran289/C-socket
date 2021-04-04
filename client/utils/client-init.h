#ifndef __CLIENT_INIT_H__
#define __CLIENT_INIT_H__

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/

int initClient(int argc, char **argv);
char *process(int signal, char *data);

#endif