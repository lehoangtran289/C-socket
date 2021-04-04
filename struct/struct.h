#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct Course {
    char num[10];
    char id[10];
    char name[30];
    int week_day;
    int is_morning;
    int p_start;
    int p_end;
    char weeks[30];
    char room[10];
} Course;

typedef struct Registration {
    char student_id[10];
    char course_num[10];
} Registration;

typedef struct Account {
    char student_id[10];
    char password[10];
} Account;

#endif