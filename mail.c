#include "main.h"

Widget Mail_Popup;
Widget MailTo_Text;
Widget MailCC_Text;
Widget MailBCC_Text;
Widget MailSubject_Text;
Widget MailAttachment_Text;
Widget MailMessage_Text;

void Open_Attachment_Dialog();




void Mail_Close(Widget   w,
		Widget   PopupWidget, 
		XmAnyCallbackStruct *call_value)
{
  G_MailVisible = False;
  XtUnrealizeWidget(PopupWidget);
}

void Mail_Confirm(Widget   w,
		  Widget   PopupWidget, 
		  XmAnyCallbackStruct *call_value)
{
  char *To,*CC,*BCC,*Subject,*Attachment,*Message;
  char *Attachment_String = NULL;
  FILE *PipeFile;
  FILE *AttachmentFile;
  struct stat AttachmentFileStat;
  char Buffer[MY_BUFSIZ];

  if (strlen(To = XmTextFieldGetString(MailTo_Text)) == 0)
    {
      ErrorMessage("Sending Mail Error:\n\nTO: Field empty. Mail not sent.\n");
      return;
    }

  CC = XmTextFieldGetString(MailCC_Text);
  BCC = XmTextFieldGetString(MailBCC_Text);
  Subject = XmTextFieldGetString(MailSubject_Text);
  Attachment = XmTextFieldGetString(MailAttachment_Text);
  Message = XmTextGetString(MailMessage_Text);

  if (strlen(Attachment) != 0)
    {
      if ((AttachmentFile = fopen(Attachment,"r")) == NULL)
	{
	  sprintf(Buffer,"File Error:\n\nCould not open Attachment file %s",Attachment);
	  ErrorMessage(Buffer); 
	  XtFree(To);
	  XtFree(CC);
	  XtFree(BCC);
	  XtFree(Subject);
	  XtFree(Attachment);
	  XtFree(Message);
	  return;
	}
      if (stat(Attachment,&AttachmentFileStat) != 0)
	{
	  sprintf(Buffer,"Sending Mail Error:\n\nCould not stat Attachment file %s",Attachment);
	  ErrorMessage(Buffer); 
	  XtFree(To);
	  XtFree(CC);
	  XtFree(BCC);
	  XtFree(Subject);
	  XtFree(Attachment);
	  XtFree(Message);
	  return;
	}
      Attachment_String = (char *)malloc(AttachmentFileStat.st_size+1);
      read(fileno(AttachmentFile),Attachment_String,AttachmentFileStat.st_size-1);
      Attachment_String[AttachmentFileStat.st_size+1] = '\0';
      Attachment_String[AttachmentFileStat.st_size] = '\0';
      fclose(AttachmentFile);
    }

  if ((PipeFile = popen(SENDMAIL_COMMAND,"w")) == NULL)
    {
      ErrorMessage("Sending Mail Error:\n\nCould not open mail process.\n");
      XtFree(To);
      XtFree(CC);
      XtFree(BCC);
      XtFree(Subject);
      XtFree(Attachment);
      XtFree(Message);
      if (strlen(Attachment) !=0)  free(Attachment_String);
      G_MailVisible = False;
      XtUnrealizeWidget(PopupWidget);
      return;
    }

  fprintf(PipeFile,"To: %s\n",To);
  if (strlen(CC) != 0 ) fprintf(PipeFile,"Cc: %s\n",CC);
  if (strlen(BCC) != 0) fprintf(PipeFile,"Bcc: %s\n",BCC);
  if (strlen(Subject) != 0) fprintf(PipeFile,"Subject: %s\n",Subject);
  fprintf(PipeFile,"X-Mailer: %s\n",PROGRAM_TITLE);
  if (strlen(Message) != 0) fprintf(PipeFile,"\n%s\n",Message);
  
  if (Attachment_String != NULL) 
    {
      fprintf(PipeFile,"\n--------- Attachment Begins ---------\n%s\n--------- Attachment Ends ---------\n",Attachment_String);
    }
  pclose(PipeFile);

  XtFree(To);
  XtFree(CC);
  XtFree(BCC);
  XtFree(Subject);
  XtFree(Attachment);
  XtFree(Message);
  free(Attachment_String);

  G_MailVisible = False;
  XtUnrealizeWidget(PopupWidget);
}

void Mail_Reset(Widget   w,
		     Widget   TextWidget, 
		     XmAnyCallbackStruct *call_value)
{

  XmTextFieldSetString(MailTo_Text,G_CurrentEntryInRoloList->EmailAddress);

  XmTextFieldSetString(MailCC_Text,"");
  XmTextFieldSetString(MailBCC_Text,"");
  XmTextFieldSetString(MailSubject_Text,"");
  XmTextFieldSetString(MailAttachment_Text,"");
  if (G_Signature_String != NULL)
    {
      XmTextSetString(MailMessage_Text,G_Signature_String);
      XmTextInsert(MailMessage_Text,0,"\n\n");
      XmTextSetCursorPosition(MailMessage_Text,0);
    }
  else {
    XmTextSetString(MailMessage_Text,"");
  }
}

void  Send_Mail_Popup()
{
  Widget Mail_Frame;
  Widget Mail_Form;
  Widget Mail_Form2;
  Widget MailTo_Label;
  Widget MailCC_Label;
  Widget MailBCC_Label;
  Widget MailSubject_Label;
  Widget MailAttachment_Label;
  Widget MailAttachment_Button;
  Widget Mail_Separator;
  Widget MailConfirm_Button;
  Widget MailReset_Button;
  Widget MailClose_Button;
  XmString LabelString;
  Atom WM_DELETE_WINDOW;

  if (G_RoloList == NULL)
    {
      return;
    }

  if (G_MailVisible == True)
    {
      XRaiseWindow(XtDisplay(Mail_Popup),XtWindow(Mail_Popup));
      XMapWindow(XtDisplay(Mail_Popup),XtWindow(Mail_Popup));
      return;
    }
  
  Mail_Popup = XtVaAppCreateShell("Mail",
				  "Mail_Popup",
				  applicationShellWidgetClass,
				  XtDisplay(W_TopLevel),
				  XmNtitle, "Mail",
				  XmNdeleteResponse, XmDO_NOTHING,
				  NULL,0); 
  
  Mail_Frame = XtVaCreateManagedWidget("Mail_Frame",
				       xmFrameWidgetClass, 
				       Mail_Popup,
				       NULL,0);
  
  Mail_Form = XtVaCreateManagedWidget("Mail_Form",
				      xmFormWidgetClass, 
				      Mail_Frame,
				      NULL,0);
  
  Mail_Form2 = XtVaCreateManagedWidget("Mail_Form2",
				       xmFormWidgetClass, 
				       Mail_Form,
				       XmNleftAttachment, XmATTACH_FORM,
				       XmNrightAttachment, XmATTACH_FORM,
				       XmNtopAttachment, XmATTACH_POSITION,
				       XmNtopPosition, 0,
				       XmNtopOffset,10,
				       XmNbottomAttachment, XmATTACH_POSITION,
				       XmNbottomPosition, 30,
				       NULL,0);
  
  LabelString = XmStringCreate("To: ", XmFONTLIST_DEFAULT_TAG),
  MailTo_Label = XtVaCreateManagedWidget("MailTo_Label",
					 xmLabelWidgetClass, 
					 Mail_Form2,
					 XmNleftOffset, 10,
					 XmNlabelString, LabelString,
					 XmNleftAttachment, XmATTACH_FORM,
					 XmNalignment, XmALIGNMENT_END,
					 XmNrightAttachment, XmATTACH_POSITION,
					 XmNrightPosition, 20,
					 XmNtopAttachment, XmATTACH_POSITION,
					 XmNtopPosition, 0,
					 XmNbottomAttachment, XmATTACH_POSITION,
					 XmNbottomPosition, 20,
					 NULL,0);
  XmStringFree(LabelString);
  MailTo_Text = XtVaCreateManagedWidget("MailTo_Text",
					xmTextFieldWidgetClass, 
					Mail_Form2,
					XmNrightOffset, 10,
					XmNleftAttachment, XmATTACH_POSITION,
					XmNleftPosition, 20,
					XmNrightAttachment, XmATTACH_FORM,
					XmNtopAttachment, XmATTACH_POSITION,
					XmNtopPosition, 0,
					XmNbottomAttachment, XmATTACH_POSITION,
					XmNbottomPosition, 20,
					NULL,0);
  
  XmTextFieldSetString(MailTo_Text,G_CurrentEntryInRoloList->EmailAddress);

  LabelString = XmStringCreate("CC: ", XmFONTLIST_DEFAULT_TAG),
  MailCC_Label = XtVaCreateManagedWidget("MailCC_Label",
					 xmLabelWidgetClass, 
					 Mail_Form2,
					 XmNleftOffset, 10,
					 XmNlabelString, LabelString,
					 XmNleftAttachment, XmATTACH_FORM,
					 XmNalignment, XmALIGNMENT_END,
					 XmNrightAttachment, XmATTACH_POSITION,
					 XmNrightPosition, 20,
					 XmNtopAttachment, XmATTACH_POSITION,
					 XmNtopPosition, 20,
					 XmNbottomAttachment, XmATTACH_POSITION,
					 XmNbottomPosition, 40,
					 NULL,0);
  XmStringFree(LabelString);
  MailCC_Text = XtVaCreateManagedWidget("MailCC_Text",
					xmTextFieldWidgetClass, 
					Mail_Form2,
					XmNrightOffset, 10,
					XmNleftAttachment, XmATTACH_POSITION,
					XmNleftPosition, 20,
					XmNrightAttachment, XmATTACH_FORM,
					XmNtopAttachment, XmATTACH_POSITION,
					XmNtopPosition, 20,
					XmNbottomAttachment, XmATTACH_POSITION,
					XmNbottomPosition, 40,
					NULL,0);
  
  
  LabelString = XmStringCreate("BCC: ", XmFONTLIST_DEFAULT_TAG),
  MailBCC_Label = XtVaCreateManagedWidget("MailBCC_Label",
					  xmLabelWidgetClass, 
					  Mail_Form2,
					  XmNleftOffset, 10,
					  XmNlabelString, LabelString,
					  XmNleftAttachment, XmATTACH_FORM,
					  XmNalignment, XmALIGNMENT_END,
					  XmNrightAttachment, XmATTACH_POSITION,
					  XmNrightPosition, 20,
					  XmNtopAttachment, XmATTACH_POSITION,
					  XmNtopPosition, 40,
					  XmNbottomAttachment, XmATTACH_POSITION,
					  XmNbottomPosition, 60,
					  NULL,0);
  XmStringFree(LabelString);
  MailBCC_Text = XtVaCreateManagedWidget("MailBCC_Text",
					 xmTextFieldWidgetClass, 
					 Mail_Form2,
					 XmNrightOffset, 10,
					 XmNleftAttachment, XmATTACH_POSITION,
					 XmNleftPosition, 20,
					 XmNrightAttachment, XmATTACH_FORM,
					 XmNtopAttachment, XmATTACH_POSITION,
					 XmNtopPosition, 40,
					 XmNbottomAttachment, XmATTACH_POSITION,
					 XmNbottomPosition, 60,
					 NULL,0);
  
  LabelString = XmStringCreate("Subject: ", XmFONTLIST_DEFAULT_TAG),
  MailSubject_Label = XtVaCreateManagedWidget("MailSubject_Label",
					      xmLabelWidgetClass, 
					      Mail_Form2,
					      XmNleftOffset, 10,
					      XmNlabelString, LabelString,
					      XmNleftAttachment, XmATTACH_FORM,
					      XmNalignment, XmALIGNMENT_END,
					      XmNrightAttachment, XmATTACH_POSITION,
					      XmNrightPosition, 20,
					      XmNtopAttachment, XmATTACH_POSITION,
					      XmNtopPosition, 60,
					      XmNbottomAttachment, XmATTACH_POSITION,
					      XmNbottomPosition, 80,
					      NULL,0);
  XmStringFree(LabelString);
  MailSubject_Text = XtVaCreateManagedWidget("MailSubject_Text",
					     xmTextFieldWidgetClass, 
					     Mail_Form2,
					     XmNrightOffset, 10,
					     XmNleftAttachment, XmATTACH_POSITION,
					     XmNleftPosition, 20,
					     XmNrightAttachment, XmATTACH_FORM,
					     XmNtopAttachment, XmATTACH_POSITION,
					     XmNtopPosition, 60,
					     XmNbottomAttachment, XmATTACH_POSITION,
					     XmNbottomPosition, 80,
					     NULL,0);
  
  LabelString = XmStringCreate("Attachment: ", XmFONTLIST_DEFAULT_TAG),
  MailAttachment_Label = XtVaCreateManagedWidget("MailAttachment_Label",
						 xmLabelWidgetClass, 
						 Mail_Form2,
						 XmNleftOffset, 10,
						 XmNlabelString, LabelString,
						 XmNleftAttachment, XmATTACH_FORM,
						 XmNalignment, XmALIGNMENT_END,
						 XmNrightAttachment, XmATTACH_POSITION,
						 XmNrightPosition, 20,
						 XmNtopAttachment, XmATTACH_POSITION,
						 XmNtopPosition, 80,
						 XmNbottomAttachment, XmATTACH_POSITION,
						 XmNbottomPosition, 100,
						 NULL,0);
  XmStringFree(LabelString);
  MailAttachment_Text = XtVaCreateManagedWidget("MailSubject_Text",
						xmTextFieldWidgetClass, 
						Mail_Form2,
						XmNleftAttachment, XmATTACH_POSITION,
						XmNleftPosition, 20,
						XmNtopAttachment, XmATTACH_POSITION,
						XmNtopPosition, 80,
						XmNbottomAttachment, XmATTACH_POSITION,
						XmNbottomPosition, 100,
						XmNcolumns, 50,
						NULL,0);

  LabelString = XmStringCreate("Browse...", XmFONTLIST_DEFAULT_TAG),
  MailAttachment_Button = XtVaCreateManagedWidget("MailAttachment_Button",
						  xmPushButtonWidgetClass, 
						  Mail_Form2,
						  XmNlabelString, LabelString,
						  XmNrightOffset, 10,
						  XmNleftAttachment, XmATTACH_WIDGET,
						  XmNleftWidget, MailAttachment_Text,
						  XmNrightAttachment, XmATTACH_FORM,
						  XmNtopAttachment, XmATTACH_POSITION,
						  XmNtopPosition, 80,
						  XmNbottomAttachment, XmATTACH_POSITION,
						  XmNbottomPosition, 100,
						  NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(MailAttachment_Button, XmNactivateCallback, 
                (XtCallbackProc) Open_Attachment_Dialog, Mail_Popup);
  
  
  MailMessage_Text = XmCreateScrolledText(Mail_Form,
					  "MailMessage_Text",
					  NULL,0);
  
  XtVaSetValues(XtParent(MailMessage_Text),
		XmNleftAttachment, XmATTACH_FORM,
		XmNrightAttachment, XmATTACH_FORM,
		XmNrightOffset, 10,
		XmNleftOffset, 10,
		XmNtopAttachment, XmATTACH_POSITION,
		XmNtopPosition, 30,
		XmNbottomAttachment, XmATTACH_POSITION,
		XmNbottomPosition, 85,
		NULL,0);

  XtVaSetValues(MailMessage_Text,
		XmNcolumns, 80,
		XmNrows, 10,
		XmNwordWrap, True,
		XmNeditMode, XmMULTI_LINE_EDIT,
		NULL,0);

  XtManageChild(MailMessage_Text);
  
  
  if (G_Signature_String != NULL)
    {
      XmTextSetString(MailMessage_Text,G_Signature_String);
      XmTextInsert(MailMessage_Text,0,"\n\n");
      XmTextSetCursorPosition(MailMessage_Text,0);
    }
  
  Mail_Separator = XtVaCreateManagedWidget("Mail_Separator",
					   xmSeparatorWidgetClass,
					   Mail_Form,
					   XmNleftAttachment, XmATTACH_FORM,
					   XmNrightAttachment, XmATTACH_FORM,
					   XmNtopAttachment, XmATTACH_POSITION,
					   XmNtopPosition, 85,
					   XmNbottomAttachment, XmATTACH_POSITION,
					   XmNbottomPosition, 90,
					   NULL,0);
  
  LabelString = XmStringCreate("Send", XmFONTLIST_DEFAULT_TAG),
  MailConfirm_Button = XtVaCreateManagedWidget("MailConfirm_Button",
					       xmPushButtonWidgetClass,
					       Mail_Form,
					       XmNlabelString, LabelString,
					       XmNleftAttachment, XmATTACH_POSITION,
					       XmNleftPosition,25,
					       XmNtopAttachment, XmATTACH_POSITION,
					       XmNtopPosition, 90,
					       XmNbottomAttachment, XmATTACH_POSITION,
					       XmNbottomPosition, 95,
					       NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(MailConfirm_Button, XmNactivateCallback, 
                (XtCallbackProc) Mail_Confirm, Mail_Popup);
  
  
  LabelString = XmStringCreate("Reset", XmFONTLIST_DEFAULT_TAG),
  MailReset_Button = XtVaCreateManagedWidget("MailReset_Button",
					     xmPushButtonWidgetClass,
					     Mail_Form,
					     XmNlabelString, LabelString,
					     XmNleftAttachment, XmATTACH_POSITION,
					     XmNleftPosition,50,
					     XmNtopAttachment, XmATTACH_POSITION,
					     XmNtopPosition, 90,
					     XmNbottomAttachment, XmATTACH_POSITION,
					     XmNbottomPosition, 95,
					     NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(MailReset_Button, XmNactivateCallback, 
                (XtCallbackProc) Mail_Reset, NULL);
  
  
  LabelString = XmStringCreate("Cancel", XmFONTLIST_DEFAULT_TAG),
  MailClose_Button = XtVaCreateManagedWidget("MailClose_Button",
					     xmPushButtonWidgetClass,
					     Mail_Form,
					     XmNlabelString, LabelString,
					     XmNleftAttachment, XmATTACH_POSITION,
					     XmNleftPosition,75,
					     XmNtopAttachment, XmATTACH_POSITION,
					     XmNtopPosition, 90,
					     XmNbottomAttachment, XmATTACH_POSITION,
					     XmNbottomPosition, 95,
					     NULL,0);
  XmStringFree(LabelString);
  XtAddCallback(MailClose_Button, XmNactivateCallback, 
                (XtCallbackProc) Mail_Close, Mail_Popup );
  
  WM_DELETE_WINDOW = XmInternAtom(XtDisplay(Mail_Popup),"WM_DELETE_WINDOW",False);
  XmAddWMProtocolCallback(Mail_Popup,WM_DELETE_WINDOW,
                          (XtCallbackProc)Mail_Close,Mail_Popup);


/*   XtManageChild(Mail_Frame); */
/*   XtPopup(Mail_Popup,XtGrabExclusive); */
   XtRealizeWidget(Mail_Popup); 
  G_MailVisible = True;
}



void OK_Attachment(Widget   w,
		   Widget   PopupWidget, 
		   XmFileSelectionBoxCallbackStruct *cbs)
{
  char *Filename;

  if (!XmStringGetLtoR(cbs->value,XmFONTLIST_DEFAULT_TAG,&Filename))
    {
      XtUnrealizeWidget(w);
      return;      
    }
  if (Filename == NULL)
    {
      XtUnrealizeWidget(w); /* no filename given */
      XtFree(Filename);
      return;      
    }
  XmTextFieldSetString(MailAttachment_Text,Filename);
  XtFree(Filename);
  XtUnrealizeWidget(w);
  return;  
}

void CANCEL_Attachment(Widget   w,
			Widget   PopupWidget, 
			XmAnyCallbackStruct *cbs)
{
  XtUnrealizeWidget(w);
  return;
}

void Open_Attachment_Dialog()
{
  XmString DirString;
  Widget W_FileDialog;

  W_FileDialog = (Widget) XmCreateFileSelectionDialog(W_TopLevel,"Attachment",NULL,0);
  XtAddCallback(W_FileDialog, XmNcancelCallback, (XtCallbackProc) CANCEL_Attachment,NULL);
  XtAddCallback(W_FileDialog, XmNokCallback, (XtCallbackProc) OK_Attachment,NULL);
  XtUnmanageChild((Widget )XmFileSelectionBoxGetChild(W_FileDialog, XmDIALOG_HELP_BUTTON));
  XtVaSetValues(XtParent(W_FileDialog),
		XmNtitle,"Attachment File Selector",
		NULL,0);
  DirString = XmStringCreate((char *)getenv("HOME"), XmFONTLIST_DEFAULT_TAG),
  XtVaSetValues(W_FileDialog,
		XmNtextColumns, 60,
		XmNdirectory, DirString,
		NULL,0);
  XtManageChild(W_FileDialog);
}
