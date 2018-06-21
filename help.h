typedef struct {
  char *Key;
  char *Value;
} Help_Type;


Help_Type Help_List[] = {{"prev","Goto Previous Entry"},
			   {"next","Goto Next Entry"},
			   {"search","Case In-sensitive search of Address Book"},
			   {"goto","Goto a specific entry" },
			   {"add","Add an entry" },
			   {"delete","Delete an entry" },
			   {"update","Update an entry" },
			   {"clear","Clear entry form, must clear first in order to use ADD" },
			   {"name","Name field {lastname, firstname}" },
			   {"wphone","Work phone number" },
			   {"hphone","Home phone number" },
			   {"company","Company or Organization" },
			   {"waddress","Work Address" },
			   {"haddress","Home Address" },
			   {"date","Date entry last updated (non-editable)" },
			   {"remarks","Remarks or descriptive information" },
			   {"current","The current entry number" },
			   {"total","The total number of entries" },
			   {"fax","Facsimile (Fax) number " },
			   {"email","Electronic Mail (Email) Adress" },
			   {"duplicate","Duplicate an entry n times"},
			   {"blank",""},
			   {NULL,NULL },
		       };
			 
