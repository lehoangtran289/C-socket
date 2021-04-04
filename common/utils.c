#include "utils.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constant.h"

char *ltrim(char *s) {
    while (isspace(*s)) s++;
    return s;
}

char *rtrim(char *s) {
    char *back = s + strlen(s);
    while (isspace(*--back))
        ;
    *(back + 1) = '\0';
    return s;
}

char *trim(char *s) {
    return rtrim(ltrim(s));
}

void logArray(const char arr[][100], int cnt) {
    for (size_t i = 0; i < cnt; i++) {
        puts(&(*arr[i]));
    }
}

void to_upper(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

int isValidDay(char *day) {
    int l = strlen(day);
    if (l == 0) return 0;
    for (int i = 0; i < strlen(day); i++) {
        if (!isalpha(day[i])) return 0;
    }
    to_upper(day);
    return !(strcmp(day, "MONDAY") &&
             strcmp(day, "TUESDAY") &&
             strcmp(day, "WEDNESDAY") &&
             strcmp(day, "THURSDAY") &&
             strcmp(day, "FRIDAY") &&
             strcmp(day, "SATURDAY") &&
             strcmp(day, "SUNDAY"));
}

int weekday_to_int(char *day) {
    to_upper(day);
    char weekdays[][10] = {"MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY"};
    for (int i = 0; i < 7; i++) {
        if (strcmp(day, weekdays[i]) == 0) {
            return i + 2;
        }
    }
    return -1;
}

char *int_to_weekday(int day) {
    char weekdays[][10] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    return day >= 2 && day <= 8 ? strcpy(malloc(sizeof(char) * 20), weekdays[day]) : NULL;
}

// void printCourse(Course course) {
// printf("%-10s %-10s %-20s %-10d %-15d %-15d %-15d %-20s %-20s\n", course.num, course.id, course.name, course.week_day, course.is_morning, course.p_start, course.p_end, course.weeks, course.room);
// }