#include "server-utils.h"
#include "../../common/utils.h"
#include "../../common/constant.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Course courses[100];
extern int course_count;

extern Registration regs[100];
extern int reg_count;

extern Account accounts[100];
extern int acc_count;

int checkLogin(Account acc) {
    // printf("%s %s\n", acc.student_id, acc.password);
    for (int i = 0; i < acc_count; i++) {
        if (strcmp(accounts[i].student_id, acc.student_id) == 0 && strcmp(accounts[i].password, acc.password) == 0) {
            return 1;
        }
    }
    return 0;
}

Course *findByCourseNum(char *course_num) {
    for (int i = 0; i < course_count; i++) {
        if (strcmp(course_num, courses[i].num) == 0) {
            return &courses[i];
        }
    }
    return NULL;
}

char **findAllCourses(char *student_id, int *cnt) {
    char **course_nums = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < reg_count; i++) {
        if (strcmp(regs[i].student_id, student_id) == 0) {
            course_nums[*cnt] = (char *)malloc(20 * sizeof(char));
            strcpy(course_nums[(*cnt)++], regs[i].course_num);
        }
    }
    return course_nums;
}

char *searchCoursesByDay(char *student_id, char *day) {
    int cnt = 0;
    char *result = (char *)malloc(10000 * sizeof(char));
    char **course_nums = findAllCourses(student_id, &cnt);

    sprintf(result, "%-10s %-10s %-20s %-10s %-15s %-15s %-15s %-20s %-20s\n", "NUMBER", "ID", "NAME", "DAY", "IS_MORNING", "P_START", "P_END", "WEEKS", "ROOM");
    for (int i = 0; i < cnt; i++) {
        Course *course = findByCourseNum(course_nums[i]);
        if (course != NULL && weekday_to_int(day) == course->week_day) {
            sprintf(result + strlen(result),"%-10s %-10s %-20s %-10d %-15d %-15d %-15d %-20s %-20s\n", course->num, course->id, course->name, course->week_day, course->is_morning, course->p_start, course->p_end, course->weeks, course->room);
        }
    } 

    return result;
}


char *getAllCourses(char *student_id) {
    int cnt = 0;
    char *result = (char *)malloc(10000 * sizeof(char));
    char **course_nums = findAllCourses(student_id, &cnt);

    sprintf(result, "%-10s %-10s %-20s %-10s %-15s %-15s %-15s %-20s %-20s\n", "NUMBER", "ID", "NAME", "DAY", "IS_MORNING", "P_START", "P_END", "WEEKS", "ROOM");
    for (int i = 0; i < cnt; i++) {
        Course *course = findByCourseNum(course_nums[i]);
        if (course != NULL) {
            sprintf(result + strlen(result),"%-10s %-10s %-20s %-10d %-15d %-15d %-15d %-20s %-20s\n", course->num, course->id, course->name, course->week_day, course->is_morning, course->p_start, course->p_end, course->weeks, course->room);
        }
    }
    return result;
}

char *getBusyWeek(char *student_id) {
    int cnt = 0;
    char *result = (char *)malloc(10000 * sizeof(char));
    char **course_nums = findAllCourses(student_id, &cnt);
    sprintf(result, "\t  %-9s %-8s %-9s %-9s %-8s", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday");
    for (int i = 0; i < 12; i++) {
        sprintf(result + strlen(result), "\n%-3d:    ", i + 1);
        for (int j = 0; j < 5; j++) {
            int flag = 0;
            for (int k = 0; k < cnt; k++) {
                Course *course = findByCourseNum(course_nums[k]);
                int OFFSET = course->is_morning ? 0 : 6;
                if (course->p_start + OFFSET <= i + 1 && course->p_end + OFFSET >= i + 1 && course->week_day == j + 2) {
                    sprintf(result + strlen(result), "| %-6s |", course->room);
                    flag = 1;
                } 
            }
            if (flag == 0) {
                sprintf(result + strlen(result), "%-8s", "|        |");
            }
        }
    }
    fflush(stdin);
    return result;
}