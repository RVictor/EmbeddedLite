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
#include "sessionmanager.h"
#include "xmlutils.h"
#include "wx/ffile.h"
#include <wx/log.h>

#include <memory>
#include "wx_xml_compatibility.h"

//Session entry
SessionEntry::SessionEntry()
{
}

SessionEntry::~SessionEntry()
{
}

void SessionEntry::DeSerialize(Archive &arch)
{
	arch.Read(wxT("m_selectedTab"), m_selectedTab);
	arch.Read(wxT("m_tabs"), m_tabs);
	arch.Read(wxT("m_workspaceName"), m_workspaceName);
	arch.Read(wxT("TabInfoArray"), m_vTabInfoArr);
	arch.Read(wxT("m_breakpoints"), (SerializedObject*)&m_breakpoints);
	// initialize tab info array from m_tabs if in config file wasn't yet tab info array
	if (m_vTabInfoArr.size() == 0 && m_tabs.GetCount() > 0) {
		for (size_t i=0; i<m_tabs.GetCount(); i++) {
			TabInfo oTabInfo;
			oTabInfo.SetFileName(m_tabs.Item(i));
			oTabInfo.SetFirstVisibleLine(0);
			oTabInfo.SetCurrentLine(0);
			m_vTabInfoArr.push_back(oTabInfo);
		}
	}
}

void SessionEntry::Serialize(Archive &arch)
{
	arch.Write(wxT("m_selectedTab"), m_selectedTab);
	// since tabs are saved in TabInfoArray we don't save tabs as string array,
	// there are only read due to read config saved in older version where wasn't TabInfoArray
	//arch.Write(wxT("m_tabs"), m_tabs);
	arch.Write(wxT("m_workspaceName"), m_workspaceName);
	arch.Write(wxT("TabInfoArray"), m_vTabInfoArr);
	arch.Write(wxT("m_breakpoints"), (SerializedObject*)&m_breakpoints);
}


//---------------------------------------------

void TabGroupEntry::DeSerialize(Archive &arch)
{
	arch.Read(wxT("m_TabgroupName"), m_tabgroupName);
	arch.Read(wxT("TabInfoArray"), m_vTabInfoArr);
}

void TabGroupEntry::Serialize(Archive &arch)
{
	arch.Write(wxT("m_TabgroupName"), m_tabgroupName);
	arch.Write(wxT("TabInfoArray"), m_vTabInfoArr);
}


//---------------------------------------------
SessionManager & SessionManager::Get()
{
	static SessionManager theManager;
	return theManager;
}

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

bool SessionManager::Load(const wxString &fileName)
{
	m_fileName = wxFileName(fileName);

	if (!m_fileName.FileExists()) {
		//no such file or directory
		//create an empty one
		wxFFile newFile(fileName, wxT("a+"));
		newFile.Write(wxT("<Sessions/>"));
		newFile.Close();
	}

	m_doc.Load(m_fileName.GetFullPath());
	return m_doc.IsOk();
}

wxFileName SessionManager::GetSessionFileName(const wxString& name, const wxString& suffix /*=wxT("")*/) const
{
	if (suffix.IsEmpty())
	{
	  return name + wxT(".session");
  }

	return name + suffix;
}

bool SessionManager::FindSession(const wxString &name, SessionEntry &session, const wxString& suffix /*=wxT("")*/, const wxChar* Tag /*=sessionTag*/)
{
	if (!m_doc.GetRoot()) {
		return false;
	}

	if (defaultSessionName == name)
		return false;

	wxXmlDocument doc;

	const wxFileName& sessionFileName = GetSessionFileName(name, suffix);
	if (sessionFileName.FileExists())
	{
		if (!doc.Load(sessionFileName.GetFullPath()) || !doc.IsOk())
			return false;
	}
	else
	{
		doc.SetRoot(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, Tag));
	}

	wxXmlNode* const node = doc.GetRoot();
	if (!node || node->GetName() != Tag)
		return false;

	Archive arch;
	arch.SetXmlNode(node);
	session.DeSerialize(arch);

	return true;
}

bool SessionManager::Save(const wxString &name, SessionEntry &session, const wxString& suffix /*=wxT("")*/, const wxChar* Tag /*=sessionTag*/)
{
	if (!m_doc.GetRoot()) {
		return false;
	}

	if (name.empty())
		return false;

	std::auto_ptr<wxXmlNode> child(new wxXmlNode(NULL, wxXML_ELEMENT_NODE, Tag));
	child->AddProperty(wxT("Name"), name);

	Archive arch;
	arch.SetXmlNode(child.get());
	session.Serialize(arch);

	wxXmlDocument doc;
	doc.SetRoot(child.release());

	// If we're saving a tabgroup, suffix will be ".tabgroup", not the default ".session"
	const wxFileName& sessionFileName = GetSessionFileName(name, suffix);
	return doc.Save(sessionFileName.GetFullPath());
}

void SessionManager::SetLastWorkspaceName(const wxString& name)
{
	// first delete the old entry
	wxXmlNode *node = m_doc.GetRoot()->GetChildren();
	while (node) {
		if (node->GetName() == wxT("LastActiveWorkspace")) {
			m_doc.GetRoot()->RemoveChild(node);
			delete node;
			break;
		}
		node = node->GetNext();
	}

	// set new one
	wxXmlNode *child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("LastActiveWorkspace"));
	m_doc.GetRoot()->AddChild(child);
	XmlUtils::SetNodeContent(child, name);

	// save changes
	m_doc.Save(m_fileName.GetFullPath());
}

wxString SessionManager::GetLastSession()
{
	// try to locate the 'LastActiveWorkspace' entry
	// if it does not exist or it exist with value empty return 'Default'
	// otherwise, return its content
	wxXmlNode *node = m_doc.GetRoot()->GetChildren();
	while (node) {
		if (node->GetName() == wxT("LastActiveWorkspace")) {
			if (node->GetNodeContent().IsEmpty()) {
				return defaultSessionName;
			} else {
				return node->GetNodeContent();
			}
		}
		node = node->GetNext();
	}
	return defaultSessionName;
}
