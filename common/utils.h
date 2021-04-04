#ifndef __UTILS_H__
#define __UTILS_H__

#include "../struct/struct.h"

void logArray(const char arr[][100], int cnt);
void to_upper(char *str);
int isValidDay(char *day);
int weekday_to_int(char *day);
char *int_to_weekday(int day);
void printCourse(Course course);
char *trim(char *s);
char *rtrim(char *s);
char *ltrim(char *s);
int max(int x, int y);

#endif