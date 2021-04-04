#ifndef __CLIENT_UTILS_H__
#define __CLIENT_UTILS_H__

#include "../../struct/struct.h"
#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/

char *process(int signal, char *data);

#endif