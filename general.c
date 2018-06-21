/*
  
  General C Routines Source Code

  Copyright (c) 1993, 1994 Boyd C. Fletcher
  
*/

#include "general.h"

void Remove_CR(char *str)
{
 int i;

 for (i=0;i <= strlen(str); i++) 
   {
    if (str[i] == '\n') 
      {
	str[i] = '\0';
      }
   }
}

void UpcaseWord(char *str)
{
 int i;

 for (i=0;i <= strlen(str); i++) 
   {
     str[i] = toupper(str[i]);
   }
}

void Remove_CTRL(char *str)
{
 int i;

 for (i=0;i <= strlen(str); i++) 
   {
     if (str[i] != '\0')
       {
	 if ((str[i] < 32) || (str[i] >= 127))
	   {
	     str[i] = '\0';
	   }
       }
   }
}

int Is_All_Spaces(char *str)
{
 int i;
 int All_Spaces = TRUE;

 for (i=0;i < strlen(str); i++)
   {
    if (str[i] != ' ')
       {
	 All_Spaces = FALSE;
       }
   }
 return (All_Spaces);
}

void Add_Hrs_Mins(int *New_Hours,
    	          int *New_Minutes,
                  int Old_Hours,
    	          int Old_Minutes)
{
 int Hrs = 0,
     Mins = 0;

/*  printf("OLD HOURS: %d MINUTES: %d\n",Old_Hours,Old_Minutes); */

 Mins = (Old_Hours*60) + Old_Minutes;
 Hrs = Mins / 60;
 Mins = Mins % 60;
 *New_Hours = Hrs;
 *New_Minutes = Mins;

/* printf("HOURS: %d MINUTES: %d\n",Hrs,Mins); */
/* printf("NEW HOURS: %d MINUTES: %d\n",*New_Hours,*New_Minutes); */
}

void Subtract_Hrs_Mins(int *New_Hours,
    	               int *New_Minutes,
                       int Old_Hours,
        	       int Old_Minutes)
{
 int Hrs = 0,
     Mins = 0;

 printf("OLD HOURS: %d MINUTES: %d\n",Old_Hours,Old_Minutes);
 Mins = (Old_Hours*60) + Old_Minutes;
 Hrs = Mins / 60;
 Mins = Mins % 60;
 printf("HOURS: %d MINUTES: %d\n",Hrs,Mins);
 *New_Hours = Hrs;
 *New_Minutes = Mins;
 printf("NEW HOURS: %d MINUTES: %d\n",*New_Hours,*New_Minutes);
}


char *Get_Current_Date()
{
  static char  Date_Str[80];
  time_t Date_Enc;
  struct tm *Date_Struct;

  time(&Date_Enc);
  Date_Struct = localtime(&Date_Enc);
  strftime(Date_Str,80,"%x",Date_Struct);
  return Date_Str;
}

#ifdef SUNOS
time_t Get_Current_Date_Num()
{
  time_t Date_Enc;
  struct tm *Date_Struct;

  time(&Date_Enc);
  Date_Struct = localtime(&Date_Enc);
  return timelocal(Date_Struct);
}
#endif

char *Return_Current_Date(time_t Date_Enc)
{
  static char  Date_Str[80];
  struct tm *Date_Struct;

  Date_Struct = localtime(&Date_Enc);
  strftime(Date_Str,80,"%x",Date_Struct);
  return Date_Str;
}

char *Return_Current_Date_Time(time_t Date_Enc)
{
  static char  Date_Str[80];
  struct tm *Date_Struct;

  Date_Struct = localtime(&Date_Enc);
  strftime(Date_Str,80,"%x %X: ",Date_Struct);
  return Date_Str;
}


char *Get_Current_Time()
{
  static char  Time_Str[80];
  time_t Time_Enc;
  struct tm *Time_Struct;


  time(&Time_Enc);
  Time_Struct = localtime(&Time_Enc);
  strftime(Time_Str,80,"%X",Time_Struct);
  return Time_Str;
}

char *Get_Current_Date_Time(char *Format)
{
  static char  Time_Str[80];
  time_t Time_Enc;
  struct tm *Time_Struct;


  time(&Time_Enc);
  Time_Struct = localtime(&Time_Enc);
  strftime(Time_Str,80,Format,Time_Struct);
  return Time_Str;
}

void Strip_White(char *string)
{
  register int i = 0;
 
  while (whitespace (string[i]))
    i++;
 
  if (i)
    strcpy (string, string + i);
 
  i = strlen (string) - 1;
 
  while (i > 0 && whitespace (string[i]))
    i--;
 
  string[++i] = '\0';
}
