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
#ifndef CONFIGTOOL_H
#define CONFIGTOOL_H

#include "wx/xml/xml.h"

class SerializedObject;

/**
 * \class ConfigTool
 * \brief this class provides a simple class that allows writing serialized classes to disk
 * \author Eran
 * \date 11/18/07
 */
class ConfigTool
{
	wxXmlDocument m_doc;
	wxString m_fileName;

public:
	ConfigTool();
	~ConfigTool();

	bool Load(const wxString &basename, const wxString &version);
	bool WriteObject(const wxString &name, SerializedObject *obj);
	bool ReadObject(const wxString &name, SerializedObject *obj);
};

#endif //CONFIGTOOL_H

