#include "main.h"

Widget Print_Popup;
Widget PrintTypePS_Toggle;
Widget PrintTypeText_Toggle;
Widget PrintFile_Text;
Widget PrintLpr_Text;
Widget PrintFile_Toggle;
Widget PrintLpr_Toggle;
Widget PrintFile_Label;
Widget PrintLpr_Label;
Widget PrintRangeStart_Text;
Widget PrintRangeEnd_Text;

void Print_Close(Widget   w,
		 Widget   PopupWidget, 
		 XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  G_PrintVisible = False;
}

void PS_PrintLine(FILE  *OutputFile,
		  int   FontWidth,
		  int   PageWidth,  /* minus margins */
		  char *String)
{
  int lcv = 0;
  int LineCounter = 0;
  int ReturnCounter = 0;
  int Open = False;

  if (strlen(String) == 0)
    {
      fprintf(OutputFile,"NewLine\n");
      return;
    }
  while (lcv != strlen(String))
    {
      if (LineCounter == 0)
	{
	  fprintf(OutputFile,"DataFont (");
	  LineCounter = 1;
	  Open = True;
	}
      else if (LineCounter > PageWidth) 
	{
	  fprintf(OutputFile,") show IndentNewLine\n");
	  LineCounter = 0;
	  Open = False;
	}
      else {
	if (String[lcv] == '\n')
	  {
	    ReturnCounter++;
	    if (ReturnCounter == 1)
	      {
		fprintf(OutputFile,") show IndentNewLine\n");
		LineCounter = 0;
		Open = False;
	      }
	    lcv++;
	  }
	else {
	  fprintf(OutputFile,"%c",String[lcv]);
	  lcv++;
	  LineCounter += FontWidth;
	  ReturnCounter = 0;
	}
      }
    }
  if (Open == True)
    {
      fprintf(OutputFile,") show NewLine\n");
    }
}


void Lpr_Toggle_Callback(Widget   w,
			 Widget   PopupWidget, 
			 XmAnyCallbackStruct *call_value)
{
  XtSetSensitive(PrintLpr_Text,True);
  XtSetSensitive(PrintLpr_Label,True);
  XtSetSensitive(PrintFile_Label,False);
  XtSetSensitive(PrintFile_Text,False);
}

void File_Toggle_Callback(Widget   w,
			 Widget   PopupWidget, 
			 XmAnyCallbackStruct *call_value)
{
  XtSetSensitive(PrintFile_Text,True);
  XtSetSensitive(PrintFile_Label,True);
  XtSetSensitive(PrintLpr_Label,False);
  XtSetSensitive(PrintLpr_Text,False);
}

void Output_PostScript(FILE *PrintFile,
		      struct passwd *PasswdRec,
		      int Start,
		      int End)
{
  Rolo_Type *Temp;
  int Counter=0;
  int PageNumber = 1;
  int lcv;
    
  fprintf(PrintFile,"%%!\n%% Created By: %s\n",PROGRAM_TITLE);
  fprintf(PrintFile,"%%!\n%% Created On: %s\n",Get_Current_Date_Time("%X %x"));
  fprintf(PrintFile,"/TitleFont {/Helvetica findfont 10 scalefont setfont } def\n");
  fprintf(PrintFile,"/DataFont {/Courier findfont 10 scalefont setfont } def\n\n");
  
  fprintf(PrintFile,"/NewLine {/vpos vpos 10 sub def\n/hpos 72 def\nhpos vpos moveto} def\n");
  fprintf(PrintFile,"/IndentNewLine {/vpos vpos 10 sub def\n/hpos 162 def\nhpos vpos moveto} def\n");
  fprintf(PrintFile,"/Tab    {/hpos hpos 90 add def\n hpos vpos moveto} def\n\n");

  fprintf(PrintFile,"72 730 moveto TitleFont (%s's address book created on %s, Page %d) show \n",
	  PasswdRec->pw_gecos,Get_Current_Date_Time("%x at %X"),PageNumber);
  fprintf(PrintFile,"/hpos 72 def\n");
  fprintf(PrintFile,"/vpos 710 def\n\n");

  fprintf(PrintFile,"hpos vpos moveto\n");
  Temp = G_RoloList;
  lcv = 1;
  Counter=1;
  while (Temp != NULL)
    {
/*       fprintf(stderr,"Counter %d Start %d End %d\n",Counter,Start,End); */
      if ((Counter >= Start) && (Counter <= End))
	{
/* 	  fprintf(stderr,"INSIDE Counter %d Start %d End %d\n",Counter,Start,End); */
	  if (lcv <= 5)
	    {
	      fprintf(PrintFile,"TitleFont (NAME)         show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->Name);
	      fprintf(PrintFile,"TitleFont (WORK PHONE)   show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->WorkPhone);
	      fprintf(PrintFile,"TitleFont (HOME PHONE)   show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->HomePhone);
	      fprintf(PrintFile,"TitleFont (FAX NUMBER)   show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->FaxNumber);
	      fprintf(PrintFile,"TitleFont (COMPANY)      show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->Company);
	      fprintf(PrintFile,"TitleFont (EMAIL ADDRESS) show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->EmailAddress);
	      fprintf(PrintFile,"TitleFont (WORK ADDRESS) show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->WorkAddress);
	      fprintf(PrintFile,"TitleFont (HOME ADDRESS) show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->HomeAddress);
	      fprintf(PrintFile,"TitleFont (DATE UPDATED) show Tab DataFont (%s) show NewLine\n",
		      Temp->DateUpdated);
	      fprintf(PrintFile,"TitleFont (REMARKS) show Tab \n");
	      PS_PrintLine(PrintFile,10,620,Temp->Remarks);
	      fprintf(PrintFile,"NewLine\n");
	      fflush(PrintFile);
	    }
	  else {
	    PageNumber++;
	    fprintf(PrintFile,"showpage\n");
	    fprintf(PrintFile,"72 730 moveto TitleFont (%s's address book created on %s, Page %d) show \n",
		    PasswdRec->pw_gecos,Get_Current_Date_Time("%x at %X"),PageNumber);
	    fprintf(PrintFile,"/hpos 72 def\n");
	    fprintf(PrintFile,"/vpos 710 def\n\n");
	    fprintf(PrintFile,"hpos vpos moveto\n");
	    fflush(PrintFile);
	    lcv=0;
	  }
	  lcv++;
	}
	      
      if (lcv <= 5)
	{
	  Temp = Temp->Next;
	  Counter++;
	}
    }
  fprintf(PrintFile,"showpage\n");
}

void Output_Text(FILE *PrintFile,
		int Start,
		int End)
{
  Rolo_Type *Temp;
  int lcv, counter;
    
  Temp = G_RoloList;
  counter = 1;
  while (Temp != NULL)
    {
      if ((counter >= Start) && (counter <= End))
	{
	  fprintf(PrintFile,"         NAME: %s\n",Temp->Name);
	  fprintf(PrintFile,"   WORK PHONE: %s\n",Temp->WorkPhone);
	  fprintf(PrintFile,"   HOME PHONE: %s\n",Temp->HomePhone);
	  fprintf(PrintFile,"   FAX NUMBER: %s\n",Temp->FaxNumber);
	  fprintf(PrintFile,"      COMPANY: %s\n",Temp->Company);
	  fprintf(PrintFile,"EMAIL ADDRESS: %s\n",Temp->EmailAddress);
	  fprintf(PrintFile," WORK ADDRESS: %s\n",Temp->WorkAddress);
	  fprintf(PrintFile," HOME ADDRESS: %s\n",Temp->HomeAddress);
	  fprintf(PrintFile," DATE UPDATED: %s\n",Temp->DateUpdated);
	  fprintf(PrintFile,"      REMARKS: ");
	  fflush(PrintFile);
	  for (lcv=0;lcv<strlen(Temp->Remarks);lcv++)
	    {
	      if (Temp->Remarks[lcv] != '\n')
		{
		  fputc(Temp->Remarks[lcv],PrintFile);
		}
	      else {
		fputc(' ',PrintFile);
	      }
	    }
	  fputc('\n',PrintFile);
	  fflush(PrintFile);
	}
      Temp = Temp->Next;
      counter++;
    }
}

void Print_Confirm(Widget   w,
		   Widget   PopupWidget, 
		   XmAnyCallbackStruct *call_value)
{
  FILE *PrintFile;
  char *PrintStr = (char *)NULL;
  struct passwd *PasswdRec;
  int Start,End;
  
  Start = atoi(XmTextFieldGetString(PrintRangeStart_Text));
  End = atoi(XmTextFieldGetString(PrintRangeEnd_Text));
  
  if (End < Start)
    {
      ErrorMessage("Range Error:\n\nEnd range is less than start range.\n");
      return;
    }

  if (G_RoloList == NULL)
    {
      InfoMessage("\nAddress Book empty, nothing to print.\n");
      XtFree(PrintStr);
      return;
    }
  if ((PasswdRec = getpwuid(getuid())) == NULL)
    {
      perror("getpwuid");
      return;
    }
  if (XmToggleButtonGetState(PrintLpr_Toggle) == True)
    {
      PrintStr = XmTextFieldGetString(PrintLpr_Text);
      if ((PrintFile = popen(PrintStr,"w")) == NULL)
	{
	  ErrorMessage("File Error:\n\nError opening pipe to print command.\n");
	  perror(PrintStr);
	  return;
	}
    }
    else {
      PrintStr = XmTextFieldGetString(PrintFile_Text);
      if ((PrintFile = fopen(PrintStr,"w")) == NULL)
	{
	  ErrorMessage("File Error:\n\nError opening output file for writing.\n");
	  perror(PrintStr);
	  return;
	}
    }
  
  if (XmToggleButtonGetState(PrintTypePS_Toggle) == True)
    {
      Output_PostScript(PrintFile,PasswdRec,Start,End);
    }
  else {
    Output_Text(PrintFile,Start,End);
  }
  
  XtFree(PrintStr);
  if (XmToggleButtonGetState(PrintLpr_Toggle) == True)
    {
      pclose(PrintFile);
    }
  else {
      fclose(PrintFile);
  }
  XtUnrealizeWidget(PopupWidget);
  G_PrintVisible = False;
}



void Print_Reset(Widget   w,
		  Widget   TextWidget, 
		  XmAnyCallbackStruct *call_value)
{
  char Buffer[MY_BUFSIZ+1];

  XmTextFieldSetString(PrintLpr_Text,DEFAULT_PRINT_COMMAND);
  XmTextFieldSetString(PrintFile_Text,DEFAULT_PRINT_FILENAME);
  XmToggleButtonSetState(PrintTypePS_Toggle,True,True);
  XmToggleButtonSetState(PrintLpr_Toggle,True,True);
  XtSetSensitive(PrintLpr_Text,True);
  XtSetSensitive(PrintLpr_Label,True);
  XtSetSensitive(PrintFile_Label,False);
  XtSetSensitive(PrintFile_Text,False);

  sprintf(Buffer,"1");
  XmTextFieldSetString(PrintRangeStart_Text,Buffer);
  
  sprintf(Buffer,"%d",G_NumInRoloList);
  XmTextFieldSetString(PrintRangeEnd_Text,Buffer);
}



void  Print_AddressBook(Widget   w,
			Widget   textwidget,   
			XmAnyCallbackStruct *call_value)
{
  Widget Print_Frame;
  Widget Print_Form;
  Widget PrintType_RC;
  Widget Print_RC;
  Widget Print_Separator;
  Widget PrintConfirm_Button;
  Widget PrintReset_Button;
  Widget PrintClose_Button;
  Widget PrintRangeStart_Label;
  Widget PrintRangeEnd_Label;
  XmString LabelString;
  Atom WM_DELETE_WINDOW;
  char Buffer[MY_BUFSIZ+1];
  Position X,Y,W,H;
  

  if (G_PrintVisible == True)
    {
      XRaiseWindow(XtDisplay(Print_Popup),XtWindow(Print_Popup));
      XMapWindow(XtDisplay(Print_Popup),XtWindow(Print_Popup));
      return;
    }

  XtVaGetValues(W_TopLevel,
		XmNx,&X,
		XmNy,&Y,
		XmNwidth,&W,
		XmNheight,&H,
		NULL,0);

  Print_Popup = XtVaCreatePopupShell("Print",
				     xmDialogShellWidgetClass,
				     W_TopLevel,
				     XmNtitle, "Print",
				     XmNdeleteResponse, XmDO_NOTHING,
				     XmNx,X+(W/5),
				     XmNy,Y+(H/5),
				     NULL,0); 

  
/*  Print_Popup = XtVaAppCreateShell("Print_Popup",
				   "Print_Popup",
				   applicationShellWidgetClass,
				   XtDisplay(W_TopLevel),
				   XmNtitle, "Print",
				   XmNdeleteResponse, XmDO_NOTHING,
				   NULL,0); 
  */

  Print_Frame = XtVaCreateWidget("Print_Frame",
				 xmFrameWidgetClass, 
				 Print_Popup,
				 NULL,0);
  
  Print_Form = XtVaCreateManagedWidget("Print_Form",
				       xmFormWidgetClass, 
				       Print_Frame,
				       NULL,0);
  
  LabelString = XmStringCreate("Range Start: ", XmSTRING_OS_CHARSET),
  PrintRangeStart_Label = XtVaCreateManagedWidget("PrintRangeStart_Label",
						  xmLabelWidgetClass, 
						  Print_Form,
						  XmNlabelString, LabelString,
						  XmNleftAttachment, XmATTACH_POSITION,
						  XmNleftPosition, 0,
						  XmNrightAttachment, XmATTACH_POSITION,
						  XmNrightPosition, 25,
						  XmNtopAttachment, XmATTACH_FORM,
						  XmNtopOffset, 15,
						  XmNalignment, XmALIGNMENT_END,
						  NULL,0);
  XmStringFree(LabelString);
  PrintRangeStart_Text = XtVaCreateManagedWidget("PrintRangeStart_Text",
						 xmTextFieldWidgetClass, 
						 Print_Form,
						 XmNcolumns, 6,
						 XmNleftAttachment, XmATTACH_POSITION,
						 XmNleftPosition, 25,
						 XmNrightAttachment, XmATTACH_POSITION,
						 XmNrightPosition, 50,
						 XmNtopAttachment, XmATTACH_FORM,
						 XmNtopOffset, 10,
						 NULL,0);

  LabelString = XmStringCreate("Range End: ", XmSTRING_OS_CHARSET),
  PrintRangeEnd_Label = XtVaCreateManagedWidget("PrintRangeEnd_Label",
						xmLabelWidgetClass, 
						Print_Form,
						XmNlabelString, LabelString,
						XmNleftAttachment, XmATTACH_POSITION,
						XmNleftPosition, 50,
						XmNrightAttachment, XmATTACH_POSITION,
						XmNrightPosition, 75,
						XmNtopAttachment, XmATTACH_FORM,
						XmNtopOffset, 15,
						XmNalignment, XmALIGNMENT_END,
						NULL,0);
  XmStringFree(LabelString);
  PrintRangeEnd_Text = XtVaCreateManagedWidget("PrintRangeEnd_Text",
					       xmTextFieldWidgetClass, 
					       Print_Form,
					       XmNcolumns, 6,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition, 75,
					       XmNrightAttachment, XmATTACH_POSITION,
					       XmNrightPosition, 100,
					       XmNrightOffset, 10,
					       XmNtopAttachment, XmATTACH_FORM,
					       XmNtopOffset, 10,
					       NULL,0);
  
  sprintf(Buffer,"1");
  XmTextFieldSetString(PrintRangeStart_Text,Buffer);
  
  sprintf(Buffer,"%d",G_NumInRoloList);
  XmTextFieldSetString(PrintRangeEnd_Text,Buffer);

  PrintType_RC = XtVaCreateManagedWidget("PrintType_RC",
					 xmRowColumnWidgetClass, 
					 Print_Form,
					 XmNleftAttachment, XmATTACH_FORM,
					 XmNleftOffset, 10,
					 XmNrightAttachment, XmATTACH_FORM,
					 XmNrightOffset, 10,
					 XmNtopAttachment, XmATTACH_WIDGET,
					 XmNtopOffset, 10,
					 XmNtopWidget, PrintRangeStart_Text,
					 XmNentryAlignment, XmALIGNMENT_END,
					 XmNorientation, XmHORIZONTAL,
					 XmNpacking, XmPACK_TIGHT,
					 XmNradioBehavior, True,
					 NULL,0);
  
  LabelString = XmStringCreate("Postscript Output", XmSTRING_OS_CHARSET),
  PrintTypePS_Toggle = XtVaCreateManagedWidget("PrintTypePS_Toggle",
					       xmToggleButtonWidgetClass, 
					       PrintType_RC,
					       XmNlabelString, LabelString,
					       NULL,0);
  XmStringFree(LabelString);
  XmToggleButtonSetState(PrintTypePS_Toggle,True,True);
  
  LabelString = XmStringCreate("Text Output", XmSTRING_OS_CHARSET),
  PrintTypeText_Toggle = XtVaCreateManagedWidget("PrintTypeText_Toggle",
						 xmToggleButtonWidgetClass, 
						 PrintType_RC,
						 XmNlabelString, LabelString,
						 NULL,0);
  XmStringFree(LabelString);

  Print_RC = XtVaCreateManagedWidget("Print_RC",
				     xmRowColumnWidgetClass, 
				     Print_Form,
				     XmNleftAttachment, XmATTACH_FORM,
				     XmNleftOffset, 10,
				     XmNrightAttachment, XmATTACH_FORM,
				     XmNrightOffset, 10,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, PrintType_RC,
				     XmNentryAlignment, XmALIGNMENT_END,
				     XmNorientation, XmHORIZONTAL,
				     XmNpacking, XmPACK_TIGHT,
				     XmNradioBehavior, True,
				     NULL,0);
  
  LabelString = XmStringCreate("Output to a Printer", XmSTRING_OS_CHARSET),
  PrintLpr_Toggle = XtVaCreateManagedWidget("PrintLpr_Toggle",
					    xmToggleButtonWidgetClass, 
					    Print_RC,
					    XmNlabelString, LabelString,
					    NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(PrintLpr_Toggle, XmNarmCallback, 
                (XtCallbackProc) Lpr_Toggle_Callback, Print_Popup);
  XmToggleButtonSetState(PrintLpr_Toggle,True,True);

  LabelString = XmStringCreate("Output to a File", XmSTRING_OS_CHARSET),
  PrintFile_Toggle = XtVaCreateManagedWidget("PrintFile_Toggle",
					     xmToggleButtonWidgetClass, 
					     Print_RC,
					     XmNlabelString, LabelString,
					     NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(PrintFile_Toggle, XmNarmCallback, 
                (XtCallbackProc) File_Toggle_Callback, Print_Popup);

  LabelString = XmStringCreate("Print Command: ", XmSTRING_OS_CHARSET),
  PrintLpr_Label = XtVaCreateManagedWidget("PrintLpr_Label",
					   xmLabelWidgetClass, 
					   Print_Form,
					   XmNlabelString, LabelString,
					   XmNleftAttachment, XmATTACH_FORM,
					   XmNleftOffset, 10,
					   XmNtopAttachment, XmATTACH_WIDGET,
					   XmNtopOffset, 15,
					   XmNtopWidget, Print_RC,
					   NULL,0);
  XmStringFree(LabelString);
  
  PrintLpr_Text = XtVaCreateManagedWidget("PrintLpr_Text",
					  xmTextFieldWidgetClass, 
					  Print_Form,
					  XmNcolumns, 40,
					  XmNleftAttachment, XmATTACH_WIDGET,
					  XmNleftOffset, 10,
					  XmNleftWidget, PrintLpr_Label,
					  XmNrightAttachment, XmATTACH_FORM,
					  XmNrightOffset, 10,
					  XmNtopAttachment, XmATTACH_WIDGET,
					  XmNtopOffset, 10,
					  XmNtopWidget, Print_RC,
					  NULL,0);
  XtAddCallback(PrintLpr_Text, XmNactivateCallback, 
                (XtCallbackProc) Print_Confirm, Print_Popup);
  XmTextFieldSetString(PrintLpr_Text,DEFAULT_PRINT_COMMAND);
  

  LabelString = XmStringCreate("Filename: ", XmSTRING_OS_CHARSET),
  PrintFile_Label = XtVaCreateManagedWidget("PrintFile_Label",
					    xmLabelWidgetClass, 
					    Print_Form,
					    XmNlabelString, LabelString,
					    XmNleftAttachment, XmATTACH_FORM,
					    XmNleftOffset, 10,
					    XmNtopAttachment, XmATTACH_WIDGET,
					    XmNtopOffset, 15,
					    XmNtopWidget, PrintLpr_Text,
					    NULL,0);
  XmStringFree(LabelString);
  XtSetSensitive(PrintFile_Label,False);
  
  PrintFile_Text = XtVaCreateManagedWidget("PrintFile_Text",
					   xmTextFieldWidgetClass, 
					   Print_Form,
					   XmNcolumns, 40,
					   XmNleftAttachment, XmATTACH_WIDGET,
					   XmNleftOffset, 10,
					   XmNleftWidget, PrintFile_Label,
					   XmNrightAttachment, XmATTACH_FORM,
					   XmNrightOffset, 10,
					   XmNtopAttachment, XmATTACH_WIDGET,
					   XmNtopOffset, 10,
					   XmNtopWidget, PrintLpr_Text,
					   NULL,0);
  XtAddCallback(PrintFile_Text, XmNactivateCallback, 
                (XtCallbackProc) Print_Confirm, Print_Popup);
  XmTextFieldSetString(PrintFile_Text,DEFAULT_PRINT_FILENAME);
  XtSetSensitive(PrintFile_Text,False);

  Print_Separator = XtVaCreateManagedWidget("Print_Separator",
					    xmSeparatorWidgetClass,
					    Print_Form,
					    XmNtopAttachment, XmATTACH_WIDGET, 
					    XmNtopWidget, PrintFile_Text,
					    XmNtopOffset, 10,
					    XmNleftAttachment, XmATTACH_FORM,
					    XmNrightAttachment, XmATTACH_FORM,
					    NULL,0);
  

  LabelString = XmStringCreate("Print", XmSTRING_OS_CHARSET),
  PrintConfirm_Button = XtVaCreateManagedWidget("PrintConfirm_Button",
						xmPushButtonWidgetClass,
						Print_Form,
						XmNlabelString, LabelString,
						XmNtopAttachment, XmATTACH_WIDGET, 
						XmNbottomAttachment, XmATTACH_FORM,
						XmNtopWidget, Print_Separator,
						XmNtopOffset, 10,
						XmNbottomOffset, 10,
						XmNleftAttachment, XmATTACH_POSITION,
						XmNleftPosition,25,
						NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(PrintConfirm_Button, XmNactivateCallback, 
                (XtCallbackProc) Print_Confirm, Print_Popup);
  
  
  LabelString = XmStringCreate("Reset", XmSTRING_OS_CHARSET),
  PrintReset_Button = XtVaCreateManagedWidget("PrintReset_Button",
					      xmPushButtonWidgetClass,
					      Print_Form,
					      XmNlabelString, LabelString,
					      XmNtopAttachment, XmATTACH_WIDGET, 
					      XmNbottomAttachment, XmATTACH_FORM,
					      XmNtopWidget, Print_Separator,
					      XmNtopOffset, 10,
					      XmNbottomOffset, 10,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition,50,
					      NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(PrintReset_Button, XmNactivateCallback, 
                (XtCallbackProc) Print_Reset, NULL);
  
  
  LabelString = XmStringCreate("Cancel", XmSTRING_OS_CHARSET),
  PrintClose_Button = XtVaCreateManagedWidget("PrintClose_Button",
					      xmPushButtonWidgetClass,
					      Print_Form,
					      XmNlabelString, LabelString,
					      XmNtopAttachment, XmATTACH_WIDGET, 
					      XmNbottomAttachment, XmATTACH_FORM,
					      XmNtopWidget, Print_Separator,
					      XmNtopOffset, 10,
					      XmNbottomOffset, 10,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition,75,
					      NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(PrintClose_Button, XmNactivateCallback, 
                (XtCallbackProc) Print_Close, Print_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(Print_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(Print_Popup,WM_DELETE_WINDOW,
			  (XtCallbackProc)Print_Close,Print_Popup);

  XtManageChild(Print_Frame);
  G_PrintVisible = True;
  XtPopup(Print_Popup,XtGrabExclusive);
/*   XtRealizeWidget(Print_Popup); */
}

/*
      fprintf(PrintFile,"NAME:         %s\tNUM: %d\n",Temp->Name,Counter);
      fprintf(PrintFile,"WORK PHONE:   %s\t",Temp->WorkPhone);
      fprintf(PrintFile,"HOME PHONE:   %s\n",Temp->HomePhone);
      fprintf(PrintFile,"COMPANY:      %s\n",Temp->Company);
      fprintf(PrintFile,"WORK ADDRESS: %s\n",Temp->WorkAddress);
      fprintf(PrintFile,"HOME ADDRESS: %s\n",Temp->HomeAddress);
      fprintf(PrintFile,"DATE UPDATED: %s\n",Temp->DateUpdated);
      fprintf(PrintFile,"REMARKS: \n%s\n",Temp->Remarks);
      fprintf(PrintFile,"-----------------------------------------------------------------------------\n");
*/
