#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "struct.h"

extern Course courses[100];
extern int course_count;

extern Registration regs[100];
extern int reg_count;

extern Account accounts[100];
extern int acc_count;

// ---------------------------------------------

int readCourseScheduleFile() {
    char buff[100];
    FILE *fInput;
    char sInput[30] = "./data/course_schedule.txt";
    fInput = fopen(sInput, "r");

    if (fInput == NULL) {
        printf("Can not open file course_schedule.txt to read\n");
        return 0;
    }

    while ((fgets(buff, 100, fInput) != NULL)) {
        buff[strlen(buff) - 1] = '\0';
        Course course;

        // split string into tokens
        char tokens[100][100];
        int token_cnt = 0;
        char *pch;
        pch = strtok(buff, " ");
        while (pch != NULL) {
            strcpy(tokens[token_cnt++], pch);
            pch = strtok(NULL, " ");
        }

        // save course num, id, name
        strcpy(course.num, tokens[0]);
        strcpy(course.id, tokens[1]);
        strcpy(course.name, "");
        for (int i = 2; i < token_cnt - 1; i++) {
            strcat(course.name, tokens[i]);
            if (i != token_cnt - 2) {
                strcat(course.name, " ");
            }
        }

        // split course-time into tokens
        char *time = tokens[token_cnt - 1];
        time[strlen(time) - 1] = '\0';
        time[strlen(time) - 1] = '\0';
        char tokens2[100][100];
        token_cnt = 0;
        pch = strtok(time, ",");
        while (pch != NULL) {
            strcpy(tokens2[token_cnt++], pch);
            pch = strtok(NULL, ",");
        }

        // save course timetable
        course.week_day = tokens2[0][0] - '0';
        course.is_morning = tokens2[0][1] - '0' == 1 ? 1 : 0;
        course.p_start = tokens2[0][2] - '0';
        course.p_end = tokens2[1][2] - '0';
        strcpy(course.room, tokens2[token_cnt - 1]);

        strcpy(course.weeks, "");
        for (int i = 2; i < token_cnt - 1; i++) {
            strcat(course.weeks, tokens2[i]);
            if (i != token_cnt - 2) {
                strcat(course.weeks, ", ");
            }
        }

        // add to courses array
        courses[course_count++] = course;
    }

    fclose(fInput);
    return 1;
}

int readRegistrationFile() {
    FILE *fInput;
    char sInput[50] = "./data/student_registration.txt";
    fInput = fopen(sInput, "r");

    if (fInput == NULL) {
        printf("Can not open file student_registration.txt to read\n");
        return 0;
    }

    char student_id[20], course_num[20];
    while (fscanf(fInput, "%s %s\n", student_id, course_num) != EOF) {
        strcpy(regs[reg_count].student_id, student_id);
        strcpy(regs[reg_count].course_num, course_num);
        reg_count++;
    }
    // for (int i = 0; i < reg_count; i++)
    // printf("%s %s\n", regs[i].student_id, regs[i].course_num);
    fclose(fInput);
    return 1;
}

int readAccountFile() {
    FILE *fInput;
    char sInput[30] = "./data/account.txt";
    fInput = fopen(sInput, "r");

    if (fInput == NULL) {
        printf("Can not open file account.txt to read\n");
        return 0;
    }

    char account[20], password[20];
    while (fscanf(fInput, "%s %s\n", account, password) != EOF) {
        strcpy(accounts[acc_count].student_id, account);
        strcpy(accounts[acc_count].password, password);
        acc_count++;
    }
    // for (int i = 0; i < acc_count; i++)
    // printf("- %s %s\n", accounts[i].student_id, accounts[i].password);
    fclose(fInput);
    return 1;
}