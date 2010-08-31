/**
  \file ikeyboard.h              

  \brief EmbeddedLite (CodeLite) file
  \author Eran Ifrah, V. Ridtchenko

  \notes

  Copyright: (C) 2008 by Eran Ifrah, 2010 Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef __IKEYBOARD_H__
#define __IKEYBOARD_H__

#include <wx/string.h>
#include <map>
#include <wx/window.h>

/**
 * @class MenuItemData
 * @author Eran
 * @date 09/15/08
 * @file ikeyboard.h
 * @brief
 */
class MenuItemData
{
public:
	wxString id;
	wxString parent;
	wxString action;
	wxString accel;
};

typedef std::map< wxString, MenuItemData > MenuItemDataMap;

/**
 * @class IKeyboard
 * @author Eran
 * @date 09/15/08
 * @file ikeyboard.h
 * @brief an interface to the keyboard layer of EmbeddedLite
 */
class IKeyboard
{
public:
	IKeyboard() {}
	virtual ~IKeyboard() {}

	/**
	 * @brief popup the 'keyboard shortcut' dialog
	 * @param parent parent for the dialog
	 * @param menuItemData the dialog initialization data
	 * @return the dialog return code from the ShowModal() call
	 */
	virtual int PopupNewKeyboardShortcutDlg(wxWindow *parent, MenuItemData& menuItemData) = 0;

	/**
	 * @brief return map of all accelerators sets for EmbeddedLite. For historic reasons, the key
	 * for the map is the action
	 * @param accelMap [output]
	 */
	virtual void GetAccelerators(MenuItemDataMap &accelMap) = 0;

	/**
	 * @brief
	 * @param accelMap
	 * @param accelerator
	 * @return
	 */
	virtual bool IsDuplicate(const MenuItemDataMap &accelMap, const wxString &accelerator) = 0;

	/**
	 * @brief add new accelerator. an Update() call should be invoked for the changes take place
	 * @param accelMap The loaded accelerator map, from previous call to IKeyboard::GetAccelerators()
	 * @param cmdId Command ID
	 * @param parent Menu parent, e.g. "Plugins"
	 * @param action Menu Item entry: "Build Workspace"
	 * @param accel Keyboard accelerator "Ctrl-Shift-1"
	 * @return true on success, false otherwise
	 */
	virtual bool AddAccelerator(MenuItemDataMap &accelMap, const MenuItemData& menuItemData) = 0;
	
	/**
	 * @brief set new accelerator map
	 * @param accelMap accelerator map
	 */
	virtual void Update(const MenuItemDataMap &accelMap) = 0;
};
#endif // __IKEYBOARD_H__
