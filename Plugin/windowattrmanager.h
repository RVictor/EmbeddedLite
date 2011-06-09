/**
  \file 

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef __windowattrmanager__
#define __windowattrmanager__

#include <wx/window.h>
#include "iconfigtool.h"

/**
 * @class WindowAttrManager
 * @author Eran
 * @date 09/30/08
 * @file windowattrmanager.h
 * @brief a helper class that helps restoring wxWindow size and position
 */
class WindowAttrManager {
public:
	/**
	 * @brief write windows' size and position to the disk
	 * @param win window to work on
	 * @param name the windows' name
	 * @param conf configuration tool to use. If set to NULL the default EditorConfigST will be used
	 */
	static void Save(wxWindow *win, const wxString &name, IConfigTool *conf);
	/**
	 * @brief load windows' size and position from the disk
	 * @param win window to work on
	 * @param name the windows' name
	 * @param conf conf configuration tool to use. If set to NULL the default EditorConfigST will be used
	 */
	static void Load(wxWindow *win, const wxString &name, IConfigTool *conf);
};
#endif // __windowattrmanager__
