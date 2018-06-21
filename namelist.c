#include "main.h"

Widget NameList_Popup;

void NameList_Close(Widget   w,
		    Widget   PopupWidget, 
		    XmAnyCallbackStruct *call_value)
{
  XtUnrealizeWidget(PopupWidget);
  G_NameListVisible = False;
}

void Name_Selected (Widget   w,
		    Widget   ListWidget, 
		    XmListCallbackStruct *call_value)
{
  Goto_Actual_Entry(call_value->item_position);

  if (G_BCardVisible == True)
    {
      Show_BCard(G_CurrentEntryInRoloList);
    }
}

void BuildNameList(Widget ListWidget)
{
  XmString ListString;
  Rolo_Type *Temp;

  if (G_RoloList == NULL)
    {
      return;
    }
  Temp = G_RoloList;
  while (Temp != NULL)
    {
      ListString = XmStringCreate(Temp->Name, XmSTRING_OS_CHARSET);
      XmListAddItem(ListWidget,ListString,0);
      XmStringFree(ListString);
      Temp = Temp->Next;
    }
}


void  Show_Name_List()
{
  Widget NameList_Frame;
  Widget NameList_Form;
  Widget NameList_Label;
  Widget NameList_Separator;
  Widget NameListClose_Button;
  XmString LabelString;
  Atom WM_DELETE_WINDOW;
  
  if (G_NameListVisible == True)
    {
      XRaiseWindow(XtDisplay(NameList_Popup),XtWindow(NameList_Popup));
      XMapWindow(XtDisplay(NameList_Popup),XtWindow(NameList_Popup));
      return;
    }


  NameList_Popup = XtVaAppCreateShell("Name List",
				      "NameList_Popup",
				      applicationShellWidgetClass,
				      XtDisplay(W_TopLevel),
				      XmNtitle, "Name List",
				      XmNdeleteResponse, XmDO_NOTHING,
				      NULL,0); 
  
  NameList_Frame = XtVaCreateWidget("NameList_Frame",
				    xmFrameWidgetClass, 
				    NameList_Popup,
				    NULL,0);
  
  NameList_Form = XtVaCreateManagedWidget("NameList_Form",
					xmFormWidgetClass, 
					NameList_Frame,
					NULL,0);
  
  LabelString = XmStringCreate("Name", XmSTRING_OS_CHARSET);
  NameList_Label = XtVaCreateManagedWidget("NameList_Label",
					   xmLabelWidgetClass, 
					   NameList_Form,
					   XmNlabelString, LabelString,
					   XmNleftAttachment, XmATTACH_FORM,
					   XmNleftOffset, 10,
					   XmNrightAttachment, XmATTACH_FORM,
					   XmNrightOffset, 10,
					   XmNtopAttachment, XmATTACH_POSITION,
					   XmNtopPosition, 5,
					   XmNbottomAttachment, XmATTACH_POSITION,
					   XmNbottomPosition, 10,
					   NULL,0);
  XmStringFree(LabelString);

  NameList_List = XmCreateScrolledList(NameList_Form,
				       "NameList_List",
				       NULL,0);
  

  XtVaSetValues(XtParent(NameList_List),
		XmNleftAttachment, XmATTACH_FORM,
		XmNleftOffset, 10,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNtopAttachment, XmATTACH_POSITION,
		XmNtopPosition, 10,
		XmNbottomAttachment, XmATTACH_POSITION,
		XmNbottomPosition, 85,
		NULL,0);

  XtVaSetValues(NameList_List,
		XmNselectionPolicy, XmSINGLE_SELECT,
		XmNvisibleItemCount, 20,
/* 		XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE, */
		NULL,0);

  XtManageChild(NameList_List);

  XtAddCallback(NameList_List, XmNsingleSelectionCallback, 
                (XtCallbackProc) Name_Selected, NameList_List);
  
  BuildNameList(NameList_List);

  NameList_Separator = XtVaCreateManagedWidget("NameList_Separator",
					       xmSeparatorWidgetClass,
					       NameList_Form,
					       XmNtopAttachment, XmATTACH_POSITION,
					       XmNtopPosition, 85,
					       XmNbottomAttachment, XmATTACH_POSITION,
					       XmNbottomPosition, 90,
					       XmNleftAttachment, XmATTACH_FORM,
					       XmNrightAttachment, XmATTACH_FORM,
					       NULL,0);
  
  
  LabelString = XmStringCreate("Close", XmSTRING_OS_CHARSET),
  NameListClose_Button = XtVaCreateManagedWidget("NameListClose_Button",
					       xmPushButtonWidgetClass,
					       NameList_Form,
					       XmNlabelString, LabelString,
					       XmNtopAttachment, XmATTACH_POSITION,
					       XmNtopPosition, 90,
					       XmNbottomAttachment, XmATTACH_POSITION,
					       XmNbottomPosition, 95,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition,40,
					       XmNrightAttachment, XmATTACH_POSITION,
					       XmNrightPosition,60,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(NameListClose_Button, XmNactivateCallback, 
                (XtCallbackProc) NameList_Close, NameList_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(NameList_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(NameList_Popup,WM_DELETE_WINDOW,
			  (XtCallbackProc)NameList_Close,NameList_Popup);

  XtManageChild(NameList_Frame);
  XtRealizeWidget(NameList_Popup);
  G_NameListVisible = True;
}

