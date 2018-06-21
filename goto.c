#include "main.h"

Widget Goto_Popup;

void Goto_Close(Widget   w,
		  Widget   PopupWidget, 
		  XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  G_GotoVisible = False;
}

void Goto_Confirm(Widget   w,
		       Widget   TextWidget, 
		       XmAnyCallbackStruct *call_value)
{
  char *GotoEntryStr = NULL;
  int GotoEntryNum = 0;
  
  if ((GotoEntryStr = XmTextFieldGetString(TextWidget)) == NULL)
    {
      return;
    }
  GotoEntryNum = atoi(GotoEntryStr);
  XtFree(GotoEntryStr);
  Goto_Actual_Entry(GotoEntryNum);
}

 
void Goto_Reset(Widget   w,
		Widget   TextWidget, 
		XmAnyCallbackStruct *call_value)
{
  XmTextFieldSetString(TextWidget,"");
}



void  Goto_Form()
{
  Widget Goto_Frame;
  Widget Goto_Form;
  Widget Goto_Label;
  Widget Goto_Text;
  Widget Goto_Separator;
  Widget GotoConfirm_Button;
  Widget GotoReset_Button;
  Widget GotoClose_Button;
  XmString LabelString;
  Atom WM_DELETE_WINDOW;
  Position X,Y,W,H;
  
  if (G_GotoVisible == True)
    {
      XRaiseWindow(XtDisplay(Goto_Popup),XtWindow(Goto_Popup));
      XMapWindow(XtDisplay(Goto_Popup),XtWindow(Goto_Popup));
      return;
    }

  XtVaGetValues(W_TopLevel,
		XmNx,&X,
		XmNy,&Y,
		XmNwidth,&W,
		XmNheight,&H,
		NULL,0);

  Goto_Popup = XtVaCreatePopupShell("Goto",
				    xmDialogShellWidgetClass,
				    W_TopLevel,
				    XmNtitle, "Goto",
				    XmNdeleteResponse, XmDO_NOTHING,
				    XmNx,X+(W/5),
				    XmNy,Y+(H/5),
				    NULL,0); 

  
  Goto_Frame = XtVaCreateWidget("Goto_Frame",
				xmFrameWidgetClass, 
				Goto_Popup,
				NULL,0);
  
  Goto_Form = XtVaCreateManagedWidget("Goto_Form",
				      xmFormWidgetClass, 
				      Goto_Frame,
				      NULL,0);
  
  LabelString = XmStringCreate("Goto Number: ", XmSTRING_OS_CHARSET),
  Goto_Label = XtVaCreateManagedWidget("Goto_Label",
				       xmLabelWidgetClass, 
				       Goto_Form,
				       XmNlabelString, LabelString,
				       XmNleftAttachment, XmATTACH_FORM,
				       XmNleftOffset, 10,
				       XmNtopAttachment, XmATTACH_FORM,
				       XmNtopOffset, 10,
				       NULL,0);
  XmStringFree(LabelString);

  Goto_Text = XtVaCreateManagedWidget("Goto_Text",
					xmTextFieldWidgetClass, 
					Goto_Form,
					XmNleftAttachment, XmATTACH_WIDGET,
					XmNleftWidget, Goto_Label,
					XmNrightAttachment, XmATTACH_FORM,
					XmNrightOffset, 10,
					XmNtopAttachment, XmATTACH_FORM,
					XmNtopOffset, 10,
					XmNcolumns, 30,
					NULL,0);
  XtAddCallback(Goto_Text, XmNactivateCallback, 
                (XtCallbackProc) Goto_Confirm, Goto_Text);
  

  Goto_Separator = XtVaCreateManagedWidget("Goto_Separator",
					     xmSeparatorWidgetClass,
					     Goto_Form,
					     XmNtopAttachment, XmATTACH_WIDGET, 
					     XmNtopWidget, Goto_Text,
					     XmNtopOffset, 10,
					     XmNleftAttachment, XmATTACH_FORM,
					     XmNrightAttachment, XmATTACH_FORM,
					     NULL,0);
  
  LabelString = XmStringCreate("Goto", XmSTRING_OS_CHARSET),
  GotoConfirm_Button = XtVaCreateManagedWidget("GotoConfirm_Button",
						 xmPushButtonWidgetClass,
						 Goto_Form,
						 XmNlabelString, LabelString,
						 XmNtopAttachment, XmATTACH_WIDGET, 
						 XmNbottomAttachment, XmATTACH_FORM,
						 XmNtopWidget, Goto_Separator,
						 XmNtopOffset, 10,
						 XmNbottomOffset, 10,
						 XmNleftAttachment, XmATTACH_POSITION,
						 XmNleftPosition,25,
						 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(GotoConfirm_Button, XmNactivateCallback, 
                (XtCallbackProc) Goto_Confirm, Goto_Text);
  

  LabelString = XmStringCreate("Reset", XmSTRING_OS_CHARSET),
  GotoReset_Button = XtVaCreateManagedWidget("GotoReset_Button",
					       xmPushButtonWidgetClass,
					       Goto_Form,
					       XmNlabelString, LabelString,
					       XmNtopAttachment, XmATTACH_WIDGET, 
					       XmNbottomAttachment, XmATTACH_FORM,
					       XmNtopWidget, Goto_Separator,
					       XmNtopOffset, 10,
					       XmNbottomOffset, 10,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition,50,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(GotoReset_Button, XmNactivateCallback, 
                (XtCallbackProc) Goto_Reset, Goto_Text);
  
  
  LabelString = XmStringCreate("Close", XmSTRING_OS_CHARSET),
  GotoClose_Button = XtVaCreateManagedWidget("GotoClose_Button",
					       xmPushButtonWidgetClass,
					       Goto_Form,
					       XmNlabelString, LabelString,
					       XmNtopAttachment, XmATTACH_WIDGET, 
					       XmNbottomAttachment, XmATTACH_FORM,
					       XmNtopWidget, Goto_Separator,
					       XmNtopOffset, 10,
					       XmNbottomOffset, 10,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition,75,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(GotoClose_Button, XmNactivateCallback, 
                (XtCallbackProc) Goto_Close, Goto_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(Goto_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(Goto_Popup,WM_DELETE_WINDOW,
			  (XtCallbackProc)Goto_Close,Goto_Popup);

  XtManageChild(Goto_Frame);
  XtPopup(Goto_Popup,XtGrabExclusive);
  G_GotoVisible = True;
}

