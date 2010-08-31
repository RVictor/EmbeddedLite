/**
  \file iconfigtool.h              

  \brief EmbeddedLite (CodeLite) file
  \author Eran Ifrah, V. Ridtchenko

  \notes

  Copyright: (C) 2008 by Eran Ifrah, 2010 Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef ICONFIGTOOL_H
#define ICONFIGTOOL_H

#include "wx/string.h"
#include "serialized_object.h"

//------------------------------------------------------------------
// The configuration tool interface
//------------------------------------------------------------------
/**
 * @class IConfigTool
 * @author Eran
 * @date 05/07/08
 * @file iconfigtool.h
 * @brief a configuration tool that allow plugin easy access to EmbeddedLite's configuration file. Plugin should only read/write its own
 * values
 * @sa SerializedObject
 */
class IConfigTool {
public:
	IConfigTool(){}
	virtual ~IConfigTool(){}
	
	/**
	 * @brief read an object from the configuration file and place it into obj
	 * @param name the name of the object that is stored in the configuration file
	 * @param obj a pointer previsouly allocated SerializedObject
	 * @return true if the object exist and was read successfully from the configuration file
	 */
	virtual bool ReadObject(const wxString &name, SerializedObject *obj) = 0;
	
	/**
	 * @brief write an object to the configuration file
	 * @param name the name that identifies the object
	 * @param obj object to store.
	 * @return true on success, false otherwise
	 */
	virtual bool WriteObject(const wxString &name, SerializedObject *obj) = 0;
};

#endif //ICONFIGTOOL_H
