#include "main.h"

Widget Search_Popup;
int SearchCounter = 0;

void Search_Close(Widget   w,
		  Widget   PopupWidget, 
		  XmAnyCallbackStruct *call_value)
{
  SearchCounter = 0;
  XtUnrealizeWidget(PopupWidget);
  G_SearchVisible = False;
}

void Search_Confirm(Widget   w,
		    Widget   TextWidget, 
		    XmAnyCallbackStruct *call_value)
{
  char *SearchStr;
  char *TempStr;
  Rolo_Type *Temp;
  int Counter=0;
  int SearchItem;
 
  SearchStr = XmTextFieldGetString(TextWidget);
  if (G_RoloList == NULL)
    {
      XtFree(SearchStr);
      return;
    }
  Temp = G_RoloList;
  Counter = 0;
  while (Temp != NULL)
    {
      UpcaseWord(SearchStr);
      Counter++;
      if (SearchCounter > G_NumInRoloList)
	{
	  SearchCounter = 1;
	  Counter = 1;
	}
/*       fprintf(stderr,"%d %d %s %s\n",Counter,SearchCounter,TempStr,SearchStr); */
      for (SearchItem = 0; SearchItem <  9 ; SearchItem++)
	{
	  switch (SearchItem)
	    {
	    case 0 :
	      {
		TempStr = strdup(Temp->Name);
		break;
	      }
	    case 1 :
	      {
		TempStr = strdup(Temp->WorkPhone);
		break;
	      }
	    case 2 :
	      {
		TempStr = strdup(Temp->HomePhone);
		break;
	      }
	    case 3 :
	      {
		TempStr = strdup(Temp->Company);
		break;
	      }	
	    case 4 :
	      {
		TempStr = strdup(Temp->WorkAddress);
		break;
	      }
	    case 5 :
	      {
		TempStr = strdup(Temp->HomeAddress);
		break;
	      }
	    case 6 :
	      {
		TempStr = strdup(Temp->DateUpdated);
		break;
	      }
	    case 7 :
	      {
		TempStr = strdup(Temp->Remarks);
		break;
	      }
	    case 8 :
	      {
		TempStr = strdup(Temp->EmailAddress);
		break;
	      }
	    default :
	      {
		TempStr = strdup(Temp->Name);
		break;
	      }
	    }
	  UpcaseWord(TempStr);
	  if ((strstr(TempStr,SearchStr) != NULL) && (Counter > SearchCounter))
	    {
	      G_CurrentNumInRoloList = Counter;
	      G_CurrentEntryInRoloList = Temp;
	      SearchCounter = Counter;
	      XtFree(SearchStr);
	      free(TempStr);
	      CurrentEntry_RoloForm();
	      return;
	    }
	  free(TempStr);
	}
      
      Temp = Temp->Next;
      if (Counter >= G_NumInRoloList)
	{
	  SearchCounter = 0;
	  Counter = 1;
	}
    }
  InfoMessage("\nSearch complete, No matches found.\n");
  XtFree(SearchStr);
}

void Search_Reset(Widget   w,
		  Widget   TextWidget, 
		  XmAnyCallbackStruct *call_value)
{
  XmTextFieldSetString(TextWidget,"");
  SearchCounter = 0;
}



void  Search_Form()
{
  Widget Search_Frame;
  Widget Search_Form;
  Widget Search_Label;
  Widget Search_Text;
  Widget Search_Separator;
  Widget SearchConfirm_Button;
  Widget SearchReset_Button;
  Widget SearchClose_Button;
  XmString LabelString;
  Atom WM_DELETE_WINDOW;
  Position X,Y,W,H;
  
  if (G_SearchVisible == True)
    {
      XRaiseWindow(XtDisplay(Search_Popup),XtWindow(Search_Popup));
      XMapWindow(XtDisplay(Search_Popup),XtWindow(Search_Popup));
      return;
    }

  SearchCounter = 0;

/*  Search_Popup = XtVaAppCreateShell("Search",
				    "Search_Popup",
				    applicationShellWidgetClass,
				    XtDisplay(W_TopLevel),
				    XmNtitle, "Search",
				    XmNdeleteResponse, XmDO_NOTHING,
				    NULL,0); 
  */
  
  XtVaGetValues(W_TopLevel,
		XmNx,&X,
		XmNy,&Y,
		XmNwidth,&W,
		XmNheight,&H,
		NULL,0);

  Search_Popup = XtVaCreatePopupShell("Search",
				      xmDialogShellWidgetClass,
				      W_TopLevel,
				      XmNtitle, "Search",
				      XmNdeleteResponse, XmDO_NOTHING,
				      XmNx,X+(W/5),
				      XmNy,Y+(H/5),
				      NULL,0); 


  Search_Frame = XtVaCreateWidget("Search_Frame",
				  xmFrameWidgetClass, 
				  Search_Popup,
				  NULL,0);
  
  Search_Form = XtVaCreateManagedWidget("Search_Form",
					xmFormWidgetClass, 
					Search_Frame,
					NULL,0);
  
  LabelString = XmStringCreate("Enter Search String: ", XmSTRING_OS_CHARSET),
  Search_Label = XtVaCreateManagedWidget("Search_Label",
					 xmLabelWidgetClass, 
					 Search_Form,
					 XmNlabelString, LabelString,
					 XmNleftAttachment, XmATTACH_FORM,
					 XmNleftOffset, 10,
					 XmNtopAttachment, XmATTACH_FORM,
					 XmNtopOffset, 10,
					 NULL,0);
  XmStringFree(LabelString);

  Search_Text = XtVaCreateManagedWidget("Search_Text",
					xmTextFieldWidgetClass, 
					Search_Form,
					XmNleftAttachment, XmATTACH_WIDGET,
					XmNleftWidget, Search_Label,
					XmNrightAttachment, XmATTACH_FORM,
					XmNrightOffset, 10,
					XmNtopAttachment, XmATTACH_FORM,
					XmNtopOffset, 10,
					XmNcolumns, 30,
					NULL,0);
  XtAddCallback(Search_Text, XmNactivateCallback, 
                (XtCallbackProc) Search_Confirm, Search_Text);
  

  Search_Separator = XtVaCreateManagedWidget("Search_Separator",
					     xmSeparatorWidgetClass,
					     Search_Form,
					     XmNtopAttachment, XmATTACH_WIDGET, 
					     XmNtopWidget, Search_Text,
					     XmNtopOffset, 10,
					     XmNleftAttachment, XmATTACH_FORM,
					     XmNrightAttachment, XmATTACH_FORM,
					     NULL,0);
  
  LabelString = XmStringCreate("Search", XmSTRING_OS_CHARSET),
  SearchConfirm_Button = XtVaCreateManagedWidget("SearchConfirm_Button",
						 xmPushButtonWidgetClass,
						 Search_Form,
						 XmNlabelString, LabelString,
						 XmNtopAttachment, XmATTACH_WIDGET, 
						 XmNbottomAttachment, XmATTACH_FORM,
						 XmNtopWidget, Search_Separator,
						 XmNtopOffset, 10,
						 XmNbottomOffset, 10,
						 XmNleftAttachment, XmATTACH_POSITION,
						 XmNleftPosition,25,
						 NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(SearchConfirm_Button, XmNactivateCallback, 
                (XtCallbackProc) Search_Confirm, Search_Text);
  

  LabelString = XmStringCreate("Reset", XmSTRING_OS_CHARSET),
  SearchReset_Button = XtVaCreateManagedWidget("SearchReset_Button",
					       xmPushButtonWidgetClass,
					       Search_Form,
					       XmNlabelString, LabelString,
					       XmNtopAttachment, XmATTACH_WIDGET, 
					       XmNbottomAttachment, XmATTACH_FORM,
					       XmNtopWidget, Search_Separator,
					       XmNtopOffset, 10,
					       XmNbottomOffset, 10,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition,50,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(SearchReset_Button, XmNactivateCallback, 
                (XtCallbackProc) Search_Reset, Search_Text);
  
  
  LabelString = XmStringCreate("Close", XmSTRING_OS_CHARSET),
  SearchClose_Button = XtVaCreateManagedWidget("SearchClose_Button",
					       xmPushButtonWidgetClass,
					       Search_Form,
					       XmNlabelString, LabelString,
					       XmNtopAttachment, XmATTACH_WIDGET, 
					       XmNbottomAttachment, XmATTACH_FORM,
					       XmNtopWidget, Search_Separator,
					       XmNtopOffset, 10,
					       XmNbottomOffset, 10,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition,75,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(SearchClose_Button, XmNactivateCallback, 
                (XtCallbackProc) Search_Close, Search_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(Search_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(Search_Popup,WM_DELETE_WINDOW,
			  (XtCallbackProc)Search_Close,Search_Popup);

  XtManageChild(Search_Frame);
  XtPopup(Search_Popup,XtGrabExclusive);
/*   XtRealizeWidget(Search_Popup); */
  G_SearchVisible = True;
}

