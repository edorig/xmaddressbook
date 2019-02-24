#include "main.h"

Widget BCard_Popup;
Widget BCard_Text;

void BCard_Close(Widget   w,
		 Widget   PopupWidget, 
		 XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  G_BCardVisible = False;
}

void Update_BCard(Rolo_Type *CurrentEntry)
{
  if (CurrentEntry == NULL)
    {
      return;
    }
  
  XmTextSetString(BCard_Text,"Name:       ");
  if (strlen(CurrentEntry->Name) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->Name);
    }

  if (strlen(CurrentEntry->EmailAddress) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),"\nEmail:      ");
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->EmailAddress);
    }
  if (strlen(CurrentEntry->WorkPhone) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),"\nWork:       ");
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->WorkPhone);
    }
  if (strlen(CurrentEntry->FaxNumber) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),"\nFax:        ");
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->FaxNumber);
    }
  if (strlen(CurrentEntry->Company) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),"\nCompany:    ");
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->Company);
    }
  if (strlen(CurrentEntry->WorkAddress) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),"\nWork Addr.: ");
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->WorkAddress);
    }
  if (strlen(CurrentEntry->HomePhone) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),"\nHome:       ");     
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->HomePhone);
    }
  if (strlen(CurrentEntry->HomeAddress) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),"\nHome Addr.: ");
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->HomeAddress);
    }
  if (strlen(CurrentEntry->Remarks) > 1)
    {
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),"\nRemarks:    ");
      XmTextInsert(BCard_Text,XmTextGetLastPosition(BCard_Text),CurrentEntry->Remarks);
    }
}

void Show_BCard(Rolo_Type *CurrentEntry)
{
  Widget BCard_Frame;
  Widget BCard_Form;
  Widget BCard_Label;
  Widget BCard_Separator;
  Widget BCardClose_Button;
  XmString LabelString;
  Atom WM_DELETE_WINDOW;


  if (G_BCardVisible == True)
    {
      XRaiseWindow(XtDisplay(BCard_Popup),XtWindow(BCard_Popup));
      XMapWindow(XtDisplay(BCard_Popup),XtWindow(BCard_Popup));
      Update_BCard(CurrentEntry);
      return;
    }

  BCard_Popup = XtVaAppCreateShell("Business Card",
				   "BCard_Popup",
				   applicationShellWidgetClass,
				   XtDisplay(W_TopLevel),
				   XmNtitle, "Business Card",
				   XmNdeleteResponse, XmDO_NOTHING,
				   NULL,0); 
  
  BCard_Frame = XtVaCreateManagedWidget("BCard_Frame",
					xmFrameWidgetClass, 
					BCard_Popup,
					NULL,0);
  
  BCard_Form = XtVaCreateManagedWidget("BCard_Form",
				       xmFormWidgetClass, 
				       BCard_Frame,
				       XmNrubberPositioning,True,
				       NULL,0);
  
  LabelString = XmStringCreate("Business Card", XmFONTLIST_DEFAULT_TAG),
  BCard_Label = XtVaCreateManagedWidget("BCard_Label",
					xmLabelWidgetClass, 
					BCard_Form,
					XmNlabelString, LabelString,
					XmNleftAttachment, XmATTACH_FORM,
					XmNrightAttachment, XmATTACH_FORM,
					XmNtopAttachment, XmATTACH_POSITION,
					XmNtopPosition, 5,
					XmNbottomAttachment, XmATTACH_POSITION,
					XmNbottomPosition, 10,
					NULL,0);
  XmStringFree(LabelString);
  
  

  BCard_Text = XmCreateScrolledText(BCard_Form,
				    "BCard_Text",
				    NULL,0);
  
  XtVaSetValues(XtParent(BCard_Text),
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNtopAttachment, XmATTACH_POSITION,
		XmNtopPosition, 10,
		XmNbottomAttachment, XmATTACH_POSITION,
		XmNbottomPosition, 85,
		NULL,0);
  
  XtVaSetValues(BCard_Text,
		XmNrows, 7,
		XmNcolumns, 70,
		XmNeditMode, XmMULTI_LINE_EDIT,
		XmNeditable, False,
		NULL,0);

  XtManageChild(BCard_Text);

  Update_BCard(CurrentEntry);
  
  BCard_Separator = XtVaCreateManagedWidget("BCard_Separator",
					    xmSeparatorWidgetClass,
					    BCard_Form,
					    XmNtopAttachment, XmATTACH_WIDGET, 
					    XmNtopWidget, BCard_Text,
					    XmNtopOffset, 10,
					    XmNleftAttachment, XmATTACH_FORM,
					    XmNrightAttachment, XmATTACH_FORM,
					    XmNtopAttachment, XmATTACH_POSITION,
					    XmNtopPosition, 85,
					    XmNbottomAttachment, XmATTACH_POSITION,
					    XmNbottomPosition, 90,
					    NULL,0);
    
  LabelString = XmStringCreate("Close", XmFONTLIST_DEFAULT_TAG),
  BCardClose_Button = XtVaCreateManagedWidget("BCardClose_Button",
					      xmPushButtonWidgetClass,
					      BCard_Form,
					      XmNlabelString, LabelString,
					      XmNtopAttachment, XmATTACH_WIDGET,
					      XmNtopWidget, BCard_Separator,
					      XmNbottomAttachment, XmATTACH_POSITION,
					      XmNbottomPosition, 98,
					      XmNleftAttachment, XmATTACH_POSITION,
					      XmNleftPosition,40,
					      XmNrightAttachment, XmATTACH_POSITION,
					      XmNrightPosition,60,
					      NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(BCardClose_Button, XmNactivateCallback, 
                (XtCallbackProc) BCard_Close, BCard_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(BCard_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(BCard_Popup,WM_DELETE_WINDOW,
			  (XtCallbackProc)BCard_Close,BCard_Popup);

  XtRealizeWidget(BCard_Popup);
  G_BCardVisible = True;
}

