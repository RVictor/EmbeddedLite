//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : depends_dlg.cpp              
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
 ///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  5 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifdef WX_PRECOMP

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#else
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "depends_dlg.h"
#include "depend_dlg_page.h"
#include "manager.h"
#include <wx/choicebk.h>
#include "macros.h"

///////////////////////////////////////////////////////////////////////////
 
DependenciesDlg::DependenciesDlg( wxWindow* parent, const wxString &projectName, int id, wxString title, wxPoint pos, wxSize size, int style ) 
: wxDialog( parent, id, title, pos, size, style )
, m_projectName(projectName)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_book = new wxChoicebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	mainSizer->Add( m_book, 1, wxEXPAND | wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* btnSizer;
	btnSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	btnSizer->Add( m_buttonOK, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	btnSizer->Add( m_buttonCancel, 0, wxALL, 5 );
	
	mainSizer->Add( btnSizer, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	
	m_book->GetChoiceCtrl()->SetFocus();
	Centre();
	Init();
}

void DependenciesDlg::Init()
{
	//fill the pages of the choice book
	wxArrayString projects;
	ManagerST::Get()->GetProjectList(projects);
	wxString activeProj = ManagerST::Get()->GetActiveProjectName();
	for(size_t i=0; i<projects.GetCount(); i++){
		m_book->AddPage(new DependenciesPage(m_book, projects.Item(i)), projects.Item(i), m_projectName == projects.Item(i));
	}

	//connect events
	ConnectButton(m_buttonOK, DependenciesDlg::OnButtonOK);
	ConnectButton(m_buttonCancel, DependenciesDlg::OnButtonCancel);
}

void DependenciesDlg::OnButtonOK(wxCommandEvent &event)
{
	wxUnusedVar(event);
	for(size_t i=0; i<m_book->GetPageCount(); i++){
		DependenciesPage *page = dynamic_cast<DependenciesPage *>( m_book->GetPage(i) );
		if(page){
			page->Save();
		}
	}
	EndModal(wxID_OK);
}

void DependenciesDlg::OnButtonCancel(wxCommandEvent &event)
{
	wxUnusedVar(event);
	EndModal(wxID_CANCEL);
}
