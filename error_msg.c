#include "main.h"

void ErrorMsgHelp_Close(Widget   w,
		       Widget   PopupWidget, 
		       XmAnyCallbackStruct *call_value)
{
  XtPopdown(PopupWidget);
}


void  ErrorMessage(char *ErrorMessageString)
     
{
  Widget ErrorMessage_Dialog;
  XmString ErrorMessageXmString;
  XmString OkXmString;

  ErrorMessageXmString = XmStringLtoRCreate(ErrorMessageString, XmSTRING_OS_CHARSET);
  OkXmString = XmStringCreate("Close",XmSTRING_OS_CHARSET);

  ErrorMessage_Dialog = XmCreateErrorDialog(W_TopLevel,
					    "ErrorMessage",
					    NULL,0);
  XtVaSetValues(ErrorMessage_Dialog,
                XmNmessageString, ErrorMessageXmString,
		XmNokLabelString, OkXmString,
		NULL,0);

  XtVaSetValues(XtParent(ErrorMessage_Dialog),
		XmNtitle, "Error Message",
		NULL,0);

  XmStringFree(ErrorMessageXmString);
  XmStringFree(OkXmString);
  XtUnmanageChild((Widget )XmMessageBoxGetChild(ErrorMessage_Dialog, XmDIALOG_HELP_BUTTON));
  XtUnmanageChild((Widget )XmMessageBoxGetChild(ErrorMessage_Dialog, XmDIALOG_CANCEL_BUTTON));
  
  XtManageChild(ErrorMessage_Dialog);
  XWarpPointer(XtDisplay(ErrorMessage_Dialog),None,XtWindow(ErrorMessage_Dialog),0,0,0,0,1,1);
  XBell(XtDisplay(W_TopLevel),100); 
  XtPopup(XtParent(ErrorMessage_Dialog), XtGrabNone);
}


void InfoMsgHelp_Close(Widget   w,
		       Widget   PopupWidget, 
		       XmAnyCallbackStruct *call_value)
{
  XtPopdown(PopupWidget);
}



void  InfoMessage(char *InfoMessageString)
     
{
  Widget InfoMessage_Dialog;
  XmString InfoMessageXmString;
  XmString OkXmString;

  InfoMessageXmString = XmStringLtoRCreate(InfoMessageString, XmSTRING_OS_CHARSET);
  OkXmString = XmStringCreate("Close",XmSTRING_OS_CHARSET);

  InfoMessage_Dialog = XmCreateInformationDialog(W_TopLevel,
						 "InfoMessage",
						 NULL,0);
  XtVaSetValues(InfoMessage_Dialog,
                XmNmessageString, InfoMessageXmString,
		XmNokLabelString, OkXmString,
		NULL,0);

  XtVaSetValues(XtParent(InfoMessage_Dialog),
		XmNtitle, "Information Message",
		NULL,0);

  XmStringFree(InfoMessageXmString);
  XmStringFree(OkXmString);
  XtUnmanageChild((Widget )XmMessageBoxGetChild(InfoMessage_Dialog, XmDIALOG_HELP_BUTTON));
  XtUnmanageChild((Widget )XmMessageBoxGetChild(InfoMessage_Dialog, XmDIALOG_CANCEL_BUTTON));
  
  XtManageChild(InfoMessage_Dialog);
  XWarpPointer(XtDisplay(InfoMessage_Dialog),None,XtWindow(InfoMessage_Dialog),0,0,0,0,1,1);
  XBell(XtDisplay(W_TopLevel),100); 
  XtPopup(XtParent(InfoMessage_Dialog), XtGrabNone);
}

