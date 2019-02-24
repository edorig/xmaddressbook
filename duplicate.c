#include "main.h"

Widget Duplicate_Popup;
Widget DuplicateEntryNum_Text;
Widget DuplicateTimes_Text;


void Duplicate_Close(Widget   w,
		     Widget   PopupWidget, 
		     XmAnyCallbackStruct *call_value)
{
  G_DuplicateVisible = False;
  XtUnrealizeWidget(PopupWidget);
}

void Duplicate_Confirm(Widget   w,
		       Widget   PopupWidget, 
		       XmAnyCallbackStruct *call_value)
{
  char *EntryNumStr;
  char *TimesStr;
  int EntryNum = 0;
  int Times = 0;
  struct Rolo_Struct Entry;
  
  if ((EntryNumStr = XmTextFieldGetString(DuplicateEntryNum_Text)) == NULL)
    {
      return;
    }
  if ((TimesStr = XmTextFieldGetString(DuplicateTimes_Text)) == NULL)
    {
      return;
    }
  EntryNum  = atoi(EntryNumStr);
  Times   = atoi(TimesStr);
  XtFree(EntryNumStr);
  XtFree(TimesStr);

  if (EntryNum > G_NumInRoloList)
    {
      EntryNum = G_NumInRoloList;
    }
  if (EntryNum <= 0)
    {
      EntryNum = 1;
    }
  G_CurrentNumInRoloList = 1;
  G_CurrentEntryInRoloList = G_RoloList;
  while ((G_CurrentEntryInRoloList != NULL) && (G_CurrentNumInRoloList != EntryNum))
    {
      G_CurrentEntryInRoloList = G_CurrentEntryInRoloList->Next;
      G_CurrentNumInRoloList++;
    }

  Entry.Name = strdup(G_CurrentEntryInRoloList->Name);
  Entry.WorkPhone = strdup(G_CurrentEntryInRoloList->WorkPhone);
  Entry.HomePhone = strdup(G_CurrentEntryInRoloList->HomePhone);
  Entry.Company = strdup(G_CurrentEntryInRoloList->Company);
  Entry.FaxNumber = strdup(G_CurrentEntryInRoloList->FaxNumber);
  Entry.EmailAddress = strdup(G_CurrentEntryInRoloList->EmailAddress);
  Entry.WorkAddress = strdup(G_CurrentEntryInRoloList->WorkAddress);
  Entry.HomeAddress = strdup(G_CurrentEntryInRoloList->HomeAddress);
  Entry.DateUpdated = strdup(G_CurrentEntryInRoloList->DateUpdated);
  Entry.Remarks = strdup(G_CurrentEntryInRoloList->Remarks);

  for (;(Times > 0);Times--)
    {
      Add_To_List(Entry);
    }
  CurrentEntry_RoloForm();
  
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

  G_DuplicateVisible = False;
  G_ChangedNotSaved = True;
  XtUnrealizeWidget(PopupWidget);
}

void Duplicate_Reset(Widget   w,
		     Widget   TextWidget, 
		     XmAnyCallbackStruct *call_value)
{
  XmTextFieldSetString(DuplicateTimes_Text,"");
  XmTextFieldSetString(DuplicateEntryNum_Text,"");
}

void  Duplicate_Entry()
{
  Widget Duplicate_Frame;
  Widget Duplicate_Form;
  Widget Duplicate_Form2;
  Widget Duplicate_Label;
  Widget Duplicate_Separator;
  Widget DuplicateConfirm_Button;
  Widget DuplicateReset_Button;
  Widget DuplicateClose_Button;
  XmString LabelString;
  char  Buffer[MY_BUFSIZ+1];
  Atom WM_DELETE_WINDOW;
  Position X,Y,W,H;

  if (G_RoloList == NULL)
    {
      return;
    }

  if (G_DuplicateVisible == True)
    {
      XRaiseWindow(XtDisplay(Duplicate_Popup),XtWindow(Duplicate_Popup));
      XMapWindow(XtDisplay(Duplicate_Popup),XtWindow(Duplicate_Popup));
      return;
    }
  
/*  Duplicate_Popup = XtVaAppCreateShell("Duplicate_Popup",
				       "Duplicate_Popup",
				       applicationShellWidgetClass,
				       XtDisplay(W_TopLevel),
				       XmNtitle, "Duplicate",
				       XmNdeleteResponse, XmDO_NOTHING,
				       NULL,0); 
*/

  XtVaGetValues(W_TopLevel,
		XmNx,&X,
		XmNy,&Y,
		XmNwidth,&W,
		XmNheight,&H,
		NULL,0);

  Duplicate_Popup = XtVaCreatePopupShell("Duplicate",
					 xmDialogShellWidgetClass,
					 W_TopLevel,
					 XmNtitle, "Duplicate",
					 XmNdeleteResponse, XmDO_NOTHING,
					 XmNx,X+(W/5),
					 XmNy,Y+(H/5),
					 NULL,0); 
  
  
  Duplicate_Frame = XtVaCreateWidget("Duplicate_Frame",
				     xmFrameWidgetClass, 
				     Duplicate_Popup,
				     NULL,0);
  
  Duplicate_Form = XtVaCreateManagedWidget("Duplicate_Form",
					   xmFormWidgetClass, 
					   Duplicate_Frame,
					   NULL,0);
  
  Duplicate_Form2 = XtVaCreateManagedWidget("Duplicate_Form",
					    xmFormWidgetClass, 
					    Duplicate_Form,
					    XmNfractionBase,4,
					    XmNleftAttachment, XmATTACH_FORM,
					    XmNrightAttachment, XmATTACH_FORM,
					    NULL,0);
  
  LabelString = XmStringCreate("Entry Number: ", XmFONTLIST_DEFAULT_TAG),
  Duplicate_Label = XtVaCreateManagedWidget("Duplicate_Label",
					    xmLabelWidgetClass, 
					    Duplicate_Form2,
					    XmNlabelString, LabelString,
					    XmNleftAttachment, XmATTACH_POSITION,
					    XmNleftPosition, 0,
					    XmNrightAttachment, XmATTACH_POSITION,
					    XmNrightPosition, 1,
					    XmNleftOffset, 5,
					    XmNtopAttachment, XmATTACH_FORM,
					    XmNtopOffset, 15,
					    NULL,0);
  XmStringFree(LabelString);
  
  DuplicateEntryNum_Text = XtVaCreateManagedWidget("DuplicateEntryNum_Text",
						   xmTextFieldWidgetClass, 
						   Duplicate_Form2,
						   XmNleftAttachment, XmATTACH_POSITION,
						   XmNleftPosition, 1,
						   XmNrightAttachment, XmATTACH_POSITION,
						   XmNrightPosition, 2,
						   XmNrightOffset, 5,
						   XmNcolumns, 4,
						   XmNtopAttachment, XmATTACH_FORM,
						   XmNtopOffset, 10,
						   NULL,0);
  
  XtAddCallback(DuplicateEntryNum_Text, XmNactivateCallback, 
                (XtCallbackProc) Duplicate_Confirm, Duplicate_Popup);
  
  LabelString = XmStringCreate("Duplicate n Times: ", XmFONTLIST_DEFAULT_TAG),
  Duplicate_Label = XtVaCreateManagedWidget("Duplicate_Label",
					    xmLabelWidgetClass, 
					    Duplicate_Form2,
					    XmNlabelString, LabelString,
					    XmNleftAttachment, XmATTACH_POSITION,
					    XmNleftPosition, 2,
					    XmNrightAttachment, XmATTACH_POSITION,
					    XmNrightPosition, 3,
					    XmNtopAttachment, XmATTACH_FORM,
					    XmNtopOffset, 15,
					    NULL,0);
  XmStringFree(LabelString);

  DuplicateTimes_Text = XtVaCreateManagedWidget("DuplicateTimes_Text",
						xmTextFieldWidgetClass, 
						Duplicate_Form2,
						XmNleftAttachment, XmATTACH_POSITION,
						XmNleftPosition, 3,
						XmNrightAttachment, XmATTACH_POSITION,
						XmNrightPosition, 4,
						XmNtopAttachment, XmATTACH_FORM,
						XmNtopOffset, 10,
						XmNcolumns, 4,
						XmNrightOffset, 5,
						NULL,0);
  XtAddCallback(DuplicateTimes_Text, XmNactivateCallback, 
                (XtCallbackProc) Duplicate_Confirm, Duplicate_Popup);
  
  XmTextFieldSetString(DuplicateTimes_Text,"");
  sprintf(Buffer,"%d",G_CurrentNumInRoloList);
  XmTextFieldSetString(DuplicateEntryNum_Text,Buffer);

  Duplicate_Separator = XtVaCreateManagedWidget("Duplicate_Separator",
						xmSeparatorWidgetClass,
						Duplicate_Form,
						XmNtopAttachment, XmATTACH_WIDGET, 
						XmNtopWidget, DuplicateTimes_Text,
						XmNtopOffset, 10,
						XmNleftAttachment, XmATTACH_FORM,
						XmNrightAttachment, XmATTACH_FORM,
						NULL,0);
  
  LabelString = XmStringCreate("Duplicate", XmFONTLIST_DEFAULT_TAG),
  DuplicateConfirm_Button = XtVaCreateManagedWidget("DuplicateConfirm_Button",
						    xmPushButtonWidgetClass,
						    Duplicate_Form,
						    XmNlabelString, LabelString,
						    XmNtopAttachment, XmATTACH_WIDGET, 
						    XmNbottomAttachment, XmATTACH_FORM,
						    XmNtopWidget, Duplicate_Separator,
						    XmNtopOffset, 10,
						    XmNbottomOffset, 10,
						    XmNleftAttachment, XmATTACH_POSITION,
						    XmNleftPosition,25,
						    NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(DuplicateConfirm_Button, XmNactivateCallback, 
                (XtCallbackProc) Duplicate_Confirm, Duplicate_Popup);
  

  LabelString = XmStringCreate("Reset", XmFONTLIST_DEFAULT_TAG),
  DuplicateReset_Button = XtVaCreateManagedWidget("DuplicateReset_Button",
						  xmPushButtonWidgetClass,
						  Duplicate_Form,
						  XmNlabelString, LabelString,
						  XmNtopAttachment, XmATTACH_WIDGET, 
						  XmNbottomAttachment, XmATTACH_FORM,
						  XmNtopWidget, Duplicate_Separator,
						  XmNtopOffset, 10,
						  XmNbottomOffset, 10,
						  XmNleftAttachment, XmATTACH_POSITION,
						  XmNleftPosition,50,
						  NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(DuplicateReset_Button, XmNactivateCallback, 
                (XtCallbackProc) Duplicate_Reset, NULL);
  
  
  LabelString = XmStringCreate("Close", XmFONTLIST_DEFAULT_TAG),
  DuplicateClose_Button = XtVaCreateManagedWidget("DuplicateClose_Button",
					       xmPushButtonWidgetClass,
					       Duplicate_Form,
					       XmNlabelString, LabelString,
					       XmNtopAttachment, XmATTACH_WIDGET, 
					       XmNbottomAttachment, XmATTACH_FORM,
					       XmNtopWidget, Duplicate_Separator,
					       XmNtopOffset, 10,
					       XmNbottomOffset, 10,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition,75,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(DuplicateClose_Button, XmNactivateCallback, 
                (XtCallbackProc) Duplicate_Close, Duplicate_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(Duplicate_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(Duplicate_Popup,WM_DELETE_WINDOW,
                          (XtCallbackProc)Duplicate_Close,Duplicate_Popup);

  XtManageChild(Duplicate_Frame);
  XtPopup(Duplicate_Popup,XtGrabExclusive);
/*   XtRealizeWidget(Duplicate_Popup); */
  G_DuplicateVisible = True;
}


