/*
  
  General C Routines Header File

  Copyright (c) 1993, 1994 Boyd C. Fletcher
  
*/

#define GENERAL_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef VMS
#include <pwd.h>
#else
#include <unixlib.h>
#include <unixio.h>
#include <X11VMS/pwd.h>
#include <X11VMS/vmsutil.h>
#endif




#define FALSE 0
#define TRUE 1
#define OK 1
#define NOT_OK 0
#define NOT_FOUND 0
#define FOUND 1
#define YES 1
#define NO 0
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))
#define Unix_Filename_Chars_CONST "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_-+=|\\~`}]{[';.,<>/?"
/*
#define Unix_Filename_Chars_CONST "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_-+=|\\~`}]{[':;.,<>/?"  

Has Colon!!!!

*/
#define	SCMPN(a, b)	strncmp(a, b, sizeof (a))
#define	SCPYN(a, b)	(void)strncpy(a, b, sizeof (a))

void UpcaseWord(char *str);
void Remove_CR(char *str);
void Remove_CTRL(char *str);
int Is_All_Spaces(char *str);
void Add_Hrs_Mins(int *New_Hours,
    	          int *New_Minutes,
                  int Old_Hours,
    	          int Old_Minutes);
void Subtract_Hrs_Mins(int *New_Hours,
    	               int *New_Minutes,
                       int Old_Hours,
    	               int Old_Minutes);
char *Get_Current_Time();
char *Get_Current_Date();
char *Get_Current_Date_Time(char *Format);
time_t Get_Current_Date_Num();
char *Return_Current_Date(time_t Date_Enc);
char Check_User_Name(char *User_Name);
