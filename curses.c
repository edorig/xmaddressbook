#include "main.h"

#if defined(CURSES)

#include <curses.h>


/******************************************************************
   CursesClearWindow
*******************************************************************/

void CursesClearWindow(WINDOW *Window)
{
  mvwaddch(Window,0,0,' ');
  wclrtobot(Window);
  wrefresh(Window);
}

/******************************************************************
   CursesCenter
*******************************************************************/

void CursesCenter(WINDOW *Window,
		  int     Bold,    /* Bold==1 mean turn on bold */
		  int     Y,
		  char    *String)
{
  if (Bold == 1) wstandout(Window);
  mvwaddstr(Window,Y,abs((COLS-strlen(String))/2),String);
  if (Bold == 1) wstandend(Window);
}


/******************************************************************
  CursesDisplayHelp
*******************************************************************/

void CursesDisplayHelp(WINDOW *Window)
{
  CursesClearWindow(Window);
  CursesCenter(Window,1,1,"Help");
  CursesCenter(Window,0,3,"n, f or SPACE = Move forward in address book.");
  CursesCenter(Window,0,4,"p, b = Move backward in address book.");
  CursesCenter(Window,0,5,"g or : = Goto a particular entry.");
  CursesCenter(Window,0,6,"s or // = Search for a particular entry.");
  CursesCenter(Window,0,6,"r or . = Repeat previous search.");
  CursesCenter(Window,0,7,"h or ? = Display this help.");
  CursesCenter(Window,0,8,"q = Quit address book.");
  CursesCenter(Window,0,11,"Press ANY KEY to exit Help.");
  wrefresh(Window);

#if  defined(SUNOS) || defined(IRIX) || defined(SOLARIS)
  notimeout(Window,TRUE);
#endif

   keypad(Window,TRUE);
  noecho();
  wgetch(Window);
  CursesClearWindow(Window);
}


/******************************************************************
  CursesDisplayEntry
*******************************************************************/

void CursesDisplayEntry(WINDOW *DisplayWindow,
			Rolo_Type *Temp)

{
  noecho();
  CursesClearWindow(DisplayWindow);
  mvwprintw(DisplayWindow,1,0,"Name:        %s\n",Temp->Name);
  wprintw(DisplayWindow,"WorkPhone:   %s\n",Temp->WorkPhone);
  wprintw(DisplayWindow,"HomePhone:   %s\n",Temp->HomePhone);
  wprintw(DisplayWindow,"Company:     %s\n",Temp->Company);
  wprintw(DisplayWindow,"Fax Number:  %s\n",Temp->FaxNumber);
  wprintw(DisplayWindow,"Email:       %s\n",Temp->EmailAddress);
  wprintw(DisplayWindow,"Work Addr.:  %s\n",Temp->WorkAddress);
  wprintw(DisplayWindow,"Home Addr.:  %s\n",Temp->HomeAddress);
  wprintw(DisplayWindow,"DateUpdated: %s\n",Temp->DateUpdated);
  wprintw(DisplayWindow,"Remarks:     %s\n",Temp->Remarks);
  wrefresh(DisplayWindow);
}


/******************************************************************
  CursesUpdateCounterWindow
*******************************************************************/

void CursesUpdateCounterWindow(WINDOW *CounterWindow,
			       int     Counter)

{
  noecho();
  CursesClearWindow(CounterWindow);
  mvwprintw(CounterWindow,0,0,"%d/%d",Counter,G_NumInRoloList);
  wrefresh(CounterWindow);
}

/******************************************************************
  CursesSearch
*******************************************************************/

void CursesSearch(WINDOW *DisplayWindow,
		  WINDOW *CommandWindow,
		  WINDOW *CounterWindow,
		  int     RepeatSearch, /* RepeatSearch == 0, to start search over */
		  int    *ReturnCounter)
     
{
  char *TempStr;
  Rolo_Type *Temp;
  int Counter=0;
  int SearchItem;
  static int CursesSearchCounter;
  static char SearchStr[BUFSIZ];


  if (RepeatSearch == 0)
    {
      CursesClearWindow(CommandWindow);
      echo();
      mvwprintw(CommandWindow,0,0,"Search Keyword: ");
      wrefresh(CommandWindow);
      mvwgetstr(CommandWindow,0,16,SearchStr);
      CursesSearchCounter = 1;
    }

  mvwprintw(CommandWindow,0,0,"Search Keyword: %s",SearchStr);
  Temp = G_RoloList;
  Counter = 0;
  while (Temp != NULL)
    {
      UpcaseWord(SearchStr);
      Counter++;
      if (CursesSearchCounter > G_NumInRoloList)
	{
	  CursesSearchCounter = 1;
	  Counter = 1;
	}
/*       fprintf(stderr,"%d %d %s %s\n",Counter,CursesSearchCounter,TempStr,SearchStr); */
      for (SearchItem = 0; SearchItem <  9 ; SearchItem++)
	{
	  switch (SearchItem)
	    {
	    case 0 :
	      {
		TempStr = strdup(Temp->Name);
		break;
	      }
	    case 1 :
	      {
		TempStr = strdup(Temp->WorkPhone);
		break;
	      }
	    case 2 :
	      {
		TempStr = strdup(Temp->HomePhone);
		break;
	      }
	    case 3 :
	      {
		TempStr = strdup(Temp->Company);
		break;
	      }	
	    case 4 :
	      {
		TempStr = strdup(Temp->WorkAddress);
		break;
	      }
	    case 5 :
	      {
		TempStr = strdup(Temp->HomeAddress);
		break;
	      }
	    case 6 :
	      {
		TempStr = strdup(Temp->DateUpdated);
		break;
	      }
	    case 7 :
	      {
		TempStr = strdup(Temp->Remarks);
		break;
	      }
	    case 8 :
	      {
		TempStr = strdup(Temp->EmailAddress);
		break;
	      }
	    default :
	      {
		TempStr = strdup(Temp->Name);
		break;
	      }
	    }
	  UpcaseWord(TempStr);
	  if ((strstr(TempStr,SearchStr) != NULL) && (Counter > CursesSearchCounter))
	    {
	      G_CurrentNumInRoloList = Counter;
	      G_CurrentEntryInRoloList = Temp;
	      CursesSearchCounter = Counter;
	      CursesDisplayEntry(DisplayWindow,Temp);
	      CursesUpdateCounterWindow(CounterWindow,Counter);
	      free(TempStr);
	      *ReturnCounter = Counter;
	      noecho();
	      return;
	    }
	  free(TempStr);
	}
      
      Temp = Temp->Next;
      if (Counter >= G_NumInRoloList)
	{
	  CursesSearchCounter = 0;
	  Counter = 1;
	}
    }
  noecho();
}

/******************************************************************
  CursesGoto
*******************************************************************/

void CursesGoto(WINDOW *DisplayWindow,
		WINDOW *CommandWindow,
		WINDOW *CounterWindow,
                int    *Counter)

{
  char GotoStr[BUFSIZ];
  int GotoNum;
  Rolo_Type *Temp;

  CursesClearWindow(CommandWindow);
  echo();
  mvwprintw(CommandWindow,0,0,"Goto Entry: ");
  wrefresh(CommandWindow);
  mvwgetstr(CommandWindow,0,12,GotoStr);
  
  GotoNum = atoi(GotoStr);
  Temp = G_RoloList;
  if (GotoNum <= 0)
    {
      (*Counter) = 1;
    }
  else {
    Temp = G_RoloList;
    (*Counter) = 1;
    while ((Temp->Next != NULL) && ((*Counter) < GotoNum))
      {
	Temp = Temp->Next;
	(*Counter)++;
      }
  }
  CursesDisplayEntry(DisplayWindow,Temp);
  CursesUpdateCounterWindow(CounterWindow,*Counter);
  noecho();
}


/******************************************************************
  CursesMain
*******************************************************************/

void  CursesMain(WINDOW *DisplayWindow,
		 WINDOW *CommandWindow,
		 WINDOW *CounterWindow)
{
  char Key_Press;
  Rolo_Type *Temp;
  int Counter = 1,lcv;


  CursesClearWindow(DisplayWindow);
  Temp = G_RoloList;
  CursesDisplayEntry(DisplayWindow,Temp);
  while (tolower(Key_Press) != 'q')
    {
     echo();
      CursesClearWindow(CommandWindow);
      if ((tolower(Key_Press) == 'f') || (tolower(Key_Press) == 'n') || (tolower(Key_Press) == ' '))
	{
	  Counter++;
	  Temp = Temp->Next;
	  if (Temp == NULL)
	    {
	      Temp = G_RoloList;
	      Counter = 1;
	    }
	  CursesDisplayEntry(DisplayWindow,Temp);
	}
      if ((tolower(Key_Press) == 'b') || (tolower(Key_Press) == 'p')) /*sloppy, should have created prev field in Rolo_Type */
	{
	  Counter--;
	  if (Counter < 1) /* wrap around begining of list */
	    {
	      Temp = G_RoloList;
	      while (Temp->Next != NULL)
		{ 
		  Temp = Temp->Next;
		}
	      Counter = G_NumInRoloList;
	    }
	  else {
	    lcv = 1;
	    Temp = G_RoloList;
	    while ((Temp != NULL) && (lcv < Counter))
	      {
		Temp = Temp->Next;
		lcv++;
	      }
	  }
	  CursesDisplayEntry(DisplayWindow,Temp);
	}
      if ((tolower(Key_Press) == 's') || (Key_Press == '/'))
	{
	  CursesSearch(DisplayWindow,CommandWindow,CounterWindow,0,&Counter);
	}
      if ((tolower(Key_Press) == 'r') || (Key_Press == '.'))
	{
	  CursesSearch(DisplayWindow,CommandWindow,CounterWindow,1,&Counter);
	}
      if ((tolower(Key_Press) == 'g') || (Key_Press == ':'))
	{
	  CursesGoto(DisplayWindow,CommandWindow,CounterWindow,&Counter);
	}
      if ((Key_Press == '?') || (tolower(Key_Press) == 'h'))
	{
	  CursesDisplayHelp(DisplayWindow);
	  CursesDisplayEntry(DisplayWindow,Temp);
	}
      if (tolower(Key_Press) == 'q')
	{
	  endwin();
	}
      CursesUpdateCounterWindow(CounterWindow,Counter);
      Key_Press = wgetch(CommandWindow);
    }
}


/******************************************************************
  CursesInterface
*******************************************************************/

void CursesInterface()
{
  WINDOW *MainWindow, *CommandWindow, *CommandBoxWindow;
  WINDOW *DisplayWindow, *TitleWindow, *CounterWindow, *CounterBoxWindow;
  


  if (G_RoloList == NULL)
    {
      fprintf(stderr,"Address Book is Empty.\n");
      exit(1);
    }

  stdscr = initscr();

  if (LINES < 20)
    {
      endwin();
      printf("Program requires 20 rows.\n");
      exit (1);
    }
  if (COLS < 80)
    {
      endwin();
      printf("Program requires 80 columns.\n");
      exit (1);
    }

  MainWindow = newwin(LINES,COLS,0,0);
  TitleWindow = subwin(MainWindow,2,78,0,0);
  DisplayWindow = subwin(MainWindow,LINES-6,COLS-2,2,1);

  CommandBoxWindow = subwin(MainWindow,3,COLS-18,LINES-4,1);
  CommandWindow = subwin(MainWindow,1,COLS-20,LINES-3,2);
  CounterBoxWindow = subwin(MainWindow,3,16,LINES-4,COLS-17);
  CounterWindow = subwin(MainWindow,1,14,LINES-3,COLS-16);

/*
  CommandBoxWindow = subwin(MainWindow,3,COLS-18,LINES-4,17);
  CommandWindow = subwin(MainWindow,1,COLS-19,LINES-3,18);
  CounterBoxWindow = subwin(MainWindow,3,16,LINES-4,1);
  CounterWindow = subwin(MainWindow,1,14,LINES-3,2);
*/
  wnoutrefresh(MainWindow);
  wnoutrefresh(TitleWindow);
  wnoutrefresh(DisplayWindow);
  wnoutrefresh(CommandBoxWindow);
  wnoutrefresh(CommandWindow);
  wnoutrefresh(CounterBoxWindow);
  wnoutrefresh(CounterWindow);

  flushinp();
  intrflush(DisplayWindow,FALSE);
  doupdate();

#if  defined(SUNOS) || defined(IRIX) || defined(SOLARIS)
  box(MainWindow,ACS_VLINE,ACS_HLINE);
  box(CommandBoxWindow,ACS_VLINE,ACS_HLINE);
  box(CounterBoxWindow,ACS_VLINE,ACS_HLINE);
#else
  box(MainWindow,'|','-');
  box(CommandBoxWindow,'|','-');
  box(CounterBoxWindow,'|','-');
#endif

  raw();

  CursesCenter(TitleWindow,0,1,PROGRAM_TITLE);
  CursesUpdateCounterWindow(CounterWindow,0);
  
  wrefresh(MainWindow);
  wrefresh(CommandWindow);
  wrefresh(CommandBoxWindow);
  wrefresh(CounterWindow);
  wrefresh(CounterBoxWindow);
  wrefresh(DisplayWindow);

#if  defined(SUNOS) || defined(IRIX) || defined(SOLARIS) || defined(BSDI)
  curs_set(1);
#endif
  
  CursesDisplayHelp(DisplayWindow);
  
  CursesMain(DisplayWindow,CommandWindow,CounterWindow);
  
  endwin();
}

/*
  echo();
  nonl();
  idlok(DisplayWindow,FALSE);
  scrollok(DisplayWindow,TRUE);

*/

#endif
