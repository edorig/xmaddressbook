#ifndef PROTOTYPE_H
#include "prototypes.h"
#endif

#define MENU_H

MenuItem File_Menu[] = {
  { "Open Addressbook...", &xmPushButtonGadgetClass, XmNactivateCallback, 'O', NULL, NULL,
      Open_AddressBook, (XtPointer)1, (MenuItem *)NULL },  
  { "", &xmSeparatorGadgetClass, NULL, '\0', NULL, NULL,
      NULL, (XtPointer) 2, (MenuItem *)NULL },
  { "Print...", &xmPushButtonGadgetClass,XmNactivateCallback, 'P', NULL, NULL,
      Print_AddressBook, (XtPointer)3, (MenuItem *)NULL },
  { "", &xmSeparatorGadgetClass, NULL, '\0', NULL, NULL,
      NULL, (XtPointer) 4, (MenuItem *)NULL },
/*  { "Preferences...", &xmPushButtonGadgetClass,XmNactivateCallback, 'f', NULL, NULL,
      Show_Preferences, (XtPointer)5, (MenuItem *)NULL },
*/
  { "", &xmSeparatorGadgetClass, NULL, '\0', NULL, NULL,
      NULL, (XtPointer) 6, (MenuItem *)NULL },
  { "Save", &xmPushButtonGadgetClass, XmNactivateCallback, 'S', NULL, NULL,
      SaveDontExit, (XtPointer)7, (MenuItem *)NULL },
  { "Save & Exit", &xmPushButtonGadgetClass, XmNactivateCallback, 'x', NULL, NULL,
      SaveAndExit, (XtPointer)8, (MenuItem *)NULL },
  { "", &xmSeparatorGadgetClass, NULL, '\0', NULL, NULL,
      NULL, (XtPointer) 9, (MenuItem *)NULL },
  { "Exit", &xmPushButtonGadgetClass,XmNactivateCallback, 'E', NULL, NULL,
      Exit_Program, (XtPointer)10, (MenuItem *)NULL },
  NULL,
};

MenuItem Misc_Menu[] = {
  { "Duplicate an Entry", &xmPushButtonGadgetClass,XmNactivateCallback, 'D', NULL, NULL,
      Duplicate_Entry, (XtPointer)1, (MenuItem *)NULL },
  { "Search for an Entry", &xmPushButtonGadgetClass,XmNactivateCallback, 'S', NULL, NULL,
      Search_FromMenu, (XtPointer)2, (MenuItem *)NULL },
  { "Goto  an Entry", &xmPushButtonGadgetClass,XmNactivateCallback, 'G', NULL, NULL,
      Goto_Form, (XtPointer)3, (MenuItem *)NULL },
  { "", &xmSeparatorGadgetClass, NULL, '\0', NULL, NULL,
      NULL, (XtPointer) 4, (MenuItem *)NULL },
  { "Show Name List", &xmPushButtonGadgetClass,XmNactivateCallback, 'L', NULL, NULL,
      Show_Name_List, (XtPointer) 5, (MenuItem *)NULL },
  { "Show Business Card", &xmPushButtonGadgetClass,XmNactivateCallback, 'B', NULL, NULL,
      Show_BCard_FromMenu, (XtPointer) 6, (MenuItem *)NULL },
 { "", &xmSeparatorGadgetClass, NULL, '\0', NULL, NULL,
      NULL, (XtPointer) 7, (MenuItem *)NULL },
  { "Send Mail", &xmPushButtonGadgetClass,XmNactivateCallback, 'M', NULL, NULL,
      Send_Mail_Popup, (XtPointer)8, (MenuItem *)NULL },
  NULL,
};

MenuItem Help_Menu[] = {
  { "Help", &xmPushButtonGadgetClass,XmNactivateCallback, 'H', NULL, NULL,
      Display_Help, (XtPointer)1, (MenuItem *)NULL },
  { "Version", &xmPushButtonGadgetClass,XmNactivateCallback, 'V', NULL, NULL,
      Display_Version, (XtPointer)2, (MenuItem *)NULL },
  NULL,
};





