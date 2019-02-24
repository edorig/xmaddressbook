#include "main.h"

/* local prototypes */

void Open_AddressBook_Dialog();

/********************

   Convert_Addressbook -- converts the old rolo program database to one readable by this program.

*********************/

ReturnType Convert_AddressBook(char *SrcFilename)
{
  FILE *SrcRoloFile;
  FILE *DestRoloFile;
  char  Line[MY_BUFSIZ+1];
  char  BigLine[MY_BIG_BUFSIZ+1];
  char DestFilename[MY_BUFSIZ+1];
  char *Name;
  char *WorkPhone;
  char *HomePhone;
  char *Company;
  char *WorkAddress;
  char *HomeAddress;
  char *DateUpdated;
  char *Remarks;
  
  if ((SrcRoloFile = fopen(SrcFilename,"r")) == NULL)
    {
      perror(SrcFilename);
      return(RETURN_NOTOK);
    }
  sprintf(DestFilename,"%s_%d%s",SrcFilename,getpid(),TEMP_FILENAME_EXTENSION);
  if ((DestRoloFile = fopen(DestFilename,"w")) == NULL)
    {
      perror(DestFilename);
      return(RETURN_NOTOK);
    }
  fprintf(DestRoloFile,"#RS1.1\n");
  fgets(Line,MY_BUFSIZ,SrcRoloFile);
  while (!feof(SrcRoloFile))
    {
      Remove_CR(Line);
      Name = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      WorkPhone = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      HomePhone = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      Company = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      WorkAddress = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      HomeAddress = strdup(Line);

      fgets(BigLine,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(BigLine);
      Remarks = strdup(BigLine);
      
      /* get double blank lines before date updated */

      if (strcmp(Remarks,"\n") == 0)
	{
	  int FoundEnd = 1;
	  
	  fgets(Line,MY_BUFSIZ,SrcRoloFile);
	  while (!feof(SrcRoloFile) && (FoundEnd != 2))
	    {
	      if (strcmp(Line,"\n") == 0)
		{
		  FoundEnd = 2;
		}
	      else {
		fgets(Line,MY_BUFSIZ,SrcRoloFile);
	      }
	    }
	}
      
      /* get date updated */
      
      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      DateUpdated = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      while (!feof(SrcRoloFile) && (strcmp(Line,"\n") != 0))
	{
	  Remarks = (char *)realloc(Remarks,sizeof(Line));
	  strcat(Remarks,Line);
	  fgets(Line,MY_BUFSIZ,SrcRoloFile);
	}
      
      fprintf(DestRoloFile,"%s\n",Name);
      fprintf(DestRoloFile,"%s\n",WorkPhone);
      fprintf(DestRoloFile,"%s\n",HomePhone);
      fprintf(DestRoloFile,"%s\n",Company);
      fprintf(DestRoloFile,"%s\n",WorkAddress);
      fprintf(DestRoloFile,"%s\n",HomeAddress);
      fprintf(DestRoloFile,"\n"); /*email address*/ 
      fprintf(DestRoloFile,"\n"); /*fax address*/
      fprintf(DestRoloFile,"%s\n",DateUpdated);
      fprintf(DestRoloFile,"%s\n#RE\n",Remarks);

      free(Name);
      free(WorkPhone);
      free(HomePhone);
      free(Company);
      free(WorkAddress);
      free(HomeAddress);
      free(DateUpdated);
      free(Remarks);
      fgets(Line,MY_BUFSIZ,SrcRoloFile);
    }
  fclose (DestRoloFile);
  fclose (SrcRoloFile);
  if (rename(DestFilename,SrcFilename) != 0)
    {
      perror("rename");
      exit(-1);
    }
  return(RETURN_OK);
}

/********************

   Read_Old_AddressBook -- Read a XmAddressBook file, i.e. an unonverted rolo file.

*********************/

ReturnType Read_Old_AddressBook(char *Filename)
{
  FILE *RoloFile;
  char  Line[MY_BUFSIZ+1];
  char  BigLine[MY_BIG_BUFSIZ+1];
  struct Rolo_Struct Entry;
  
  if ((RoloFile = fopen(Filename,"r")) == NULL)
    {
      perror(Filename);
      return(RETURN_NOTOK);
    }

  fgets(Line,MY_BUFSIZ,RoloFile);
  while (!feof(RoloFile))
    {
      Remove_CR(Line);
      Entry.Name = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.WorkPhone = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.HomePhone = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.Company = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.WorkAddress = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.HomeAddress = strdup(Line);

      fgets(BigLine,MY_BUFSIZ,RoloFile);
      Remove_CR(BigLine);
      Entry.Remarks = strdup(BigLine);
      
      /* get double blank lines before date updated */

      if (strcmp(Entry.Remarks,"\n") == 0)
	{
	  int FoundEnd = 1;
	  
	  fgets(Line,MY_BUFSIZ,RoloFile);
	  while (!feof(RoloFile) && (FoundEnd != 2))
	    {
	      if (strcmp(Line,"\n") == 0)
		{
		  FoundEnd = 2;
		}
	      else {
		fgets(Line,MY_BUFSIZ,RoloFile);
	      }
	    }
	}
      
      /* get date updated */
      
      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.DateUpdated = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      while (!feof(RoloFile) && (strcmp(Line,"\n") != 0))
	{
	  Entry.Remarks = (char *)realloc(Entry.Remarks,sizeof(Line));
	  strcat(Entry.Remarks,Line);
	  fgets(Line,MY_BUFSIZ,RoloFile);
	}
      
      Add_To_List(Entry);

      free(Entry.Name);
      free(Entry.WorkPhone);
      free(Entry.HomePhone);
      free(Entry.Company);
      free(Entry.WorkAddress);
      free(Entry.HomeAddress);
      free(Entry.DateUpdated);
      free(Entry.Remarks);
      fgets(Line,MY_BUFSIZ,RoloFile);
    }
  fclose (RoloFile);
  return(RETURN_OK);
}

/********************

   Convert_AddressBook_To_1_1-- Convert XmAddressBook 1.0.X to 1.1 XmAddressBook file. Sorry! 

*********************/

ReturnType Convert_AddressBook_To_1_1(char *SrcFilename, 
				      char *DestFilename)
{
  FILE *SrcRoloFile;
  FILE *DestRoloFile;
  char  Line[MY_BUFSIZ+1];
  struct Rolo_Struct Entry;
  
  if ((SrcRoloFile = fopen(SrcFilename,"r")) == NULL)
    {
      perror(SrcFilename);
      return(RETURN_NOTOK);
    }
  if ((DestRoloFile = fopen(DestFilename,"w")) == NULL)
    {
      perror(DestFilename);
      return(RETURN_NOTOK);
    }
  fprintf(DestRoloFile,"#RS1.1\n");
  
  fgets(Line,MY_BUFSIZ,SrcRoloFile);
  if (Line != NULL)
    {
      Remove_CR(Line);
      if (strcmp(Line,"#RS") != 0)
	{
	  fprintf(stderr,"ERROR, %s is not a XmAddressBook 1.0.X file, try using the -convert option.\n",
		  SrcFilename);
	  exit(-1);
	}
      if (!feof(SrcRoloFile)) fgets(Line,MY_BUFSIZ,SrcRoloFile);
    }
  while (!feof(SrcRoloFile))
    {
      Remove_CR(Line);
      Entry.Name = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      Entry.WorkPhone = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      Entry.HomePhone = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      Entry.Company = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      Entry.WorkAddress = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      Entry.HomeAddress = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Remove_CR(Line);
      Entry.DateUpdated = strdup(Line);

      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      Entry.Remarks = strdup(Line);
    
      fgets(Line,MY_BUFSIZ,SrcRoloFile);
      while (!feof(SrcRoloFile) && (strcmp(Line,"#RE\n") != 0))
	{
	  Entry.Remarks = (char *)realloc(Entry.Remarks,sizeof(Line));
	  strcat(Entry.Remarks,Line);
	  fgets(Line,MY_BUFSIZ,SrcRoloFile);
	}

      fprintf(DestRoloFile,"%s\n",Entry.Name);
      fprintf(DestRoloFile,"%s\n",Entry.WorkPhone);
      fprintf(DestRoloFile,"%s\n",Entry.HomePhone);
      fprintf(DestRoloFile,"%s\n",Entry.Company);
      fprintf(DestRoloFile,"%s\n",Entry.WorkAddress);
      fprintf(DestRoloFile,"%s\n",Entry.HomeAddress);
      fprintf(DestRoloFile,"\n");
      fprintf(DestRoloFile,"\n");
      fprintf(DestRoloFile,"%s\n",Entry.DateUpdated);
      fprintf(DestRoloFile,"%s\n#RE\n",Entry.Remarks);

      free(Entry.Name);
      free(Entry.WorkPhone);
      free(Entry.HomePhone);
      free(Entry.Company);
      free(Entry.WorkAddress);
      free(Entry.HomeAddress);
      free(Entry.DateUpdated);
      free(Entry.Remarks);
      fgets(Line,MY_BUFSIZ,SrcRoloFile);
    }
  fclose (SrcRoloFile);
  fclose (DestRoloFile);
  return(RETURN_OK);
}

/********************

   Read_AddressBook -- Read new xmaddressbook formatted file, i.e. a converted rolo file.

*********************/

ReturnType Read_AddressBook(char *Filename)
{
  FILE *RoloFile;
  char  Line[MY_BUFSIZ+1];
  struct Rolo_Struct Entry;

  if ((RoloFile = fopen(Filename,"r")) == NULL)
    {
      sprintf(Line,"File Error:\n\nCannot open address book file %s for reading.\n",
	      Filename);
      ErrorMessage(Line);
      return(RETURN_NOTOK);
    }
  fgets(Line,MY_BUFSIZ,RoloFile);
  if (Line != NULL)
    {
      Remove_CR(Line);
      if (strcmp(Line,"#RS1.1") != 0)
	{
	  if (G_ASCII_Mode == True)
	    {
	      fprintf(stderr,"ERROR, %s is not a XmAddressBook 1.1 file, try using the -convert option.\n",
		      Filename);
	      exit (-1);
	    }
	  else {
	    sprintf(Line,"File Error:\n\n%s is not a XmAddressBook 1.1 file, try using the -convert option.\n",
		    Filename);
	    ErrorMessage(Line);
	  }
	  return(RETURN_NOTOK);
	}
      if (!feof(RoloFile)) fgets(Line,MY_BUFSIZ,RoloFile);
    }
  while (!feof(RoloFile))
    {
      Remove_CR(Line);
      Entry.Name = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.WorkPhone = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.HomePhone = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.Company = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.WorkAddress = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.HomeAddress = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.EmailAddress = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.FaxNumber = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Remove_CR(Line);
      Entry.DateUpdated = strdup(Line);

      fgets(Line,MY_BUFSIZ,RoloFile);
      Entry.Remarks = strdup(Line);
    
      fgets(Line,MY_BUFSIZ,RoloFile);
      while (!feof(RoloFile) && (strcmp(Line,"#RE\n") != 0))
	{
	  if (Line[0] != '\n')
	    {
	      Entry.Remarks = (char *)realloc(Entry.Remarks,sizeof(Line));
	      strcat(Entry.Remarks,Line);
	    }
	  fgets(Line,MY_BUFSIZ,RoloFile);
	}

/*      
      fprintf(stdout,"Name: %s\n",Entry.Name);
      fprintf(stdout,"WorkPhone: %s\n",Entry.WorkPhone);
      fprintf(stdout,"HomePhone: %s\n",Entry.HomePhone);
      fprintf(stdout,"Company: %s\n",Entry.Company);
      fprintf(stdout,"Fax Number: %s\n",Entry.FaxNumber);
      fprintf(stdout,"Email Address: %s\n",Entry.EmailAddress);
      fprintf(stdout,"WorkAddress: %s\n",Entry.WorkAddress);
      fprintf(stdout,"HomeAddress: %s\n",Entry.HomeAddress);
      fprintf(stdout,"DateUpdated: %s\n",Entry.DateUpdated);
      fprintf(stdout,"Remarks: %s\n",Entry.Remarks);
      fprintf(stdout,"---------------------------------------------\n");
*/

      Add_To_List(Entry);

      free(Entry.Name);
      free(Entry.WorkPhone);
      free(Entry.HomePhone);
      free(Entry.Company);
      free(Entry.FaxNumber);
      free(Entry.EmailAddress);
      free(Entry.WorkAddress);
      free(Entry.HomeAddress);
      free(Entry.DateUpdated);
      free(Entry.Remarks);
      fgets(Line,MY_BUFSIZ,RoloFile);
    }
  fclose (RoloFile);
  return(RETURN_OK);
}

/********************

  Write_Addressbook

*********************/

ReturnType Write_AddressBook(char *Filename)
{
  FILE *RoloFile;
  Rolo_Type *Temp;
  char Buffer[MY_BUFSIZ+1];

  if (G_RoloList == NULL)
    {
      InfoMessage("Address book empty, nothing to save.\n");
      return(RETURN_OK);
    }
  if ((RoloFile = fopen(Filename,"w")) == NULL)
    {
      sprintf(Buffer,"File Error:\n\nCould not write address book file %s",Filename);
      ErrorMessage(Buffer);
      return(RETURN_NOTOK);
    }
  Temp = G_RoloList;
  fprintf(RoloFile,"#RS1.1\n");
  while (Temp != NULL)
    {
      fprintf(RoloFile,"%s\n",Temp->Name);
      fprintf(RoloFile,"%s\n",Temp->WorkPhone);
      fprintf(RoloFile,"%s\n",Temp->HomePhone);
      fprintf(RoloFile,"%s\n",Temp->Company);
      fprintf(RoloFile,"%s\n",Temp->WorkAddress);
      fprintf(RoloFile,"%s\n",Temp->HomeAddress);
      fprintf(RoloFile,"%s\n",Temp->EmailAddress);
      fprintf(RoloFile,"%s\n",Temp->FaxNumber);
      fprintf(RoloFile,"%s\n",Temp->DateUpdated);
      fprintf(RoloFile,"%s\n#RE\n",Temp->Remarks);
      Temp = Temp->Next;
    }
  fclose(RoloFile);
  G_ChangedNotSaved = False;
  return(RETURN_OK);
}

/********************

  Read_Help_File

*********************/

ReturnType Read_Help_File()
{
  FILE *HelpFile;
  struct stat HelpFileStat;
  char Buffer[MY_BUFSIZ+1];

  if ((HelpFile = fopen(HELP_FILE,"r")) == NULL)
    {
      sprintf(Buffer,"File Error:\n\nCould not open help file %s",HELP_FILE);
      ErrorMessage(Buffer);
      return (RETURN_NOTOK);
    }
  if (stat(HELP_FILE,&HelpFileStat) != 0)
    {
      sprintf(Buffer,"File Error:\n\nCould not stat help file %s",HELP_FILE);
      ErrorMessage(Buffer);
      return (RETURN_NOTOK);
    }
  G_HelpString = (char *)malloc(HelpFileStat.st_size+1);
  read(fileno(HelpFile),G_HelpString,HelpFileStat.st_size-1);
  G_HelpString[HelpFileStat.st_size+1] = '\0';
  G_HelpString[HelpFileStat.st_size] = '\0';
  fclose(HelpFile);
/*   fprintf(stderr,"*%s*#%d#\n",G_HelpString,strlen(G_HelpString)); */
  return (RETURN_OK);
}

/********************

  Read_Signature_File

*********************/

ReturnType Read_Signature_File()
{
  FILE *SignatureFile;
  struct stat SignatureFileStat;
/*   char Buffer[MY_BUFSIZ+1]; */

  if ((SignatureFile = fopen(G_Signature_Filename,"r")) == NULL)
    {
/*       sprintf(Buffer,"File Error:\n\nCould not open Signature file %s",G_Signature_Filename); */
/*       ErrorMessage(Buffer); */
      return (RETURN_OK);
    }
  if (stat(G_Signature_Filename,&SignatureFileStat) != 0)
    {
/*       sprintf(Buffer,"File Error:\n\nCould not stat Signature file %s",G_Signature_Filename); */
/*       ErrorMessage(Buffer); */
      return (RETURN_OK);
    }
  G_Signature_String = (char *)malloc(SignatureFileStat.st_size+1);
  read(fileno(SignatureFile),G_Signature_String,SignatureFileStat.st_size-1);
  G_Signature_String[SignatureFileStat.st_size+1] = '\0';
  G_Signature_String[SignatureFileStat.st_size] = '\0';
  fclose(SignatureFile);
/*   fprintf(stderr,"*%s*#%d#\n",G_Signature_String,strlen(G_Signature_String)); */
  return (RETURN_OK);
}

void OK_OpenAddressBook(Widget   w,
			Widget   PopupWidget, 
			XmFileSelectionBoxCallbackStruct *cbs)
{
  int lcv = 0;
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
  free(G_Addressbook_Filename);
  G_Addressbook_Filename = strdup(Filename);
  for (lcv = G_NumInRoloList;lcv != 0;lcv--)
    {
      Delete_From_List(lcv);
    }
  Clear_RoloForm();
  Read_AddressBook(G_Addressbook_Filename);
  if (G_RoloList != NULL)
    {
      G_CurrentNumInRoloList = 1;
      G_CurrentEntryInRoloList = G_RoloList;
    }
  CurrentEntry_RoloForm();
  XtUnrealizeWidget(w);
  XtFree(Filename);
  return;  
}

void CANCEL_OpenAddressbook(Widget   w,
			Widget   PopupWidget, 
			XmAnyCallbackStruct *cbs)
{
  XtUnrealizeWidget(w);
  return;
}

void  Open_AddressBook(Widget   w,
		       Widget   textwidget,   
		       XmAnyCallbackStruct *call_value)
{
  G_OpenAddressbook = True;
  if (G_ChangedNotSaved == True)
    {
      SaveDontExit(NULL,NULL,NULL);
    }
  else {
    Open_AddressBook_Dialog();
  }
}

void Open_AddressBook_Dialog()
{
  XmString DirString;
  Widget W_FileDialog;

  G_OpenAddressbook = False;
  W_FileDialog = (Widget) XmCreateFileSelectionDialog(W_TopLevel,"FileSelector",NULL,0);
  XtAddCallback(W_FileDialog, XmNcancelCallback, (XtCallbackProc) CANCEL_OpenAddressbook,NULL);
  XtAddCallback(W_FileDialog, XmNokCallback, (XtCallbackProc) OK_OpenAddressBook,NULL);
  XtUnmanageChild((Widget )XmFileSelectionBoxGetChild(W_FileDialog, XmDIALOG_HELP_BUTTON));
  XtVaSetValues(XtParent(W_FileDialog),
		XmNtitle,"XmAddressBook File Selector",
		NULL,0);
  DirString = XmStringCreate((char *)getenv("HOME"), XmFONTLIST_DEFAULT_TAG),
  XtVaSetValues(W_FileDialog,
		XmNtextColumns, 60,
		XmNdirectory, DirString,
		NULL,0);
  XtManageChild(W_FileDialog);
}
