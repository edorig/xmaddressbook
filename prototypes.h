#define PROTOTYPE_H

/* file.c */

ReturnType Convert_AddressBook(char *SrcFilename);
ReturnType Convert_AddressBook_To_1_1(char *SrcFilename, 
				      char *DestFilename);
ReturnType Read_AddressBook(char *Filename);
ReturnType Write_AddressBook(char *Filename);
ReturnType Read_Help_File();
ReturnType Read_Signature_File();
void  Open_AddressBook(Widget   w,
		       Widget   textwidget,   
		       XmAnyCallbackStruct *call_value);
void  Open_AddressBook_Dialog();

/* quit.c */

void  Exit_Program(Widget   w,
		   Widget   textwidget,   
		   XmAnyCallbackStruct *call_value);
void SaveAndExit(Widget   w,
		 Widget   textwidget,   
		 XmAnyCallbackStruct *call_value);
void SaveDontExit(Widget   w,
		 Widget   textwidget,   
		 XmAnyCallbackStruct *call_value);


/* menu.c */

Widget BuildPulldownMenu(Widget parent,
			 char *menu_title, 
			 char menu_mnemonic,
			 MenuItem *items,
			 Bool IsHelpMenu);

/* help.c */

void  Display_Version(Widget   w,
		      Widget   textwidget,   
		      XmAnyCallbackStruct *call_value);
void Display_Help();

/* list.c */

ReturnType Print_List();
ReturnType Delete_From_List(int DeleteNum);
ReturnType Add_To_List(struct Rolo_Struct AddEntry);
ReturnType Update_Entry(struct Rolo_Struct UpdateEntry,
			int CurrentNum);

/* search.c */

void  Search_Form();

/* print.c */

void  Print_AddressBook(Widget   w,
			Widget   argwidget,   
			XmAnyCallbackStruct *call_value);

/* error_msg.c */

void  InfoMessage(char *InfoMessageString);
void  ErrorMessage(char *ErrorMessageString);


/* duplicate.c */

void  Duplicate_Entry();

/* namelist.c */

void Show_Name_List();

/* goto.c */

void Goto_Actual_Entry(int GotoEntryNum);
void Goto_Form();

/* main.c */

void Search_FromMenu();
void Show_BCard_FromMenu();
void Clear_RoloForm();
void CurrentEntry_RoloForm();
void Search_FromMenu();

/* card.c */

void Show_BCard(Rolo_Type *CurrentEntry);


/* mail.c */

void Send_Mail_Popup();


/* preferences.c */

void Show_Preferences();

/* curses.c */

#if defined(CURSES)
void CursesInterface();
#endif
