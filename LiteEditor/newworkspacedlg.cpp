/**
  \file project.cpp

  \brief EmbeddedLite (CodeLite) file
  \author Eran Ifrah, V. Ridtchenko

  \notes

  Copyright: (C) 2008 by Eran Ifrah, 2010 Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include "newworkspacedlg.h"
#include "windowattrmanager.h"
#include "globals.h"
#include "wx/msgdlg.h"
#include "wx/dirdlg.h"
#include "wx/filename.h"
#include "elconfig.h"

NewWorkspaceDlg::NewWorkspaceDlg( wxWindow* parent )
		:
		NewWorkspaceBase( parent )
{
	m_textCtrlWorkspacePath->SetValue(wxGetCwd());
	m_textCtrlWorkspaceName->SetFocus();
	
	WindowAttrManager::Load(this, wxT("NewWorkspaceDlgAttr"), NULL);
	Centre();
}

NewWorkspaceDlg::~NewWorkspaceDlg()
{
	WindowAttrManager::Save(this, wxT("NewWorkspaceDlgAttr"), NULL);
}

void NewWorkspaceDlg::OnWorkspacePathUpdated( wxCommandEvent& event )
{
	//update the static text control with the actual path

	wxString workspacePath;
	workspacePath << m_textCtrlWorkspacePath->GetValue();

	workspacePath = workspacePath.Trim().Trim(false);

	wxString tmpSep( wxFileName::GetPathSeparator() );
	if ( !workspacePath.EndsWith(tmpSep) && workspacePath.IsEmpty() == false ) {
		workspacePath << wxFileName::GetPathSeparator();
	}
	
	if( m_textCtrlWorkspaceName->GetValue().Trim().Trim(false).IsEmpty() ) {
		m_staticTextWorkspaceFileName->SetLabel(wxEmptyString);
		return;
	}
	
	if( m_checkBoxCreateSeparateDir->IsChecked() ) {
		workspacePath << m_textCtrlWorkspaceName->GetValue();
		workspacePath << wxFileName::GetPathSeparator();
	}
	
	workspacePath << m_textCtrlWorkspaceName->GetValue();
	workspacePath << wxT(".") << EL_WORKSPACE_EXT;
	m_staticTextWorkspaceFileName->SetLabel( workspacePath );
}

void NewWorkspaceDlg::OnWorkspaceDirPicker( wxCommandEvent& event )
{
	const wxString& dir = wxDirSelector(wxT("Choose a folder:"));
	if ( !dir.empty() ) {
		m_textCtrlWorkspacePath->SetValue( dir );
	}
}

void NewWorkspaceDlg::OnButtonCreate( wxCommandEvent& event )
{
	//validate that the path part is valid
	m_workspacePath = m_staticTextWorkspaceFileName->GetLabel();
	wxFileName fn(m_workspacePath);
	
	if( m_checkBoxCreateSeparateDir->IsChecked() ){
		// dont test the result
		fn.Mkdir(fn.GetPath(), 0777, wxPATH_MKDIR_FULL);
	}
	
	if ( !wxDirExists(fn.GetPath()) ) {
		wxMessageBox(_("Invalid path: ") + fn.GetPath(), wxT("Error"), wxOK | wxICON_HAND);
		return;
	}
	EndModal(wxID_OK);
}
