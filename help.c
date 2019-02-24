#include "main.h"

void DisplayHelp_Close(Widget   w,
		       Widget   PopupWidget, 
		       XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
}



void  Display_Help(Widget   w,
		   Widget   textwidget,   
		   XmAnyCallbackStruct *call_value)
{
  Widget DisplayHelp_Popup;
  Widget DisplayHelp_Frame;
  Widget DisplayHelp_Form;
  Widget DisplayHelp_Text;
  Widget DisplayHelp_Separator;
  Widget DisplayHelpClose_Button;
  XmString LabelString;
  Atom WM_DELETE_WINDOW;
  
  
  DisplayHelp_Popup = XtVaAppCreateShell("DisplayHelp_Popup",
					 "DisplayHelp_Popup",
					 applicationShellWidgetClass,
					 XtDisplay(W_TopLevel),
					 XmNtitle, "Help",
					 XmNdeleteResponse, XmDO_NOTHING,
					 NULL,0); 
  
  DisplayHelp_Frame = XtVaCreateManagedWidget("DisplayHelp_Frame",
					      xmFrameWidgetClass, 
					      DisplayHelp_Popup,
					      NULL,0);
  
  DisplayHelp_Form = XtVaCreateManagedWidget("DisplayHelp_Form",
					     xmFormWidgetClass, 
					     DisplayHelp_Frame,
					     NULL,0);

  DisplayHelp_Text = XmCreateScrolledText(DisplayHelp_Form,
					  "DisplayHelp_Text",
					  NULL,
					  0);
  XtVaSetValues(XtParent(DisplayHelp_Text),
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNtopAttachment, XmATTACH_POSITION,
		XmNtopPosition, 0,
		XmNbottomAttachment, XmATTACH_POSITION,
		XmNbottomPosition, 85,
		NULL,0);

  XtVaSetValues(DisplayHelp_Text,
		XmNrows, 20,
		XmNcolumns, 80,
		XmNeditable, False,
		XmNeditMode, XmMULTI_LINE_EDIT,
		XmNcursorPositionVisible, False,
		NULL,0);

  if (G_HelpString == NULL)
    {
      XmTextSetString(DisplayHelp_Text,"Sorry no help available.");
    } 
  else {
/*     fprintf(stderr,"*%s*#%d#\n",G_HelpString,strlen(G_HelpString));  */
    XmTextSetString(DisplayHelp_Text,G_HelpString);
  }
  

  XtManageChild(DisplayHelp_Text);
  
  DisplayHelp_Separator = XtVaCreateManagedWidget("DisplayHelp_Separator",
						  xmSeparatorWidgetClass,
						  DisplayHelp_Form,
						  XmNleftAttachment, XmATTACH_FORM,
						  XmNrightAttachment, XmATTACH_FORM,
						  XmNtopAttachment, XmATTACH_POSITION,
						  XmNtopPosition, 85,
						  XmNbottomAttachment, XmATTACH_POSITION,
						  XmNbottomPosition, 90,
						  NULL,0);
  
  LabelString = XmStringCreate("Close", XmFONTLIST_DEFAULT_TAG),
  DisplayHelpClose_Button = XtVaCreateManagedWidget("DisplayHelpClose_Button",
						    xmPushButtonWidgetClass,
						    DisplayHelp_Form,
						    XmNlabelString, LabelString,
						    XmNtopAttachment, XmATTACH_POSITION,
						    XmNtopPosition, 90,
						    XmNbottomAttachment, XmATTACH_POSITION,
						    XmNbottomPosition, 95,
						    XmNleftAttachment, XmATTACH_POSITION,
						    XmNleftPosition,45,
						    NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(DisplayHelpClose_Button, XmNactivateCallback, 
                (XtCallbackProc) DisplayHelp_Close, DisplayHelp_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(DisplayHelp_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(DisplayHelp_Popup,WM_DELETE_WINDOW,
			  (XtCallbackProc)DisplayHelp_Close,DisplayHelp_Popup);

  XtRealizeWidget(DisplayHelp_Popup);
}

void DisplayVersion_Close(Widget   w,
			    Widget   PopupWidget, 
			    XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
}

void  Display_Version(Widget   w,
		      Widget   textwidget,   
		      XmAnyCallbackStruct *call_value)
{
  Widget DisplayVersion_Popup;
  Arg args[4];

  XtSetArg(args[0], XmNautoUnmanage, False);
					 XmNtitle, "Help",

  XtSetArg(args[1], XmNmessageString, XmStringLtoRCreate(PROGRAM_DESCRIPTION,
							 XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[2], XmNokLabelString, XmStringCreate("Close",XmFONTLIST_DEFAULT_TAG));
  XtSetArg(args[3], XmNtitle, "Version");
  DisplayVersion_Popup = (Widget )XmCreateMessageDialog(W_TopLevel, "DisplayVersion_Dialog", args, 4);
  
  XtUnmanageChild((Widget )XmMessageBoxGetChild(DisplayVersion_Popup, XmDIALOG_HELP_BUTTON));
  XtUnmanageChild((Widget )XmMessageBoxGetChild(DisplayVersion_Popup, XmDIALOG_CANCEL_BUTTON));
  XtVaSetValues((Widget )XmMessageBoxGetChild(DisplayVersion_Popup, XmDIALOG_OK_BUTTON),
		NULL,0);
  XtVaSetValues((Widget )XmMessageBoxGetChild(DisplayVersion_Popup, XmDIALOG_MESSAGE_LABEL),
		XmNalignment, XmALIGNMENT_CENTER,
		NULL,0); 
  XtAddCallback(DisplayVersion_Popup, XmNokCallback,  (XtCallbackProc) DisplayVersion_Close,
		DisplayVersion_Popup);
  
  XtManageChild(DisplayVersion_Popup);
  XWarpPointer(XtDisplay(DisplayVersion_Popup),None,XtWindow(DisplayVersion_Popup),0,0,0,0,1,1); 
  /*  XDefineCursor(XtDisplay(DisplayVersion_Popup),XtWindow(DisplayVersion_Popup),QuestionCursor); */
  XFlush(XtDisplay(DisplayVersion_Popup));

  XtPopup(XtParent(DisplayVersion_Popup), XtGrabNone);
}

