#include "main.h"

void ExitProgram_Cancel(Widget   w,
			Widget   PopupWidget, 
			XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
}

void ExitProgram_Confirm(Widget   w,
			 Widget   PopupWidget, 
			 XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  exit (0);
}

void  Exit_Program(Widget   w,
		   Widget   textwidget,   
		   XmAnyCallbackStruct *call_value)
{
  Widget ExitProgram_Popup;
  Arg args[5];

  if (G_ChangedNotSaved == False)
    {
      exit (0);
    }
  XtSetArg(args[0], XmNautoUnmanage, False);
  XtSetArg(args[1], XmNmessageString, XmStringLtoRCreate("Exit program w/o saving?",
                                                         XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[2], XmNokLabelString, XmStringCreate("Confirm",XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[3], XmNcancelLabelString, XmStringCreate("Cancel",XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[4], XmNtitle, "Exit Program");
  ExitProgram_Popup = (Widget )XmCreateMessageDialog(W_TopLevel, "ExitProgram_Dialog",
							args, 5);
  
  XtUnmanageChild((Widget )XmMessageBoxGetChild(ExitProgram_Popup, XmDIALOG_HELP_BUTTON));
  XtVaSetValues((Widget )XmMessageBoxGetChild(ExitProgram_Popup, XmDIALOG_OK_BUTTON),
                NULL,0);
  XtVaSetValues((Widget )XmMessageBoxGetChild(ExitProgram_Popup, XmDIALOG_CANCEL_BUTTON),
                NULL,0);
/*   XtVaSetValues((Widget )XmMessageBoxGetChild(ExitProgram_Popup, XmDIALOG_MESSAGE_LABEL),
                XmNfontList, G_BigFontList, 
                NULL,0); */
  XtAddCallback(ExitProgram_Popup, XmNokCallback,  (XtCallbackProc) ExitProgram_Confirm,
                ExitProgram_Popup);
  XtAddCallback(ExitProgram_Popup, XmNcancelCallback,  (XtCallbackProc) ExitProgram_Cancel,
                ExitProgram_Popup);
  
  XtManageChild(ExitProgram_Popup);
  XWarpPointer(XtDisplay(ExitProgram_Popup),None,XtWindow(ExitProgram_Popup),0,0,0,0,1,1); 
  /*  XDefineCursor(XtDisplay(ExitProgram_Popup),XtWindow(ExitProgram_Popup),QuestionCursor); */
  XFlush(XtDisplay(ExitProgram_Popup));
  XtPopup(XtParent(ExitProgram_Popup), XtGrabNone);
}

void SaveAndExit_Cancel(Widget   w,
			Widget   PopupWidget, 
			XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
}

void SaveAndExit_Confirm(Widget   w,
			 Widget   PopupWidget, 
			 XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  Write_AddressBook(G_Addressbook_Filename);
  exit (0);
}

void SaveAndExit(Widget   w,
		 Widget   textwidget,   
		 XmAnyCallbackStruct *call_value)
{
  Widget SaveAndExit_Popup;
  Arg args[5];

  if (G_ChangedNotSaved == False)
    {
      exit(0);
    }
  XtSetArg(args[0], XmNautoUnmanage, False);
  XtSetArg(args[1], XmNmessageString, XmStringLtoRCreate("Save and Exit Program?",
                                                         XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[2], XmNokLabelString, XmStringCreate("Confirm",XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[3], XmNcancelLabelString, XmStringCreate("Cancel",XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[4], XmNtitle, "Save and Exit Program");
  SaveAndExit_Popup = (Widget )XmCreateMessageDialog(W_TopLevel, "SaveAndExit_Dialog",
							args, 5);
  
  XtUnmanageChild((Widget )XmMessageBoxGetChild(SaveAndExit_Popup, XmDIALOG_HELP_BUTTON));
  XtVaSetValues((Widget )XmMessageBoxGetChild(SaveAndExit_Popup, XmDIALOG_OK_BUTTON),
                NULL,0);
  XtVaSetValues((Widget )XmMessageBoxGetChild(SaveAndExit_Popup, XmDIALOG_CANCEL_BUTTON),
                NULL,0);
/*   XtVaSetValues((Widget )XmMessageBoxGetChild(SaveAndExit_Popup, XmDIALOG_MESSAGE_LABEL),
                XmNfontList, G_BigFontList, 
                NULL,0); */
  XtAddCallback(SaveAndExit_Popup, XmNokCallback,  (XtCallbackProc) SaveAndExit_Confirm,
                SaveAndExit_Popup);
  XtAddCallback(SaveAndExit_Popup, XmNcancelCallback,  (XtCallbackProc) SaveAndExit_Cancel,
                SaveAndExit_Popup);
  
  XtManageChild(SaveAndExit_Popup);
  XWarpPointer(XtDisplay(SaveAndExit_Popup),None,XtWindow(SaveAndExit_Popup),0,0,0,0,1,1); 
  /*  XDefineCursor(XtDisplay(SaveAndExit_Popup),XtWindow(SaveAndExit_Popup),QuestionCursor); */
  XFlush(XtDisplay(SaveAndExit_Popup));
  XtPopup(XtParent(SaveAndExit_Popup), XtGrabNone);
}


void SaveDontExit_Cancel(Widget   w,
			Widget   PopupWidget, 
			XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  if (G_OpenAddressbook == True)
    {
      Open_AddressBook_Dialog();
    }
}

void SaveDontExit_Confirm(Widget   w,
			 Widget   PopupWidget, 
			 XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  Write_AddressBook(G_Addressbook_Filename);
  if (G_OpenAddressbook == True)
    {
      Open_AddressBook_Dialog();
    }
}

void SaveDontExit(Widget   w,
		 Widget   textwidget,   
		 XmAnyCallbackStruct *call_value)
{
  Widget SaveDontExit_Popup;
  Arg args[5];

  XtSetArg(args[0], XmNautoUnmanage, False);
  XtSetArg(args[1], XmNmessageString, XmStringLtoRCreate("Save Current Address Book?",
                                                         XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[2], XmNokLabelString, XmStringCreate("Confirm",XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[3], XmNcancelLabelString, XmStringCreate("Cancel",XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[4], XmNtitle, "Save");
  SaveDontExit_Popup = (Widget )XmCreateMessageDialog(W_TopLevel, "SaveDontExit_Dialog",
							args, 5);
  
  XtUnmanageChild((Widget )XmMessageBoxGetChild(SaveDontExit_Popup, XmDIALOG_HELP_BUTTON));
  XtVaSetValues((Widget )XmMessageBoxGetChild(SaveDontExit_Popup, XmDIALOG_OK_BUTTON),
                NULL,0);
  XtVaSetValues((Widget )XmMessageBoxGetChild(SaveDontExit_Popup, XmDIALOG_CANCEL_BUTTON),
                NULL,0);
/*   XtVaSetValues((Widget )XmMessageBoxGetChild(SaveDontExit_Popup, XmDIALOG_MESSAGE_LABEL),
                XmNfontList, G_BigFontList, 
                NULL,0); */
  XtAddCallback(SaveDontExit_Popup, XmNokCallback,  (XtCallbackProc) SaveDontExit_Confirm,
                SaveDontExit_Popup);
  XtAddCallback(SaveDontExit_Popup, XmNcancelCallback,  (XtCallbackProc) SaveDontExit_Cancel,
                SaveDontExit_Popup);
  
  XtManageChild(SaveDontExit_Popup);
  XWarpPointer(XtDisplay(SaveDontExit_Popup),None,XtWindow(SaveDontExit_Popup),0,0,0,0,1,1); 
  /*  XDefineCursor(XtDisplay(SaveDontExit_Popup),XtWindow(SaveDontExit_Popup),QuestionCursor); */
  XFlush(XtDisplay(SaveDontExit_Popup));
  XtPopup(XtParent(SaveDontExit_Popup), XtGrabNone);
}

