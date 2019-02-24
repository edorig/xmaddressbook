#define MAIN
#include "main.h"
#include "menu.h"
#include "help.h"


/* function prototypes */
void Print_Version();
void Print_Usage(char *ProgramName);
void Create_RoloForm();
void Create_ButtonBar();
void Clear_RoloForm();
void NextEntry_RoloForm();
void GotoEntry_RoloForm();
void PreviousEntry_RoloForm();
void DeleteEntry_RoloForm();
void AddEntry_RoloForm();
void UpdateEntry_RoloForm();
void CurrentEntry_RoloForm();
void  Help_Line(Widget   w,
		char     *Key);
void  Search_FromMenu();
void  Show_BCard_FromMenu();


/* ButtonBar Widgets */
Widget W_ButtonBar_Frame;
Widget W_Goto_Editor;
Widget W_Add_Button;
Widget W_Delete_Button;
Widget W_Update_Button;
Widget W_Duplicate_Button;

/* RoloForm Widgets */
Widget W_RoloForm_Frame;
Widget W_Name_Editor;
Widget W_WorkPhone_Editor;
Widget W_HomePhone_Editor;
Widget W_FaxNumber_Editor;
Widget W_EmailAddress_Editor;
Widget W_Company_Editor;
Widget W_WorkAddress_Editor;
Widget W_HomeAddress_Editor;
Widget W_DateUpdated_Editor;
Widget W_Remarks_Editor;
Widget W_CurrentNum_Editor;
Widget W_TotalNum_Editor;
Widget W_Help_Label;


#ifndef VMS
void
#else
int
#endif
main(int argc,
char **argv)
{
  int lcv;
  int ConvertAddressbookFlag = False;
  int ConvertAddressbook1_1Flag = False;
  char *UserName = NULL;
  struct passwd *PasswdEntry;
  Bool AlternateUserFile = False;
  Atom WM_DELETE_WINDOW;
  String fallback_resources[] = {
    "*fontList: -misc-fixed-medium-r-normal--14-130-75-75-c-70-iso10646-1",
    NULL,
  };

  
  if (argc >= 1)
    {
      for (lcv=0;lcv < argc; lcv++)
	{
	  if ((strcmp(argv[lcv],"-help") == 0) || (strcmp(argv[lcv],"-h") == 0))
	    {
	      G_ASCII_Mode = True;
	      Print_Usage(argv[0]);
	      exit(0);
	    }
	  if ((strcmp(argv[lcv],"-user") == 0) || (strcmp(argv[lcv],"-u") == 0))
	    {
	      lcv++;
	      if (argc != lcv)
		{
		  UserName = strdup(argv[lcv]);
		  AlternateUserFile = True;
		}
	      break;
	    }
#if defined(CURSES)
	  if ((strcmp(argv[lcv],"-curses") == 0) || (strcmp(argv[lcv],"-c") == 0))
	    {
	      G_CursesInterface = True;
	    }
#endif
	  if ((strcmp(argv[lcv],"-version") == 0) || (strcmp(argv[lcv],"-v") == 0))
	    {
	      Print_Version();
	      exit(0);
	    }
	  if ((strcmp(argv[lcv],"-convert") == 0))
	    {
	      G_ASCII_Mode = True;
	      ConvertAddressbookFlag = True;
	    }
	  if ((strcmp(argv[lcv],"-convert1.1") == 0) || (strcmp(argv[lcv],"-c1.1") == 0))
	    {
	      G_ASCII_Mode = True;
	      ConvertAddressbook1_1Flag = True;
	      if (argc != 4)
		{
		  Print_Usage(argv[0]);
		}
	      Convert_AddressBook_To_1_1(argv[++lcv],argv[++lcv]);
	      exit(0);
	    }
	}
    }

  if (UserName == NULL)
    {
      if ((PasswdEntry = getpwuid(getuid())) == NULL)
	{
	  perror("getpwuid");
	  exit(1);
	}
      UserName = strdup(PasswdEntry->pw_name);
    }

  G_Addressbook_Filename = (char *)malloc(MY_BUFSIZ);
  if (AlternateUserFile == True)
    {
#ifndef VMS
      sprintf(G_Addressbook_Filename,"%s/%s",UserName,DEFAULT_ADDRESSBOOK_FILENAME);
#else
      sprintf(G_Addressbook_Filename,"%s%s",UserName,DEFAULT_ADDRESSBOOK_FILENAME);
#endif
    }
  else {
#ifndef VMS
    sprintf(G_Addressbook_Filename,"%s/%s",getenv("HOME"),
#else
	    sprintf(G_Addressbook_Filename,"%s%s",getenv("HOME"),
#endif
		    DEFAULT_ADDRESSBOOK_FILENAME);
  }

  G_Signature_Filename = (char *)malloc(MY_BUFSIZ);
  sprintf(G_Signature_Filename,"%s/%s",getenv("HOME"),SIGNATURE_FILE);

  if (ConvertAddressbookFlag == True)
    {
      Convert_AddressBook(G_Addressbook_Filename);
      exit (0);
    }

  if ((G_PrinterName = (char *)getenv("PRINTER")) == NULL)
    {
      G_PrinterName = (char *)getenv("LASER");
    }
  
#if defined(CURSES)
  if (G_CursesInterface == True)
    {
      Read_AddressBook(G_Addressbook_Filename);
      CursesInterface();
      exit(0);
    }
#endif

  XtSetLanguageProc(NULL,NULL,NULL); 
  W_TopLevel = XtVaAppInitialize(&G_ApplicationContext, "XmAddressBook", NULL, 0,
				 &argc, argv, fallback_resources, NULL);
  
  
  XtVaSetValues(W_TopLevel,  
		XmNtitle, PROGRAM_TITLE,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL,0);
  
  W_Main_Frame = XtVaCreateManagedWidget("W_Main_Frame",
					 xmFrameWidgetClass, 
					 W_TopLevel,
					 NULL,0);
  
  W_Main_Form = XtVaCreateManagedWidget("W_Main_Form",
					xmFormWidgetClass, 
					W_Main_Frame,
					NULL,0);

  W_MenuBar = XmCreateMenuBar(W_Main_Form, "W_MenuBar", NULL, 0);
  BuildPulldownMenu(W_MenuBar, "File", 'F', File_Menu,False);
  BuildPulldownMenu(W_MenuBar, "Misc", 'M', Misc_Menu,False);
  BuildPulldownMenu(W_MenuBar, "Help", 'H', Help_Menu,True);
  XtVaSetValues(W_MenuBar,
		XmNrightAttachment,XmATTACH_FORM,
		XmNleftAttachment,XmATTACH_FORM,
		XmNtopAttachment,XmATTACH_FORM,
		NULL,0);
  XtManageChild(W_MenuBar);
  XtAddEventHandler(W_MenuBar,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "blank");
  
  Create_ButtonBar();
  Create_RoloForm(); 

  XtRealizeWidget(W_TopLevel);

  Read_AddressBook(G_Addressbook_Filename);
  Read_Help_File();
  Read_Signature_File();
  if (G_RoloList != NULL)
    {
      G_CurrentNumInRoloList = 1;
      G_CurrentEntryInRoloList = G_RoloList;
    }
  CurrentEntry_RoloForm();

  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(W_TopLevel),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(W_TopLevel,WM_DELETE_WINDOW,
			  (XtCallbackProc)SaveAndExit,NULL);

  XtAppMainLoop(G_ApplicationContext);
}
 

/**********************

  Create_RoloForm -- Draw the addressbook form on the screen

***********************/

void Create_RoloForm()
{
  XmString LabelString;
  Widget W_RoloForm_Form;
  Widget W_Name_Label;
  Widget W_WorkPhone_Label;
  Widget W_HomePhone_Label;
  Widget W_Company_Label;
  Widget W_WorkAddress_Label;
  Widget W_HomeAddress_Label;
  Widget W_DateUpdated_Label;
  Widget W_Remarks_Label;
  Widget W_RoloForm_Separator;
  Widget W_CurrentNum_Label;
  Widget W_TotalNum_Label;
  Widget W_FaxNumber_Label;
  Widget W_EmailAddress_Label;
  
  W_RoloForm_Frame = XtVaCreateManagedWidget("W_RoloForm_Frame",
					      xmFrameWidgetClass, 
					      W_Main_Form,
					      XmNleftAttachment, XmATTACH_FORM,
					      XmNrightAttachment, XmATTACH_FORM,
					      XmNbottomAttachment, XmATTACH_FORM,
					      XmNtopAttachment, XmATTACH_WIDGET,
					      XmNtopWidget, W_ButtonBar_Frame,
					      NULL,0);
  
  W_RoloForm_Form = XtVaCreateManagedWidget("W_RoloForm_Form",
					     xmFormWidgetClass, 
					     W_RoloForm_Frame,
					     NULL,0);

  LabelString = XmStringCreate("Name: ", XmFONTLIST_DEFAULT_TAG),
  W_Name_Label= XtVaCreateManagedWidget("W_Name_Label",
					xmLabelWidgetClass,
					W_RoloForm_Form,
					XmNlabelString,LabelString,
					XmNleftAttachment, XmATTACH_FORM,
					XmNrightAttachment, XmATTACH_POSITION,
					XmNrightPosition, 18,
					XmNtopAttachment, XmATTACH_FORM,
					XmNalignment, XmALIGNMENT_END,
					NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_Name_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "name");

  W_Name_Editor= XtVaCreateManagedWidget("W_Name_Editor",
                                         xmTextFieldWidgetClass,
					 W_RoloForm_Form,
					 XmNleftAttachment, XmATTACH_POSITION,
					 XmNleftPosition, 18,
					 XmNrightAttachment, XmATTACH_FORM,
					 XmNtopAttachment, XmATTACH_FORM,
                                         NULL,0);
  XtAddEventHandler(W_Name_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "name");

  LabelString = XmStringCreate("Work Phone: ", XmFONTLIST_DEFAULT_TAG),
  W_WorkPhone_Label= XtVaCreateManagedWidget("W_WorkPhone_Label",
					     xmLabelWidgetClass,
					     W_RoloForm_Form,
					     XmNlabelString,LabelString,
					     XmNleftAttachment, XmATTACH_FORM,
					     XmNrightAttachment, XmATTACH_POSITION,
					     XmNrightPosition, 18,
					     XmNtopAttachment, XmATTACH_WIDGET,
					     XmNtopWidget, W_Name_Editor,
					     XmNalignment, XmALIGNMENT_END,
					     NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_WorkPhone_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "wphone");

  W_WorkPhone_Editor= XtVaCreateManagedWidget("W_WorkPhone_Editor",
					      xmTextFieldWidgetClass,
					      W_RoloForm_Form,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition, 18,
					      XmNrightAttachment, XmATTACH_FORM,
					      XmNtopAttachment, XmATTACH_WIDGET,
					      XmNtopWidget, W_Name_Editor,
					      NULL,0);
  XtAddEventHandler(W_WorkPhone_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "wphone");

  LabelString = XmStringCreate("Fax Number: ", XmFONTLIST_DEFAULT_TAG),
  W_FaxNumber_Label= XtVaCreateManagedWidget("W_FaxNumber_Label",
					     xmLabelWidgetClass,
					     W_RoloForm_Form,
					     XmNlabelString,LabelString,
					     XmNleftAttachment, XmATTACH_FORM,
					     XmNrightAttachment, XmATTACH_POSITION,
					     XmNrightPosition, 18,
					     XmNtopAttachment, XmATTACH_WIDGET,
					     XmNtopWidget, W_WorkPhone_Editor,
					     XmNalignment, XmALIGNMENT_END,
					     NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_FaxNumber_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "fax");

  W_FaxNumber_Editor= XtVaCreateManagedWidget("W_FaxNumber_Editor",
					      xmTextFieldWidgetClass,
					      W_RoloForm_Form,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition, 18,
					      XmNrightAttachment, XmATTACH_FORM,
					      XmNtopAttachment, XmATTACH_WIDGET,
					      XmNtopWidget, W_WorkPhone_Editor,
					      NULL,0);
  XtAddEventHandler(W_FaxNumber_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "fax");

  LabelString = XmStringCreate("Home Phone: ", XmFONTLIST_DEFAULT_TAG),
  W_HomePhone_Label= XtVaCreateManagedWidget("W_HomePhone_Label",
					     xmLabelWidgetClass,
					     W_RoloForm_Form,
					     XmNlabelString,LabelString,
					     XmNleftAttachment, XmATTACH_FORM,
					     XmNrightAttachment, XmATTACH_POSITION,
					     XmNrightPosition, 18,
					     XmNtopAttachment, XmATTACH_WIDGET,
					     XmNtopWidget, W_FaxNumber_Editor,
					     XmNalignment, XmALIGNMENT_END,
					     NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_HomePhone_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "hphone");

  W_HomePhone_Editor= XtVaCreateManagedWidget("W_HomePhone_Editor",
					      xmTextFieldWidgetClass,
					      W_RoloForm_Form,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition, 18,
					      XmNrightAttachment, XmATTACH_FORM,
					      XmNtopAttachment, XmATTACH_WIDGET,
					      XmNtopWidget, W_FaxNumber_Editor,
					      NULL,0);
  XtAddEventHandler(W_HomePhone_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "hphone");

  LabelString = XmStringCreate("Company: ", XmFONTLIST_DEFAULT_TAG),
  W_Company_Label= XtVaCreateManagedWidget("W_Company_Label",
					   xmLabelWidgetClass,
					   W_RoloForm_Form,
					   XmNlabelString,LabelString,
					   XmNleftAttachment, XmATTACH_FORM,
					   XmNrightAttachment, XmATTACH_POSITION,
					   XmNrightPosition, 18,
					   XmNtopAttachment, XmATTACH_WIDGET,
					   XmNtopWidget, W_HomePhone_Editor,
					   XmNalignment, XmALIGNMENT_END,
					   NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_Company_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "company");

  W_Company_Editor= XtVaCreateManagedWidget("W_Company_Editor",
					    xmTextFieldWidgetClass,
					    W_RoloForm_Form,
					    XmNleftAttachment, XmATTACH_POSITION,
					    XmNleftPosition, 18,
					    XmNrightAttachment, XmATTACH_FORM,
					    XmNtopAttachment, XmATTACH_WIDGET,
					    XmNtopWidget, W_HomePhone_Editor,
					    NULL,0);
  XtAddEventHandler(W_Company_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "company");
  
  LabelString = XmStringCreate("Email Address: ", XmFONTLIST_DEFAULT_TAG),
  W_EmailAddress_Label= XtVaCreateManagedWidget("W_EmailAddress_Label",
					     xmLabelWidgetClass,
					     W_RoloForm_Form,
					     XmNlabelString,LabelString,
					     XmNleftAttachment, XmATTACH_FORM,
					     XmNrightAttachment, XmATTACH_POSITION,
					     XmNrightPosition, 18,
					     XmNtopAttachment, XmATTACH_WIDGET,
					     XmNtopWidget, W_Company_Editor,
					     XmNalignment, XmALIGNMENT_END,
					     NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_EmailAddress_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "email");

  W_EmailAddress_Editor= XtVaCreateManagedWidget("W_EmailAddress_Editor",
					      xmTextFieldWidgetClass,
					      W_RoloForm_Form,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition, 18,
					      XmNrightAttachment, XmATTACH_FORM,
					      XmNtopAttachment, XmATTACH_WIDGET,
					      XmNtopWidget, W_Company_Editor,
					      NULL,0);
  XtAddEventHandler(W_EmailAddress_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "email");

  LabelString = XmStringCreate("Work Address: ", XmFONTLIST_DEFAULT_TAG),
  W_WorkAddress_Label= XtVaCreateManagedWidget("W_WorkAddress_Label",
					       xmLabelWidgetClass,
					       W_RoloForm_Form,
					       XmNlabelString,LabelString,
					       XmNleftAttachment, XmATTACH_FORM,
					       XmNrightAttachment, XmATTACH_POSITION,
					       XmNrightPosition, 18,
					       XmNtopAttachment, XmATTACH_WIDGET,
					       XmNtopWidget, W_EmailAddress_Editor,
					       XmNalignment, XmALIGNMENT_END,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_WorkAddress_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "waddress");

  W_WorkAddress_Editor= XtVaCreateManagedWidget("W_WorkAddress_Editor",
						xmTextFieldWidgetClass,
						W_RoloForm_Form,
						XmNleftAttachment, XmATTACH_POSITION,
						XmNleftPosition, 18,
						XmNrightAttachment, XmATTACH_FORM,
						XmNtopAttachment, XmATTACH_WIDGET,
						XmNtopWidget, W_EmailAddress_Editor,
						NULL,0);
  XtAddEventHandler(W_WorkAddress_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "waddress");
  
  LabelString = XmStringCreate("Home Address: ", XmFONTLIST_DEFAULT_TAG),
  W_HomeAddress_Label= XtVaCreateManagedWidget("W_HomeAddress_Label",
					       xmLabelWidgetClass,
					       W_RoloForm_Form,
					       XmNlabelString,LabelString,
					       XmNleftAttachment, XmATTACH_FORM,
					       XmNrightAttachment, XmATTACH_POSITION,
					       XmNrightPosition, 18,
					       XmNtopAttachment, XmATTACH_WIDGET,
					       XmNtopWidget, W_WorkAddress_Editor,
					       XmNalignment, XmALIGNMENT_END,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_HomeAddress_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "haddress");

  W_HomeAddress_Editor= XtVaCreateManagedWidget("W_HomeAddress_Editor",
						xmTextFieldWidgetClass,
						W_RoloForm_Form,
						XmNleftAttachment, XmATTACH_POSITION,
						XmNleftPosition, 18,
						XmNrightAttachment, XmATTACH_FORM,
						XmNtopAttachment, XmATTACH_WIDGET,
						XmNtopWidget, W_WorkAddress_Editor,
						NULL,0);
  XtAddEventHandler(W_HomeAddress_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "haddress");

  
  LabelString = XmStringCreate("Date Updated: ", XmFONTLIST_DEFAULT_TAG),
  W_DateUpdated_Label= XtVaCreateManagedWidget("W_DateUpdated_Label",
					       xmLabelWidgetClass,
					       W_RoloForm_Form,
					       XmNlabelString,LabelString,
					       XmNleftAttachment, XmATTACH_FORM,
					       XmNrightAttachment, XmATTACH_POSITION,
					       XmNrightPosition, 18,
					       XmNtopAttachment, XmATTACH_WIDGET,
					       XmNtopWidget, W_HomeAddress_Editor,
					       XmNalignment, XmALIGNMENT_END,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_DateUpdated_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "date");

  W_DateUpdated_Editor= XtVaCreateManagedWidget("W_DateUpdated_Editor",
						xmTextFieldWidgetClass,
						W_RoloForm_Form,
						XmNleftAttachment, XmATTACH_POSITION,
						XmNleftPosition, 18,
						XmNrightAttachment, XmATTACH_FORM,
						XmNtopAttachment, XmATTACH_WIDGET,
						XmNtopWidget, W_HomeAddress_Editor,
						XmNeditable, False,
						NULL,0);
  XtAddEventHandler(W_DateUpdated_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "date");
  
  LabelString = XmStringCreate("Remarks: ", XmFONTLIST_DEFAULT_TAG),
  W_Remarks_Label= XtVaCreateManagedWidget("W_Remarks_Label",
					   xmLabelWidgetClass,
					   W_RoloForm_Form,
					   XmNlabelString,LabelString,
					   XmNleftAttachment, XmATTACH_FORM,
					   XmNrightAttachment, XmATTACH_POSITION,
					   XmNrightPosition, 18,
					   XmNtopAttachment, XmATTACH_WIDGET,
					   XmNtopWidget, W_DateUpdated_Editor,
					   XmNalignment, XmALIGNMENT_END,
					   XmNalignment, XmALIGNMENT_END,
					   NULL,0);
  XtAddEventHandler(W_Remarks_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "remarks");


  XmStringFree(LabelString);
  W_Remarks_Editor= XtVaCreateManagedWidget("W_Remarks_Editor",
					    xmTextWidgetClass,
					    W_RoloForm_Form,
					    XmNleftAttachment, XmATTACH_POSITION,
					    XmNleftPosition, 18,
					    XmNrightAttachment, XmATTACH_FORM,
					    XmNtopAttachment, XmATTACH_WIDGET,
					    XmNtopWidget, W_DateUpdated_Editor,
					    XmNrows, 5,
					    XmNcolumns, 30,
					    XmNeditMode, XmMULTI_LINE_EDIT,
					    XmNmaxLength, MY_BIG_BUFSIZ,
					    NULL,0);
  XtAddEventHandler(W_Remarks_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "remarks");

  W_RoloForm_Separator = XtVaCreateManagedWidget("W_RoloForm_Separator",
						 xmSeparatorWidgetClass, 
						 W_RoloForm_Form,
						 XmNleftAttachment, XmATTACH_FORM,
						 XmNrightAttachment, XmATTACH_FORM,
						 XmNtopAttachment, XmATTACH_WIDGET,
						 XmNtopWidget, W_Remarks_Editor,
						 XmNorientation, XmHORIZONTAL,
						 NULL, 0);
  
  
  LabelString = XmStringCreate("Current Entry:  ", XmFONTLIST_DEFAULT_TAG),
  W_CurrentNum_Label= XtVaCreateManagedWidget("W_CurrentNum_Label",
					      xmLabelWidgetClass,
					      W_RoloForm_Form,
					      XmNlabelString,LabelString,
					      XmNleftAttachment, XmATTACH_FORM,
					      XmNtopAttachment, XmATTACH_WIDGET,
					      XmNtopWidget, W_RoloForm_Separator,
/* 					      XmNbottomAttachment, XmATTACH_FORM, */
					      NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_CurrentNum_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "current");

  W_CurrentNum_Editor= XtVaCreateManagedWidget("W_CurrentNum_Editor",
					       xmTextFieldWidgetClass,
					       W_RoloForm_Form,
					       XmNleftAttachment, XmATTACH_WIDGET,
					       XmNleftWidget, W_CurrentNum_Label,
					       XmNtopAttachment, XmATTACH_WIDGET,
					       XmNtopWidget, W_RoloForm_Separator,
/* 					       XmNbottomAttachment, XmATTACH_FORM, */
					       XmNeditable, False,
					       NULL,0);
  XtAddEventHandler(W_CurrentNum_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "current");
  
  
  LabelString = XmStringCreate("Total Number of Entries:  ", XmFONTLIST_DEFAULT_TAG),
  W_TotalNum_Label= XtVaCreateManagedWidget("W_TotalNum_Label",
					    xmLabelWidgetClass,
					    W_RoloForm_Form,
					    XmNlabelString,LabelString,
					    XmNleftAttachment, XmATTACH_WIDGET,
					    XmNleftWidget, W_CurrentNum_Editor,
					    XmNtopAttachment, XmATTACH_WIDGET,
					    XmNtopWidget, W_RoloForm_Separator,
/* 					    XmNbottomAttachment, XmATTACH_FORM, */
					    NULL,0);
  XmStringFree(LabelString);
  XtAddEventHandler(W_TotalNum_Label,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "total");

  W_TotalNum_Editor= XtVaCreateManagedWidget("W_TotalNum_Editor",
					     xmTextFieldWidgetClass,
					     W_RoloForm_Form,
					     XmNleftAttachment, XmATTACH_WIDGET,
					     XmNleftWidget, W_TotalNum_Label,
					     XmNrightAttachment, XmATTACH_FORM,
					     XmNtopAttachment, XmATTACH_WIDGET,
					     XmNtopWidget, W_RoloForm_Separator,
/* 					     XmNbottomAttachment, XmATTACH_FORM, */
					     XmNeditable, False,
					     NULL,0);

  XtAddEventHandler(W_TotalNum_Editor,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "total");

  W_RoloForm_Separator = XtVaCreateManagedWidget("W_RoloForm_Separator",
						 xmSeparatorWidgetClass, 
						 W_RoloForm_Form,
						 XmNleftAttachment, XmATTACH_FORM,
						 XmNrightAttachment, XmATTACH_FORM,
						 XmNtopAttachment, XmATTACH_WIDGET,
						 XmNtopWidget, W_CurrentNum_Editor,
						 XmNorientation, XmHORIZONTAL,
						 NULL, 0);
  
  LabelString = XmStringCreate("Help: ", XmFONTLIST_DEFAULT_TAG),
  W_Help_Label= XtVaCreateManagedWidget("W_Help_Label",
					xmLabelWidgetClass,
					W_RoloForm_Form,
					XmNlabelString,LabelString,
					XmNleftAttachment, XmATTACH_FORM,
					XmNleftOffset, 5,
					XmNrightAttachment, XmATTACH_FORM,
					XmNrightOffset, 5,
					XmNbottomAttachment, XmATTACH_FORM,
					XmNbottomOffset, 5,
					XmNtopAttachment, XmATTACH_WIDGET,
					XmNtopWidget, W_RoloForm_Separator,
					XmNtopOffset, 5,
					XmNalignment, XmALIGNMENT_BEGINNING,
					XmNrecomputeSize, False,
					NULL,0);
  XmStringFree(LabelString);
}


void Create_ButtonBar()
{
  XmString LabelString;
  Widget W_ButtonBar_Form;
  Widget W_ButtonBar_RC;
  Widget W_Next_Button;
  Widget W_Prev_Button;
  Widget W_Search_Button;
  Widget W_Goto_Button;
  Widget W_Clear_Button;

  W_ButtonBar_Frame = XtVaCreateManagedWidget("W_ButtonBar_Frame",
					      xmFrameWidgetClass, 
					      W_Main_Form,
					      XmNleftAttachment, XmATTACH_FORM,
					      XmNrightAttachment, XmATTACH_FORM,
					      XmNtopAttachment, XmATTACH_WIDGET,
					      XmNtopWidget, W_MenuBar,
					      NULL,0);
  
  W_ButtonBar_Form = XtVaCreateManagedWidget("W_ButtonBar_Form",
					     xmFormWidgetClass, 
					     W_ButtonBar_Frame,
					     NULL,0);

  W_ButtonBar_RC = XtVaCreateManagedWidget("W_ButtonBar_RC",
					   xmRowColumnWidgetClass, 
					   W_ButtonBar_Form,
					   XmNleftAttachment, XmATTACH_FORM,
					   XmNrightAttachment, XmATTACH_FORM,
					   XmNtopAttachment, XmATTACH_FORM,
					   XmNorientation, XmHORIZONTAL,
					   XmNpacking, XmPACK_COLUMN,
					   XmNentryAlignment, XmALIGNMENT_CENTER,
					   NULL,0);

  LabelString = XmStringCreate("Previous", XmFONTLIST_DEFAULT_TAG),
  W_Prev_Button= XtVaCreateManagedWidget("W_Prev_Button",
					 xmArrowButtonWidgetClass,
					 W_ButtonBar_RC,
					 XmNlabelString,LabelString,
					 XmNarrowDirection, XmARROW_LEFT,
					 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Prev_Button, XmNactivateCallback,  
		(XtCallbackProc)  PreviousEntry_RoloForm, NULL);
  XtAddEventHandler(W_Prev_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "prev");


  LabelString = XmStringCreate("Next", XmFONTLIST_DEFAULT_TAG),
  W_Next_Button= XtVaCreateManagedWidget("W_Next_Button",
					 xmArrowButtonWidgetClass,
					 W_ButtonBar_RC,
					 XmNlabelString,LabelString,
					 XmNarrowDirection, XmARROW_RIGHT,
					 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Next_Button, XmNactivateCallback,  
		(XtCallbackProc)  NextEntry_RoloForm, NULL);
  XtAddEventHandler(W_Next_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "next");

  LabelString = XmStringCreate("Search", XmFONTLIST_DEFAULT_TAG),
  W_Search_Button= XtVaCreateManagedWidget("W_Search_Button",
					 xmPushButtonWidgetClass,
					 W_ButtonBar_RC,
					 XmNlabelString,LabelString,
					 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Search_Button, XmNactivateCallback,  
		(XtCallbackProc)  Search_FromMenu, NULL);
  XtAddEventHandler(W_Search_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "search");

  LabelString = XmStringCreate("Goto", XmFONTLIST_DEFAULT_TAG),
  W_Goto_Button= XtVaCreateManagedWidget("W_Goto_Button",
					 xmPushButtonWidgetClass,
					 W_ButtonBar_RC,
					 XmNlabelString,LabelString,
					 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Goto_Button, XmNactivateCallback,  
		(XtCallbackProc)  Goto_Form, NULL);
  XtAddEventHandler(W_Goto_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "goto");

  LabelString = XmStringCreate("Add", XmFONTLIST_DEFAULT_TAG),
  W_Add_Button= XtVaCreateManagedWidget("W_Add_Button",
					 xmPushButtonWidgetClass,
					 W_ButtonBar_RC,
					 XmNlabelString,LabelString,
					 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Add_Button, XmNactivateCallback,  
		(XtCallbackProc)  AddEntry_RoloForm, NULL);
  XtAddEventHandler(W_Add_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "add");

  LabelString = XmStringCreate("Delete", XmFONTLIST_DEFAULT_TAG),
  W_Delete_Button= XtVaCreateManagedWidget("W_Delete_Button",
					 xmPushButtonWidgetClass,
					 W_ButtonBar_RC,
					 XmNlabelString,LabelString,
					 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Delete_Button, XmNactivateCallback,  
		(XtCallbackProc)  DeleteEntry_RoloForm, NULL);
  XtAddEventHandler(W_Delete_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "delete");

  LabelString = XmStringCreate("Update", XmFONTLIST_DEFAULT_TAG),
  W_Update_Button= XtVaCreateManagedWidget("W_Update_Button",
					 xmPushButtonWidgetClass,
					 W_ButtonBar_RC,
					 XmNlabelString,LabelString,
					 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Update_Button, XmNactivateCallback,  
		(XtCallbackProc)  UpdateEntry_RoloForm, NULL);
  XtAddEventHandler(W_Update_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "update");

  LabelString = XmStringCreate("Clear", XmFONTLIST_DEFAULT_TAG),
  W_Clear_Button= XtVaCreateManagedWidget("W_Clear_Button",
					 xmPushButtonWidgetClass,
					 W_ButtonBar_RC,
					 XmNlabelString,LabelString,
					 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Clear_Button, XmNactivateCallback,  
		(XtCallbackProc)  Clear_RoloForm, NULL);
  XtAddEventHandler(W_Clear_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "clear");
  
/*
  LabelString = XmStringCreate("Duplicate", XmFONTLIST_DEFAULT_TAG),
  W_Duplicate_Button= XtVaCreateManagedWidget("W_Duplicate_Button",
					      xmPushButtonWidgetClass,
					      W_ButtonBar_RC,
					      XmNlabelString,LabelString,
					      NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(W_Duplicate_Button, XmNactivateCallback,  
		(XtCallbackProc)  Duplicate_Entry, NULL);
  XtAddEventHandler(W_Duplicate_Button,EnterWindowMask,False,(XtEventHandler) Help_Line, 
		    (XtPointer) "duplicate");

*/

}

void Clear_RoloForm()
{
  char Buffer[MY_BUFSIZ+1];

  XtSetSensitive(W_Update_Button,False);
  XtSetSensitive(W_Delete_Button,False);
  XmTextFieldSetString(W_Name_Editor,"");
  XmTextFieldSetString(W_WorkPhone_Editor,"");
  XmTextFieldSetString(W_HomePhone_Editor,"");
  XmTextFieldSetString(W_FaxNumber_Editor,"");
  XmTextFieldSetString(W_EmailAddress_Editor,"");
  XmTextFieldSetString(W_Company_Editor,"");
  XmTextFieldSetString(W_WorkAddress_Editor,"");
  XmTextFieldSetString(W_HomeAddress_Editor,"");
  XmTextFieldSetString(W_DateUpdated_Editor,"");
  XmTextSetString(W_Remarks_Editor,"");
  XmTextFieldSetString(W_CurrentNum_Editor,"0");
  sprintf(Buffer,"%d",G_NumInRoloList);
  XmTextFieldSetString(W_TotalNum_Editor,Buffer);
  G_CurrentNumInRoloList = 0;
}

void NextEntry_RoloForm()
{
  char Buffer[MY_BUFSIZ+1];

  XtSetSensitive(W_Update_Button,True);
  XtSetSensitive(W_Delete_Button,True);
  if (G_RoloList == NULL)
    {
      Clear_RoloForm();
      return;
    }
  if ((G_CurrentEntryInRoloList == NULL) || (G_CurrentNumInRoloList >= G_NumInRoloList))
    {
      G_CurrentNumInRoloList = 1;
      G_CurrentEntryInRoloList = G_RoloList;
    }
  else {
    G_CurrentEntryInRoloList = G_CurrentEntryInRoloList->Next;
    G_CurrentNumInRoloList++;
  }
  if (G_CurrentEntryInRoloList != NULL)
    {
      XmTextFieldSetString(W_Name_Editor,G_CurrentEntryInRoloList->Name);
      XmTextFieldSetString(W_WorkPhone_Editor,G_CurrentEntryInRoloList->WorkPhone);
      XmTextFieldSetString(W_HomePhone_Editor,G_CurrentEntryInRoloList->HomePhone);
      XmTextFieldSetString(W_Company_Editor,G_CurrentEntryInRoloList->Company);
      XmTextFieldSetString(W_FaxNumber_Editor,G_CurrentEntryInRoloList->FaxNumber);
      XmTextFieldSetString(W_EmailAddress_Editor,G_CurrentEntryInRoloList->EmailAddress);
      XmTextFieldSetString(W_WorkAddress_Editor,G_CurrentEntryInRoloList->WorkAddress);
      XmTextFieldSetString(W_HomeAddress_Editor,G_CurrentEntryInRoloList->HomeAddress);
      XmTextFieldSetString(W_DateUpdated_Editor,G_CurrentEntryInRoloList->DateUpdated);
      XmTextSetString(W_Remarks_Editor,G_CurrentEntryInRoloList->Remarks);
      sprintf(Buffer,"%d",G_CurrentNumInRoloList);
      XmTextFieldSetString(W_CurrentNum_Editor,Buffer);
      sprintf(Buffer,"%d",G_NumInRoloList);
      XmTextFieldSetString(W_TotalNum_Editor,Buffer);
    }
  if (G_BCardVisible == True)
    {
      Show_BCard(G_CurrentEntryInRoloList);
    }
}

void CurrentEntry_RoloForm()
{
  char Buffer[MY_BUFSIZ+1];

  if (G_RoloList == NULL)
    {
      Clear_RoloForm();
      return;
    }
  if (G_CurrentEntryInRoloList == NULL)
    {
/*       fprintf(stderr,"G_CurrentEntryInRoloList == NULL\n"); */
      G_CurrentNumInRoloList = 1;
      G_CurrentEntryInRoloList = G_RoloList;
    }
/*  else {
    G_CurrentEntryInRoloList = G_CurrentEntryInRoloList->Next;
  }
*/
/*   fprintf(stderr,"%s %d\n",G_CurrentEntryInRoloList->Name,G_CurrentNumInRoloList); */
  if (G_CurrentEntryInRoloList != NULL)
    {
      if (G_BCardVisible == True)
	{
	  Show_BCard(G_CurrentEntryInRoloList);
	}

      XmTextFieldSetString(W_Name_Editor,G_CurrentEntryInRoloList->Name);
      XmTextFieldSetString(W_WorkPhone_Editor,G_CurrentEntryInRoloList->WorkPhone);
      XmTextFieldSetString(W_HomePhone_Editor,G_CurrentEntryInRoloList->HomePhone);
      XmTextFieldSetString(W_Company_Editor,G_CurrentEntryInRoloList->Company);
      XmTextFieldSetString(W_FaxNumber_Editor,G_CurrentEntryInRoloList->FaxNumber);
      XmTextFieldSetString(W_EmailAddress_Editor,G_CurrentEntryInRoloList->EmailAddress);
      XmTextFieldSetString(W_WorkAddress_Editor,G_CurrentEntryInRoloList->WorkAddress);
      XmTextFieldSetString(W_HomeAddress_Editor,G_CurrentEntryInRoloList->HomeAddress);
      XmTextFieldSetString(W_DateUpdated_Editor,G_CurrentEntryInRoloList->DateUpdated);
      XmTextSetString(W_Remarks_Editor,G_CurrentEntryInRoloList->Remarks);
      sprintf(Buffer,"%d",G_CurrentNumInRoloList);
      XmTextFieldSetString(W_CurrentNum_Editor,Buffer);
      sprintf(Buffer,"%d",G_NumInRoloList);
      XmTextFieldSetString(W_TotalNum_Editor,Buffer);
    }
  if (G_BCardVisible == True)
    {
      Show_BCard(G_CurrentEntryInRoloList);
    }
}


void GotoEntry_RoloForm()
{
  char *GotoEntryStr = NULL;
  int GotoEntryNum = 0;
  
  XtSetSensitive(W_Update_Button,True);
  XtSetSensitive(W_Delete_Button,True);
  if ((GotoEntryStr = XmTextFieldGetString(W_Goto_Editor)) == NULL)
    {
      return;
    }
  GotoEntryNum = atoi(GotoEntryStr);
  XtFree(GotoEntryStr);
  Goto_Actual_Entry(GotoEntryNum);
  if (G_BCardVisible == True)
    {
      Show_BCard(G_CurrentEntryInRoloList);
    }
}

void Goto_Actual_Entry(int GotoEntryNum)
{
  char Buffer[MY_BUFSIZ+1];

  if (G_RoloList == NULL)
    {
      Clear_RoloForm();
      return;
    }
  if (GotoEntryNum > G_NumInRoloList)
    {
      GotoEntryNum = G_NumInRoloList;
    }
  if (GotoEntryNum <= 0)
    {
      GotoEntryNum = 1;
    }
  G_CurrentNumInRoloList = 1;
  G_CurrentEntryInRoloList = G_RoloList;
  while ((G_CurrentEntryInRoloList != NULL) && (G_CurrentNumInRoloList != GotoEntryNum))
    {
      G_CurrentEntryInRoloList = G_CurrentEntryInRoloList->Next;
      G_CurrentNumInRoloList++;
    }

  if (G_CurrentEntryInRoloList != NULL)
    {
      XmTextFieldSetString(W_Name_Editor,G_CurrentEntryInRoloList->Name);
      XmTextFieldSetString(W_WorkPhone_Editor,G_CurrentEntryInRoloList->WorkPhone);
      XmTextFieldSetString(W_HomePhone_Editor,G_CurrentEntryInRoloList->HomePhone);
      XmTextFieldSetString(W_Company_Editor,G_CurrentEntryInRoloList->Company);
      XmTextFieldSetString(W_FaxNumber_Editor,G_CurrentEntryInRoloList->FaxNumber);
      XmTextFieldSetString(W_EmailAddress_Editor,G_CurrentEntryInRoloList->EmailAddress);
      XmTextFieldSetString(W_WorkAddress_Editor,G_CurrentEntryInRoloList->WorkAddress);
      XmTextFieldSetString(W_HomeAddress_Editor,G_CurrentEntryInRoloList->HomeAddress);
      XmTextFieldSetString(W_DateUpdated_Editor,G_CurrentEntryInRoloList->DateUpdated);
      XmTextSetString(W_Remarks_Editor,G_CurrentEntryInRoloList->Remarks);
      sprintf(Buffer,"%d",G_CurrentNumInRoloList);
      XmTextFieldSetString(W_CurrentNum_Editor,Buffer);
      sprintf(Buffer,"%d",G_NumInRoloList);
      XmTextFieldSetString(W_TotalNum_Editor,Buffer);
    }
  if (G_BCardVisible == True)
    {
      Show_BCard(G_CurrentEntryInRoloList);
    }
}


void PreviousEntry_RoloForm()
{
  char Buffer[MY_BUFSIZ+1];
  int GotoEntryNum = 0;
  
  XtSetSensitive(W_Update_Button,True);
  XtSetSensitive(W_Delete_Button,True);
  if (G_RoloList == NULL)
    {
      Clear_RoloForm();
      return;
    }
  G_CurrentNumInRoloList--;
  GotoEntryNum = G_CurrentNumInRoloList;
  if (G_CurrentNumInRoloList <= 0)
    {
      GotoEntryNum = G_NumInRoloList;
    }
  G_CurrentNumInRoloList = 1;
  G_CurrentEntryInRoloList = G_RoloList;
  while ((G_CurrentEntryInRoloList != NULL) && (G_CurrentNumInRoloList != GotoEntryNum))
    {
      G_CurrentEntryInRoloList = G_CurrentEntryInRoloList->Next;
      G_CurrentNumInRoloList++;
    }

  if (G_CurrentEntryInRoloList != NULL)
    {
      XmTextFieldSetString(W_Name_Editor,G_CurrentEntryInRoloList->Name);
      XmTextFieldSetString(W_WorkPhone_Editor,G_CurrentEntryInRoloList->WorkPhone);
      XmTextFieldSetString(W_HomePhone_Editor,G_CurrentEntryInRoloList->HomePhone);
      XmTextFieldSetString(W_Company_Editor,G_CurrentEntryInRoloList->Company);
      XmTextFieldSetString(W_FaxNumber_Editor,G_CurrentEntryInRoloList->FaxNumber);
      XmTextFieldSetString(W_EmailAddress_Editor,G_CurrentEntryInRoloList->EmailAddress);
      XmTextFieldSetString(W_WorkAddress_Editor,G_CurrentEntryInRoloList->WorkAddress);
      XmTextFieldSetString(W_HomeAddress_Editor,G_CurrentEntryInRoloList->HomeAddress);
      XmTextFieldSetString(W_DateUpdated_Editor,G_CurrentEntryInRoloList->DateUpdated);
      XmTextSetString(W_Remarks_Editor,G_CurrentEntryInRoloList->Remarks);
      sprintf(Buffer,"%d",G_CurrentNumInRoloList);
      XmTextFieldSetString(W_CurrentNum_Editor,Buffer);
      sprintf(Buffer,"%d",G_NumInRoloList);
      XmTextFieldSetString(W_TotalNum_Editor,Buffer);
    }
  if (G_BCardVisible == True)
    {
      Show_BCard(G_CurrentEntryInRoloList);
    }
}


void AddEntry_RoloForm()
{
  struct Rolo_Struct Entry;
  XmString ListString;

  G_ChangedNotSaved = True;
  XtSetSensitive(W_Update_Button,True);
  XtSetSensitive(W_Delete_Button,True);
  Entry.Name = XmTextFieldGetString(W_Name_Editor);
  Entry.WorkPhone = XmTextFieldGetString(W_WorkPhone_Editor);
  Entry.HomePhone = XmTextFieldGetString(W_HomePhone_Editor);
  Entry.Company = XmTextFieldGetString(W_Company_Editor);
  Entry.FaxNumber = XmTextFieldGetString(W_FaxNumber_Editor);
  Entry.EmailAddress =  XmTextFieldGetString(W_EmailAddress_Editor);
  Entry.WorkAddress = XmTextFieldGetString(W_WorkAddress_Editor);
  Entry.HomeAddress = XmTextFieldGetString(W_HomeAddress_Editor);
  Entry.DateUpdated = strdup(Get_Current_Date_Time("%a %X %x"));
  Entry.Remarks = XmTextGetString(W_Remarks_Editor);

  if ((strlen(Entry.Name) == 0) &&
      (strlen(Entry.WorkPhone) == 0) &&
      (strlen(Entry.HomePhone) == 0) &&
      (strlen(Entry.Company) == 0) &&
      (strlen(Entry.FaxNumber) == 0) &&
      (strlen(Entry.EmailAddress) == 0) &&
      (strlen(Entry.WorkAddress) == 0) &&
      (strlen(Entry.HomeAddress) == 0) &&
      (strlen(Entry.Remarks) == 0))
    {
      ErrorMessage("Addition Error: \n\nEntire entry blank. Entry not added.\n");
      return;
    }

  Add_To_List(Entry);
  CurrentEntry_RoloForm();
  
  if (G_NameListVisible == True)
    {
/*       fprintf(stderr,"ADD TO NAMELIST %d\n",G_CurrentNumInRoloList); */
      ListString = XmStringCreate(Entry.Name, XmFONTLIST_DEFAULT_TAG);
      XmListAddItem(NameList_List,ListString,G_CurrentNumInRoloList);
      XmListSelectPos(NameList_List,G_CurrentNumInRoloList,True);
      XmStringFree(ListString);
    }
  
  XtFree(Entry.Name);
  XtFree(Entry.WorkPhone);
  XtFree(Entry.HomePhone);
  XtFree(Entry.FaxNumber);
  XtFree(Entry.EmailAddress);
  XtFree(Entry.Company);
  XtFree(Entry.WorkAddress);
  XtFree(Entry.HomeAddress);
  XtFree(Entry.DateUpdated);
  XtFree(Entry.Remarks);
}

void UpdateEntry_RoloForm()
{
  struct Rolo_Struct Entry;
  XmString ListString;

  G_ChangedNotSaved = True;
  Entry.Name = XmTextFieldGetString(W_Name_Editor);
  Entry.WorkPhone = XmTextFieldGetString(W_WorkPhone_Editor);
  Entry.HomePhone = XmTextFieldGetString(W_HomePhone_Editor);
  Entry.FaxNumber = XmTextFieldGetString(W_FaxNumber_Editor);
  Entry.EmailAddress =  XmTextFieldGetString(W_EmailAddress_Editor);
  Entry.Company = XmTextFieldGetString(W_Company_Editor);
  Entry.WorkAddress = XmTextFieldGetString(W_WorkAddress_Editor);
  Entry.HomeAddress = XmTextFieldGetString(W_HomeAddress_Editor);
  Entry.DateUpdated = strdup(Get_Current_Date_Time("%a %X %x"));
  Entry.Remarks = XmTextGetString(W_Remarks_Editor);
  
  if ((strlen(Entry.Name) == 0) &&
      (strlen(Entry.WorkPhone) == 0) &&
      (strlen(Entry.HomePhone) == 0) &&
      (strlen(Entry.Company) == 0) &&
      (strlen(Entry.FaxNumber) == 0) &&
      (strlen(Entry.EmailAddress) == 0) &&
      (strlen(Entry.WorkAddress) == 0) &&
      (strlen(Entry.HomeAddress) == 0) &&
      (strlen(Entry.Remarks) == 0))
    {
      ErrorMessage("Update Error: \n\nEntire entry blank. Entry not updated.\n");
      return;
    }

  Update_Entry(Entry,G_CurrentNumInRoloList);
  CurrentEntry_RoloForm();
  
  if (G_NameListVisible == True)
    {
/*       fprintf(stderr,"UPDATE TO NAMELIST  %d\n",G_CurrentNumInRoloList); */
      XmListDeletePos(NameList_List,G_CurrentNumInRoloList); 
      ListString = XmStringCreate(Entry.Name, XmFONTLIST_DEFAULT_TAG);
      XmListAddItem(NameList_List,ListString,G_CurrentNumInRoloList);
      XmStringFree(ListString);
    }

  XtFree(Entry.Name);
  XtFree(Entry.WorkPhone);
  XtFree(Entry.HomePhone);
  XtFree(Entry.FaxNumber);
  XtFree(Entry.EmailAddress);
  XtFree(Entry.Company);
  XtFree(Entry.WorkAddress);
  XtFree(Entry.HomeAddress);
  XtFree(Entry.DateUpdated);
  XtFree(Entry.Remarks);
}


void DeleteEntry_RoloForm()
{
  G_ChangedNotSaved = True;
  if (G_CurrentNumInRoloList != 0)
    {
      Delete_From_List(G_CurrentNumInRoloList);
    }

  if (G_NameListVisible == True)
    {
/*       fprintf(stderr,"DELETE FROM NAMELIST %d\n",G_CurrentNumInRoloList);  */
      XmListDeletePos(NameList_List,G_CurrentNumInRoloList);
    }

  CurrentEntry_RoloForm();
}

void SearchEntry_RoloForm()
{
  struct Rolo_Struct Entry;
  
  XtSetSensitive(W_Update_Button,True);
  XtSetSensitive(W_Delete_Button,True);
  Entry.Name = XmTextFieldGetString(W_Name_Editor);
  Entry.WorkPhone = XmTextFieldGetString(W_WorkPhone_Editor);
  Entry.HomePhone = XmTextFieldGetString(W_HomePhone_Editor);
  Entry.FaxNumber = XmTextFieldGetString(W_FaxNumber_Editor);
  Entry.EmailAddress =  XmTextFieldGetString(W_EmailAddress_Editor);
  Entry.Company = XmTextFieldGetString(W_Company_Editor);
  Entry.WorkAddress = XmTextFieldGetString(W_WorkAddress_Editor);
  Entry.HomeAddress = XmTextFieldGetString(W_HomeAddress_Editor);
  Entry.DateUpdated = XmTextFieldGetString(W_DateUpdated_Editor);
  Entry.Remarks = XmTextGetString(W_Remarks_Editor);

  XtFree(Entry.Name);
  XtFree(Entry.WorkPhone);
  XtFree(Entry.HomePhone);
  XtFree(Entry.Company);
  XtFree(Entry.FaxNumber);
  XtFree(Entry.EmailAddress);
  XtFree(Entry.WorkAddress);
  XtFree(Entry.HomeAddress);
  XtFree(Entry.DateUpdated);
  XtFree(Entry.Remarks);
}


void  Help_Line(Widget   w,
		char     *Key)
{
  XmString LabelString;
  char LabelBuffer[MY_BUFSIZ];
  int lcv;

/*   fprintf(stderr,"Help: %s\n",Key); */
  lcv = 0;
  while (Help_List[lcv].Key != NULL)
    {
      if (strcmp(Help_List[lcv].Key,Key) == 0)
	{
	  sprintf(LabelBuffer,"Help: %s",Help_List[lcv].Value);
	  LabelString = XmStringCreate(LabelBuffer, XmFONTLIST_DEFAULT_TAG),
	  XtVaSetValues(W_Help_Label,
			XmNlabelString,LabelString,
			NULL,0);
	  XmStringFree(LabelString);
	  return;
	}
      lcv++;
    }
}

void  Search_FromMenu()

{
  XtSetSensitive(W_Update_Button,True);
  XtSetSensitive(W_Delete_Button,True);
  Search_Form();
}

void Show_BCard_FromMenu()
{
  Show_BCard(G_CurrentEntryInRoloList);
}

void Print_Usage(char *ProgramName)
{
  fprintf(stderr,"Usage: %s [options] [standard X options]\n\nOptions:\n",ProgramName);
  fprintf(stderr,"\t-help or -h     -- What you see here.\n");
  fprintf(stderr,"\t-convert        -- Convert old rolo addressbook file to XmAddressBook format.\n");
  fprintf(stderr,"\t-convert1.1 or -c1.1  Source_Filename Destination_Filename");
  fprintf(stderr,"\t                -- Convert XmAddressBook 1.0.X file to XmAddressBook 1.1 format.\n");
  fprintf(stderr,"\t-user or -u     -- Another user's %s file\n",
	  DEFAULT_ADDRESSBOOK_FILENAME);
  fprintf(stderr,"\t-version or -v  -- Print version of program.\n");
#if defined(CURSES)
  fprintf(stderr,"\t-curses or -c   -- Use stripped down curses interface.\n");
#endif
}

void Print_Version()
{
  fprintf(stderr,"%s\n",PROGRAM_DESCRIPTION);
}
