#include "main.h"

Widget Pref_Popup;
Widget PrefTypePS_Toggle;
Widget PrefTypeText_Toggle;
Widget PrefFile_Text;
Widget PrefLpr_Text;
Widget PrefFile_Toggle;
Widget PrefLpr_Toggle;
Widget PrefFile_Label;
Widget PrefLpr_Label;
Widget PrefRangeStart_Text;
Widget PrefRangeEnd_Text;

void Pref_Close(Widget   w,
		 Widget   PopupWidget, 
		 XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  G_PreferencesVisible = False;
}


void Pref_Confirm(Widget   w,
		   Widget   PopupWidget, 
		   XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  G_PreferencesVisible = False;
}



void Pref_Reset(Widget   w,
		  Widget   TextWidget, 
		  XmAnyCallbackStruct *call_value)
{
}



void  Show_Preferences(Widget   w,
		       Widget   textwidget,   
		       XmAnyCallbackStruct *call_value)
{
  Widget Pref_Frame;
  Widget Pref_Form;
  Widget PrefType_RC;
  Widget Pref_RC;
  Widget Pref_Separator;
  Widget PrefConfirm_Button;
  Widget PrefReset_Button;
  Widget PrefClose_Button;
  Widget PrefRangeStart_Label;
  Widget PrefRangeEnd_Label;
  XmString LabelString;
  Atom WM_DELETE_WINDOW;
  char Buffer[MY_BUFSIZ+1];
  Position X,Y,W,H;
  

  if (G_PreferencesVisible == True);
    {
      XRaiseWindow(XtDisplay(Pref_Popup),XtWindow(Pref_Popup));
      XMapWindow(XtDisplay(Pref_Popup),XtWindow(Pref_Popup));
      return;
    }

  XtVaGetValues(W_TopLevel,
		XmNx,&X,
		XmNy,&Y,
		XmNwidth,&W,
		XmNheight,&H,
		NULL,0);

  Pref_Popup = XtVaCreatePopupShell("Preferences",
				     xmDialogShellWidgetClass,
				     W_TopLevel,
				     XmNtitle, "Preferences",
				     XmNdeleteResponse, XmDO_NOTHING,
				     XmNx,X+(W/5),
				     XmNy,Y+(H/5),
				     NULL,0); 

  Pref_Frame = XtVaCreateWidget("Pref_Frame",
				 xmFrameWidgetClass, 
				 Pref_Popup,
				 NULL,0);
  
  Pref_Form = XtVaCreateManagedWidget("Pref_Form",
				       xmFormWidgetClass, 
				       Pref_Frame,
				       NULL,0);
  
  LabelString = XmStringCreate("Range Start: ", XmSTRING_OS_CHARSET),
  PrefRangeStart_Label = XtVaCreateManagedWidget("PrefRangeStart_Label",
						  xmLabelWidgetClass, 
						  Pref_Form,
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
  PrefRangeStart_Text = XtVaCreateManagedWidget("PrefRangeStart_Text",
						 xmTextFieldWidgetClass, 
						 Pref_Form,
						 XmNcolumns, 6,
						 XmNleftAttachment, XmATTACH_POSITION,
						 XmNleftPosition, 25,
						 XmNrightAttachment, XmATTACH_POSITION,
						 XmNrightPosition, 50,
						 XmNtopAttachment, XmATTACH_FORM,
						 XmNtopOffset, 10,
						 NULL,0);

  LabelString = XmStringCreate("Range End: ", XmSTRING_OS_CHARSET),
  PrefRangeEnd_Label = XtVaCreateManagedWidget("PrefRangeEnd_Label",
						xmLabelWidgetClass, 
						Pref_Form,
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
  PrefRangeEnd_Text = XtVaCreateManagedWidget("PrefRangeEnd_Text",
					       xmTextFieldWidgetClass, 
					       Pref_Form,
					       XmNcolumns, 6,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition, 75,
					       XmNrightAttachment, XmATTACH_POSITION,
					       XmNrightPosition, 100,
					       XmNrightOffset, 10,
					       XmNtopAttachment, XmATTACH_FORM,
					       XmNtopOffset, 10,
					       NULL,0);
  
  PrefType_RC = XtVaCreateManagedWidget("PrefType_RC",
					 xmRowColumnWidgetClass, 
					 Pref_Form,
					 XmNleftAttachment, XmATTACH_FORM,
					 XmNleftOffset, 10,
					 XmNrightAttachment, XmATTACH_FORM,
					 XmNrightOffset, 10,
					 XmNtopAttachment, XmATTACH_WIDGET,
					 XmNtopOffset, 10,
					 XmNtopWidget, PrefRangeStart_Text,
					 XmNentryAlignment, XmALIGNMENT_END,
					 XmNorientation, XmHORIZONTAL,
					 XmNpacking, XmPACK_TIGHT,
					 XmNradioBehavior, True,
					 NULL,0);
  
  LabelString = XmStringCreate("Postscript Output", XmSTRING_OS_CHARSET),
  PrefTypePS_Toggle = XtVaCreateManagedWidget("PrefTypePS_Toggle",
					       xmToggleButtonWidgetClass, 
					       PrefType_RC,
					       XmNlabelString, LabelString,
					       NULL,0);
  XmStringFree(LabelString);
  XmToggleButtonSetState(PrefTypePS_Toggle,True,True);
  
  LabelString = XmStringCreate("Text Output", XmSTRING_OS_CHARSET),
  PrefTypeText_Toggle = XtVaCreateManagedWidget("PrefTypeText_Toggle",
						 xmToggleButtonWidgetClass, 
						 PrefType_RC,
						 XmNlabelString, LabelString,
						 NULL,0);
  XmStringFree(LabelString);

  Pref_RC = XtVaCreateManagedWidget("Pref_RC",
				     xmRowColumnWidgetClass, 
				     Pref_Form,
				     XmNleftAttachment, XmATTACH_FORM,
				     XmNleftOffset, 10,
				     XmNrightAttachment, XmATTACH_FORM,
				     XmNrightOffset, 10,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, PrefType_RC,
				     XmNentryAlignment, XmALIGNMENT_END,
				     XmNorientation, XmHORIZONTAL,
				     XmNpacking, XmPACK_TIGHT,
				     XmNradioBehavior, True,
				     NULL,0);
  
  LabelString = XmStringCreate("Output to a Prefer", XmSTRING_OS_CHARSET),
  PrefLpr_Toggle = XtVaCreateManagedWidget("PrefLpr_Toggle",
					    xmToggleButtonWidgetClass, 
					    Pref_RC,
					    XmNlabelString, LabelString,
					    NULL,0);
  XmStringFree(LabelString);
/*  XtAddCallback(PrefLpr_Toggle, XmNarmCallback, 
                (XtCallbackProc) Lpr_Toggle_Callback, Pref_Popup);
  XmToggleButtonSetState(PrefLpr_Toggle,True,True);
*/

  LabelString = XmStringCreate("Output to a File", XmSTRING_OS_CHARSET),
  PrefFile_Toggle = XtVaCreateManagedWidget("PrefFile_Toggle",
					     xmToggleButtonWidgetClass, 
					     Pref_RC,
					     XmNlabelString, LabelString,
					     NULL,0);
  XmStringFree(LabelString);
/*  XtAddCallback(PrefFile_Toggle, XmNarmCallback, 
                (XtCallbackProc) File_Toggle_Callback, Pref_Popup);
*/
  LabelString = XmStringCreate("Pref Command: ", XmSTRING_OS_CHARSET),
  PrefLpr_Label = XtVaCreateManagedWidget("PrefLpr_Label",
					   xmLabelWidgetClass, 
					   Pref_Form,
					   XmNlabelString, LabelString,
					   XmNleftAttachment, XmATTACH_FORM,
					   XmNleftOffset, 10,
					   XmNtopAttachment, XmATTACH_WIDGET,
					   XmNtopOffset, 15,
					   XmNtopWidget, Pref_RC,
					   NULL,0);
  XmStringFree(LabelString);
  
  PrefLpr_Text = XtVaCreateManagedWidget("PrefLpr_Text",
					  xmTextFieldWidgetClass, 
					  Pref_Form,
					  XmNcolumns, 40,
					  XmNleftAttachment, XmATTACH_WIDGET,
					  XmNleftOffset, 10,
					  XmNleftWidget, PrefLpr_Label,
					  XmNrightAttachment, XmATTACH_FORM,
					  XmNrightOffset, 10,
					  XmNtopAttachment, XmATTACH_WIDGET,
					  XmNtopOffset, 10,
					  XmNtopWidget, Pref_RC,
					  NULL,0);
  XtAddCallback(PrefLpr_Text, XmNactivateCallback, 
                (XtCallbackProc) Pref_Confirm, Pref_Popup);
/*   XmTextFieldSetString(PrefLpr_Text,DEFAULT_Pref_COMMAND); */
  

  LabelString = XmStringCreate("Filename: ", XmSTRING_OS_CHARSET),
  PrefFile_Label = XtVaCreateManagedWidget("PrefFile_Label",
					    xmLabelWidgetClass, 
					    Pref_Form,
					    XmNlabelString, LabelString,
					    XmNleftAttachment, XmATTACH_FORM,
					    XmNleftOffset, 10,
					    XmNtopAttachment, XmATTACH_WIDGET,
					    XmNtopOffset, 15,
					    XmNtopWidget, PrefLpr_Text,
					    NULL,0);
  XmStringFree(LabelString);
  XtSetSensitive(PrefFile_Label,False);
  
  PrefFile_Text = XtVaCreateManagedWidget("PrefFile_Text",
					   xmTextFieldWidgetClass, 
					   Pref_Form,
					   XmNcolumns, 40,
					   XmNleftAttachment, XmATTACH_WIDGET,
					   XmNleftOffset, 10,
					   XmNleftWidget, PrefFile_Label,
					   XmNrightAttachment, XmATTACH_FORM,
					   XmNrightOffset, 10,
					   XmNtopAttachment, XmATTACH_WIDGET,
					   XmNtopOffset, 10,
					   XmNtopWidget, PrefLpr_Text,
					   NULL,0);
  XtAddCallback(PrefFile_Text, XmNactivateCallback, 
                (XtCallbackProc) Pref_Confirm, Pref_Popup);
/*   XmTextFieldSetString(PrefFile_Text,DEFAULT_Pref_FILENAME); */
  XtSetSensitive(PrefFile_Text,False);

  Pref_Separator = XtVaCreateManagedWidget("Pref_Separator",
					    xmSeparatorWidgetClass,
					    Pref_Form,
					    XmNtopAttachment, XmATTACH_WIDGET, 
					    XmNtopWidget, PrefFile_Text,
					    XmNtopOffset, 10,
					    XmNleftAttachment, XmATTACH_FORM,
					    XmNrightAttachment, XmATTACH_FORM,
					    NULL,0);
  

  LabelString = XmStringCreate("Pref", XmSTRING_OS_CHARSET),
  PrefConfirm_Button = XtVaCreateManagedWidget("PrefConfirm_Button",
						xmPushButtonWidgetClass,
						Pref_Form,
						XmNlabelString, LabelString,
						XmNtopAttachment, XmATTACH_WIDGET, 
						XmNbottomAttachment, XmATTACH_FORM,
						XmNtopWidget, Pref_Separator,
						XmNtopOffset, 10,
						XmNbottomOffset, 10,
						XmNleftAttachment, XmATTACH_POSITION,
						XmNleftPosition,25,
						NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(PrefConfirm_Button, XmNactivateCallback, 
                (XtCallbackProc) Pref_Confirm, Pref_Popup);
  
  
  LabelString = XmStringCreate("Reset", XmSTRING_OS_CHARSET),
  PrefReset_Button = XtVaCreateManagedWidget("PrefReset_Button",
					      xmPushButtonWidgetClass,
					      Pref_Form,
					      XmNlabelString, LabelString,
					      XmNtopAttachment, XmATTACH_WIDGET, 
					      XmNbottomAttachment, XmATTACH_FORM,
					      XmNtopWidget, Pref_Separator,
					      XmNtopOffset, 10,
					      XmNbottomOffset, 10,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition,50,
					      NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(PrefReset_Button, XmNactivateCallback, 
                (XtCallbackProc) Pref_Reset, NULL);
  
  
  LabelString = XmStringCreate("Cancel", XmSTRING_OS_CHARSET),
  PrefClose_Button = XtVaCreateManagedWidget("PrefClose_Button",
					      xmPushButtonWidgetClass,
					      Pref_Form,
					      XmNlabelString, LabelString,
					      XmNtopAttachment, XmATTACH_WIDGET, 
					      XmNbottomAttachment, XmATTACH_FORM,
					      XmNtopWidget, Pref_Separator,
					      XmNtopOffset, 10,
					      XmNbottomOffset, 10,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition,75,
					      NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(PrefClose_Button, XmNactivateCallback, 
                (XtCallbackProc) Pref_Close, Pref_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(Pref_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(Pref_Popup,WM_DELETE_WINDOW,
			  (XtCallbackProc)Pref_Close,Pref_Popup);

  XtManageChild(Pref_Frame);
  G_PreferencesVisible = True;
  XtPopup(Pref_Popup,XtGrabExclusive);
}

