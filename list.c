#include "main.h"

ReturnType Add_To_List(struct Rolo_Struct AddEntry)
{
  Rolo_Type *Temp, *NewEntry, *Prev;
  int Counter;

  if (G_RoloList == NULL)
    {
      if ((G_RoloList = (Rolo_Type *)malloc(sizeof(Rolo_Type))) == NULL)
	{
	  return (RETURN_NOTOK);
	}
/*        fprintf(stderr,"add %s to front of list\n", AddEntry.Name);   */
      G_RoloList->Name  = strdup(AddEntry.Name);
      G_RoloList->WorkPhone  = strdup(AddEntry.WorkPhone);
      G_RoloList->HomePhone  = strdup(AddEntry.HomePhone);
      G_RoloList->Company  = strdup(AddEntry.Company);
      G_RoloList->EmailAddress  = strdup(AddEntry.EmailAddress);
      G_RoloList->FaxNumber = strdup(AddEntry.FaxNumber);
      G_RoloList->WorkAddress  = strdup(AddEntry.WorkAddress);
      G_RoloList->HomeAddress  = strdup(AddEntry.HomeAddress);
      G_RoloList->DateUpdated  = strdup(AddEntry.DateUpdated );
      G_RoloList->Remarks  = strdup(AddEntry.Remarks );
      G_RoloList->Next  = NULL;
      G_NumInRoloList = 1;
      G_CurrentNumInRoloList = 1;
      G_CurrentEntryInRoloList = G_RoloList;
    }
  else {
    Temp = G_RoloList;
    Prev = NULL;
    Counter = 0;
    while (Temp != NULL)
      {
	Counter++;
	if (strcmp(AddEntry.Name,Temp->Name) < 0)
	  {
	    if ((NewEntry = (Rolo_Type *)malloc(sizeof(Rolo_Type))) == NULL)
	      {
		return (RETURN_NOTOK);
	      }
/*  	    fprintf(stderr,"add %s in front of %s\n", AddEntry.Name,Temp->Name);  */
	    NewEntry->Name  = strdup(AddEntry.Name);
	    NewEntry->WorkPhone  = strdup(AddEntry.WorkPhone);
	    NewEntry->HomePhone  = strdup(AddEntry.HomePhone);
	    NewEntry->Company  = strdup(AddEntry.Company);
	    NewEntry->EmailAddress  = strdup(AddEntry.EmailAddress);
	    NewEntry->FaxNumber = strdup(AddEntry.FaxNumber);
	    NewEntry->WorkAddress  = strdup(AddEntry.WorkAddress);
	    NewEntry->HomeAddress  = strdup(AddEntry.HomeAddress);
	    NewEntry->DateUpdated  = strdup(AddEntry.DateUpdated );
	    NewEntry->Remarks  = strdup(AddEntry.Remarks );
	    NewEntry->Next  = Temp;
	    if (Prev == NULL)
	      {
		G_RoloList = NewEntry;
		G_CurrentNumInRoloList = 1;
	      }
	    else {
	      Prev->Next = NewEntry;
	      G_CurrentNumInRoloList = Counter;
	    }
	    G_CurrentEntryInRoloList = NewEntry;
	    G_NumInRoloList++;
	    return (RETURN_OK);
	  }
	Prev = Temp;
	Temp = Temp->Next;
      }
    if ((NewEntry = (Rolo_Type *)malloc(sizeof(Rolo_Type))) == NULL)
      {
	return (RETURN_NOTOK);
      }
/*     fprintf(stderr,"add %s after of %s\n", AddEntry.Name,Prev->Name);  */
    NewEntry->Name  = strdup(AddEntry.Name);
    NewEntry->WorkPhone  = strdup(AddEntry.WorkPhone);
    NewEntry->HomePhone  = strdup(AddEntry.HomePhone);
    NewEntry->Company  = strdup(AddEntry.Company);
    NewEntry->EmailAddress  = strdup(AddEntry.EmailAddress);
    NewEntry->FaxNumber = strdup(AddEntry.FaxNumber);
    NewEntry->WorkAddress  = strdup(AddEntry.WorkAddress);
    NewEntry->HomeAddress  = strdup(AddEntry.HomeAddress);
    NewEntry->DateUpdated  = strdup(AddEntry.DateUpdated );
    NewEntry->Remarks  = strdup(AddEntry.Remarks );
    NewEntry->Next  = NULL;
    Prev->Next = NewEntry;
    G_CurrentNumInRoloList = ++Counter;
    G_CurrentEntryInRoloList = NewEntry;
    G_NumInRoloList++;
  }
  return (RETURN_OK);
}

ReturnType Delete_From_List(int DeleteNum)
{
  Rolo_Type *Temp, *Prev;
  int Counter;

  if (G_RoloList == NULL)
    {
      G_NumInRoloList = 0;
      return (RETURN_OK);
    }
  else {
    Temp = G_RoloList;
    Prev = NULL;
    Counter = 1;
    while ((Temp != NULL) && (Counter < DeleteNum))
      {
	Prev = Temp;
	Temp = Temp->Next;
	Counter++;
      }
    if (Counter == 1)
      {
        G_CurrentNumInRoloList = 1;
	G_CurrentEntryInRoloList = Temp->Next;
	G_RoloList = Temp->Next;
	free(Temp->Name);
	free(Temp->WorkPhone);
	free(Temp->HomePhone);
	free(Temp->Company);
	free(Temp->FaxNumber);
	free(Temp->EmailAddress);
	free(Temp->WorkAddress);
	free(Temp->HomeAddress);
	free(Temp->DateUpdated);
	free(Temp->Remarks);
	free(Temp);
	G_NumInRoloList--;
      }
    else if (Temp != NULL)
      {
        G_CurrentNumInRoloList = Counter;
	G_CurrentEntryInRoloList = Temp->Next;
	Prev->Next = Temp->Next;
	free(Temp->Name);
	free(Temp->WorkPhone);
	free(Temp->HomePhone);
	free(Temp->Company);
	free(Temp->FaxNumber);
	free(Temp->EmailAddress);
	free(Temp->WorkAddress);
	free(Temp->HomeAddress);
	free(Temp->DateUpdated);
	free(Temp->Remarks);
	free(Temp);
	G_NumInRoloList--;
      }
  return (RETURN_OK);
  }
}

ReturnType Print_List()
{
  Rolo_Type *Temp;
  int Counter;

  if (G_RoloList == NULL)
    {
      fprintf(stderr,"Rolo list empty\n");
    }
  else {
    Temp = G_RoloList;
    Counter = 0;
    while (Temp != NULL)
      {
	Counter++;
	fprintf(stderr,"Name: %s\n",Temp->Name);
	fprintf(stderr,"WorkPhone: %s\n",Temp->WorkPhone);
	fprintf(stderr,"HomePhone: %s\n",Temp->HomePhone);
	fprintf(stderr,"Company: %s\n",Temp->Company);
	fprintf(stderr,"Fax Number: %s\n",Temp->FaxNumber);
	fprintf(stderr,"Email Address: %s\n",Temp->EmailAddress);
	fprintf(stderr,"WorkAddress: %s\n",Temp->WorkAddress);
	fprintf(stderr,"HomeAddress: %s\n",Temp->HomeAddress);
	fprintf(stderr,"DateUpdated: %s\n",Temp->DateUpdated);
	fprintf(stderr,"Remarks: %s\n",Temp->Remarks);
	Temp = Temp->Next;
      }
    fprintf(stderr,"Number in list: %d\n",Counter);
  }
  return (RETURN_OK);
}


ReturnType Update_Entry(struct Rolo_Struct UpdateEntry,
			int CurrentNum)
{
  Rolo_Type *Temp;
  int Counter;

  if (G_RoloList == NULL)
    {
      return (RETURN_OK);
    }
  else {
    Temp = G_RoloList;
    Counter = 1;
    while ((Temp != NULL) && (Counter < CurrentNum))
      {
	Temp = Temp->Next;
	Counter++;
      }
    if (Temp != NULL)
      {
	Temp->Name  = strdup(UpdateEntry.Name);
	Temp->WorkPhone  = strdup(UpdateEntry.WorkPhone);
	Temp->HomePhone  = strdup(UpdateEntry.HomePhone);
	Temp->Company  = strdup(UpdateEntry.Company);
	Temp->EmailAddress  = strdup(UpdateEntry.EmailAddress);
	Temp->FaxNumber  = strdup(UpdateEntry.FaxNumber);
	Temp->WorkAddress  = strdup(UpdateEntry.WorkAddress);
	Temp->HomeAddress  = strdup(UpdateEntry.HomeAddress);
	Temp->DateUpdated  = strdup(UpdateEntry.DateUpdated);
	Temp->Remarks  = strdup(UpdateEntry.Remarks);
      }
  }
  return (RETURN_OK);
}






