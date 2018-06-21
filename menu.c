#include "main.h"
/* Written by Dan Heller.  Copyright 1991, O'Reilly && Associates. 
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* Pulldown menus are built from cascade buttons, so this function
 * also includes pullright menus.  Create the menu, the cascade button
 * that owns the menu, and then the submenu items.
 */
Widget BuildPulldownMenu(Widget parent,
			 char *menu_title, 
			 char menu_mnemonic,
			 MenuItem *items,
			 Bool IsHelpMenu)
{
  Widget PullDown, cascade, widget;
  int i;
  XmString str;
  
  PullDown = XmCreatePulldownMenu(parent, "_pulldown", NULL, 0);


/*
  XtVaSetValues(PullDown,
		XmNtearOffModel, XmTEAR_OFF_ENABLED,
		NULL);
*/  
  
  str = XmStringCreateSimple(menu_title);
  cascade = XtVaCreateManagedWidget(menu_title,
				    xmCascadeButtonGadgetClass, parent,
				    XmNsubMenuId,   PullDown,
				    XmNlabelString, str,
				    XmNmnemonic,    menu_mnemonic,
				    NULL);
  XmStringFree(str);
  
  /* if its the help menu, tell menubar to always right-justify it */
  
  if (IsHelpMenu == True)
    {
      XtVaSetValues(parent, XmNmenuHelpWidget, cascade, NULL,0);
    }
  
    /* Now add the menu items */
  for (i = 0; items[i].label != NULL; i++) {
    /* If subitems exist, create the pull-right menu by calling this
     * function recursively.  Since the function returns a cascade
     * button, the widget returned is used..
     */
    if (items[i].subitems)
      {
	widget = BuildPulldownMenu(PullDown,
				   items[i].label, items[i].mnemonic, items[i].subitems, False);
      }
    else
      {
	widget = XtVaCreateManagedWidget(items[i].label,
					 *items[i].class, PullDown,
					 NULL);
      }
    /* Whether the item is a real item or a cascade button with a
     * menu, it can still have a mnemonic.
     */
    if (items[i].mnemonic != '\0');
      {
	XtVaSetValues(widget, XmNmnemonic, items[i].mnemonic, NULL);
      }
    /* any item can have an accelerator, except cascade menus. But,
     * we don't worry about that; we know better in our declarations.
     */
    if (items[i].accelerator) {
      str = XmStringCreateSimple(items[i].accel_text);
      XtVaSetValues(widget,
		    XmNaccelerator, items[i].accelerator,
		    XmNacceleratorText, str,
		    NULL);
      XmStringFree(str);
    }
    /* again, anyone can have a callback */
    if (items[i].callback)
      {
	XtAddCallback(widget, items[i].callbacktype,
		      items[i].callback, items[i].callback_data);
      } 
  }
  return cascade;
}
