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
#include "formbuildsettingsbasedlg.h"

///////////////////////////////////////////////////////////////////////////

FormBuildSettingsBaseDlg::FormBuildSettingsBaseDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_panel1, wxID_ANY, wxT("wxFormBuilder path:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlFBPath = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlFBPath->SetMinSize( wxSize( 400,-1 ) );
	
	fgSizer1->Add( m_textCtrlFBPath, 1, wxALL|wxEXPAND, 5 );
	
	m_buttonBrowse = new wxButton( m_panel1, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_buttonBrowse, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( m_panel1, wxID_ANY, wxT("Command:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textCtrlCommand = new wxTextCtrl( m_panel1, wxID_ANY, wxT("$(WXFB) $(WXFB_PRJ)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrlCommand, 1, wxALL|wxEXPAND, 5 );
	
	
	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_panel1->SetSizer( fgSizer1 );
	m_panel1->Layout();
	fgSizer1->Fit( m_panel1 );
	mainSizer->Add( m_panel1, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOk = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOk->SetDefault(); 
	bSizer2->Add( m_buttonOk, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonCancel, 0, wxALL, 5 );
	
	mainSizer->Add( bSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonBrowse->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FormBuildSettingsBaseDlg::OnButtonBrowse ), NULL, this );
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FormBuildSettingsBaseDlg::OnButtonOK ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FormBuildSettingsBaseDlg::OnButtonCancel ), NULL, this );
}

FormBuildSettingsBaseDlg::~FormBuildSettingsBaseDlg()
{
	// Disconnect Events
	m_buttonBrowse->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FormBuildSettingsBaseDlg::OnButtonBrowse ), NULL, this );
	m_buttonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FormBuildSettingsBaseDlg::OnButtonOK ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FormBuildSettingsBaseDlg::OnButtonCancel ), NULL, this );
}
