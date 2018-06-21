#define MAIN_H

#include <stdio.h>
#ifdef BSDI
#include <sys/types.h>
#endif
#include <sys/stat.h>

#include <X11/X.h>
#include <X11/Intrinsic.h>
#include <X11/cursorfont.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/CascadeBG.h>
#include <Xm/ArrowB.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <Xm/List.h>
#include <Xm/Label.h>
#include <Xm/Text.h>
#include <Xm/Separator.h>
#include <Xm/SeparatoG.h>
#include <Xm/Frame.h>
#include <Xm/BulletinB.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/FileSB.h>
#include <Xm/MessageB.h>
#include <Xm/Protocols.h>
#include <Xm/DialogS.h>
#include <Xm/AtomMgr.h>

#include "general.h"

/*******************

  User-Customizable Defines

********************/

#ifdef HPUX
#define DEFAULT_PRINT_COMMAND     "lpr -dps1 "
#elif defined(VMS)
#define DEFAULT_PRINT_COMMAND      "print"
#else
#define DEFAULT_PRINT_COMMAND     "/usr/bin/lpr "
#endif

#ifndef VMS
#define SENDMAIL_COMMAND          "/usr/lib/sendmail -t "
#else
#define SENDMAIL_COMMAND          "mail"
#endif

#define DEFAULT_PRINT_FILENAME    "xmaddressbook.output"
#ifndef VMS
#define DEFAULT_ADDRESSBOOK_FILENAME  ".xmaddressbook.dat"
#else
#define DEFAULT_ADDRESSBOOK_FILENAME  "xmaddressbook.dat"
#endif
#define DEFAULT_OLD_ADDRESSBOOK_FILENAME  ".rolodex.dat"  

#if !defined(HELP_FILE)
#ifndef VMS
#define HELP_FILE                 "xmaddressbook.hlp"
#else
#define HELP_FILE                 "public$Root:[xtools.xutil.xmaddressbook]xmaddressbook.hlp"
#endif
#endif

#ifndef VMS
#define SIGNATURE_FILE            ".signature"
#else
#define SIGNATURE_FILE            "sys$login:mail.signature"
#endif


#define SIGNATURE_FILE            ".signature"

/*******************

   Defines

********************/

#define TEMP_FILENAME_EXTENSION   ".tmp"
#define PROGRAM_TITLE             "XmAddressBook Version 1.5.3"
#define PROGRAM_DESCRIPTION       "XmAddressBook\n\nA Motif Address Book\n\nVersion 1.5.3 \n\nCopyright 1994,1995,1996 by Boyd Fletcher and Old Dominion University\n\nSend problems to: boyd@ccpo.odu.edu"
#define RETURN_NOTOK 0
#define RETURN_OK    1
#define MY_BUFSIZ        1024
#define MY_BIG_BUFSIZ    4096

/******************

   Types

*******************/

typedef int ReturnType;

typedef struct _menu_item {
    char        *label;         /* the label for the item */
    WidgetClass *class;         /* pushbutton, label, separator... */
    String       callbacktype;
    char         mnemonic;      /* mnemonic; NULL if none */
    char        *accelerator;   /* accelerator; NULL if none */
    char        *accel_text;    /* to be converted to compound string */
    void       (*callback)();   /* routine to call; NULL if none */
    XtPointer    callback_data; /* client_data for callback() */
    struct _menu_item *subitems; /* pullright menu items, if not NULL */
} MenuItem;

typedef struct Rolo_Struct {
  char *Name;
  char *WorkPhone;
  char *HomePhone;
  char *FaxNumber;
  char *EmailAddress;
  char *Company;
  char *WorkAddress;
  char *HomeAddress;
  char *DateUpdated;
  char *Remarks;
  struct Rolo_Struct *Next;
} Rolo_Type;

/*

   Global Variables

*/

#include "prototypes.h"

#ifndef MAIN
extern XmFontList G_MainFontList;
extern XFontStruct *G_MainFontStruct;
extern XmFontList G_BigFontList;
extern XFontStruct *G_BigFontStruct;
extern XmFontList G_FixedFontList;
extern XFontStruct *G_FixedFontStruct;
extern char *G_PrinterName;
extern Rolo_Type *G_RoloList;
extern int G_NumInRoloList;
extern int G_CurrentNumInRoloList;
extern Rolo_Type *G_CurrentEntryInRoloList;
extern char *G_HelpString;
extern char *G_Addressbook_Filename;
extern XtAppContext G_ApplicationContext;
extern Widget W_TopLevel;
extern Widget W_Main_Window, W_Main_Form, W_Main_Frame;
extern Widget W_MenuBar;
extern Bool G_ChangedNotSaved;
extern Bool G_OpenAddressbook;
extern Bool G_ASCII_Mode;
extern Bool G_DuplicateVisible;
extern Bool G_PrintVisible;
extern Bool G_SearchVisible;
extern Bool G_NameListVisible;
extern Bool G_GotoVisible;
extern Bool G_BCardVisible;
extern Widget NameList_List;
extern Bool G_CursesInterface;
extern Bool G_MailVisible;
extern Bool G_PreferencesVisible;
extern char *G_Signature_String;
extern char *G_Signature_Filename;
#else
XmFontList G_MainFontList;
XFontStruct *G_MainFontStruct;
XmFontList G_BigFontList;
XFontStruct *G_BigFontStruct;
XmFontList G_FixedFontList;
XFontStruct *G_FixedFontStruct;
char *G_PrinterName = NULL;
Rolo_Type *G_RoloList = NULL;
Rolo_Type *G_CurrentEntryInRoloList = NULL;
int G_CurrentNumInRoloList = 0;
int G_NumInRoloList = 0;
char *G_HelpFile;
char *G_HelpString = NULL;
char *G_Addressbook_Filename = NULL;
XtAppContext G_ApplicationContext;
Widget W_TopLevel;
Widget W_Main_Window, W_Main_Form, W_Main_Frame;
Widget W_MenuBar;
Bool G_ChangedNotSaved = False;
Bool G_OpenAddressbook = False;
Bool G_ASCII_Mode = False;
Bool G_DuplicateVisible = False;
Bool G_PrintVisible = False;
Bool G_SearchVisible = False;
Bool G_NameListVisible = False;
Bool G_GotoVisible = False;
Bool G_BCardVisible = False;
Bool G_MailVisible = False;
Bool G_PreferencesVisible = False;
Widget NameList_List;
Bool G_CursesInterface = False;
char *G_Signature_Filename = NULL;
char *G_Signature_String = NULL;
#endif



