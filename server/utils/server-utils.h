#ifndef __SERVER_UTILS_H__
#define __SERVER_UTILS_H__

#include "../../struct/struct.h"

Course *findByCourseNum(char *course_num);
int checkLogin(Account acc);
char **findAllCourses(char *student_id, int *cnt);
char *searchCoursesByDay(char *student_id, char *day);
char *getAllCourses(char *student_id);
char *getBusyWeek(char *student_id);
void handleClientRequest(int connfd, char *buf);
void sendToCLient(int connfd, char *message);

#endif