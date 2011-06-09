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
#ifndef __configurationtoolbase__
#define __configurationtoolbase__

#include "wx/xml/xml.h"

class SerializedObject;

class ConfigurationToolBase {
protected:
	wxXmlDocument m_doc;
	wxString m_fileName;
	
public:
	ConfigurationToolBase();
	virtual ~ConfigurationToolBase();
	
	bool Load(const wxString &filename);
	bool WriteObject(const wxString &name, SerializedObject *obj);
	bool ReadObject(const wxString &name, SerializedObject *obj);
	
	virtual wxString GetRootName() = 0;
};
#endif // __configurationtoolbase__

