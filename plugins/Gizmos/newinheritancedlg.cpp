//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : newinheritancedlg.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "newinheritancedlg.h"
#include "open_resource_dialog.h"
#include "windowattrmanager.h"
#include "imanager.h"

NewIneritanceDlg::NewIneritanceDlg( wxWindow* parent, IManager *mgr, const wxString &parentName, const wxString &access )
: NewIneritanceBaseDlg( parent, wxID_ANY, wxT("New Inheritance"))
, m_mgr(mgr)
{
	//by default select 0
	m_choiceAccess->Select(0);
	if(access.IsEmpty() == false){
		m_choiceAccess->SetStringSelection(access);
	}
	m_textCtrlInhertiance->SetValue(parentName);
	Centre();

	WindowAttrManager::Load(this, wxT("NewIneritanceDlg"), m_mgr->GetConfigTool());
}

NewIneritanceDlg::~NewIneritanceDlg()
{
	WindowAttrManager::Save(this, wxT("NewIneritanceDlg"), m_mgr->GetConfigTool());
}

void NewIneritanceDlg::OnButtonMore( wxCommandEvent& event )
{
	m_textCtrlInhertiance->SetFocus();
	OpenResourceDialog dlg(this, m_mgr, OpenResourceDialog::TYPE_CLASS, false);
	if(dlg.ShowModal() == wxID_OK){
		wxString parentName;
		if( dlg.GetSelection().m_scope.IsEmpty() == false && dlg.GetSelection().m_scope != wxT("<global>"))
		{
			parentName << dlg.GetSelection().m_scope << wxT("::");
		}
		parentName << dlg.GetSelection().m_name;
		m_textCtrlInhertiance->SetValue(parentName);

		m_fileName = dlg.GetSelection().m_file;
	}
}
