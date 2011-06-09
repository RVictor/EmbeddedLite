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
#include "build_system.h"
#include "xmlutils.h"
#include "wx_xml_compatibility.h"

BuilderConfig::BuilderConfig(wxXmlNode *node) 
: m_isActive(false)
{
	if(node) {
		m_name        = XmlUtils::ReadString(node, wxT("Name"));
		m_toolPath    = XmlUtils::ReadString(node, wxT("ToolPath"));
		m_toolOptions = XmlUtils::ReadString(node, wxT("Options"));
		m_toolJobs    = XmlUtils::ReadString(node, wxT("Jobs"), wxT("1"));
		m_isActive    = XmlUtils::ReadBool  (node, wxT("Active"), m_isActive);
	}
}

BuilderConfig::~BuilderConfig()
{
}

wxXmlNode *BuilderConfig::ToXml() const
{
	wxXmlNode *node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("BuildSystem"));
	node->AddProperty(wxT("Name"),     m_name);
	node->AddProperty(wxT("ToolPath"), m_toolPath);
	node->AddProperty(wxT("Options"),  m_toolOptions);
	node->AddProperty(wxT("Jobs"),     m_toolJobs);
	node->AddProperty(wxT("Active"),   m_isActive ? wxString(wxT("yes")) : wxString(wxT("no")));
	return node;
}
