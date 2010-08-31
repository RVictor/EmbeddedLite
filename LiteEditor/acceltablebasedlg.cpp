//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : acceltablebasedlg.cpp              
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
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "acceltablebasedlg.h"

///////////////////////////////////////////////////////////////////////////

AccelTableBaseDlg::AccelTableBaseDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_listCtrl1 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_SORT_ASCENDING|wxLC_VRULES );
	bSizer4->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_buttonEdit = new wxButton( this, wxID_ANY, wxT("&Edit..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_buttonEdit, 0, wxALL, 5 );
	
	m_buttonDefault = new wxButton( this, wxID_ANY, wxT("&Defaults"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_buttonDefault, 0, wxALL, 5 );
	
	bSizer4->Add( bSizer3, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( this, wxID_OK, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button1->SetDefault(); 
	bSizer2->Add( m_button1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( this, wxID_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button2, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_listCtrl1->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( AccelTableBaseDlg::OnColClicked ), NULL, this );
	m_listCtrl1->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( AccelTableBaseDlg::OnItemActivated ), NULL, this );
	m_listCtrl1->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AccelTableBaseDlg::OnItemSelected ), NULL, this );
	m_buttonEdit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AccelTableBaseDlg::OnEditButton ), NULL, this );
	m_buttonDefault->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AccelTableBaseDlg::OnButtonDefaults ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AccelTableBaseDlg::OnButtonOk ), NULL, this );
}

AccelTableBaseDlg::~AccelTableBaseDlg()
{
	// Disconnect Events
	m_listCtrl1->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( AccelTableBaseDlg::OnColClicked ), NULL, this );
	m_listCtrl1->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( AccelTableBaseDlg::OnItemActivated ), NULL, this );
	m_listCtrl1->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AccelTableBaseDlg::OnItemSelected ), NULL, this );
	m_buttonEdit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AccelTableBaseDlg::OnEditButton ), NULL, this );
	m_buttonDefault->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AccelTableBaseDlg::OnButtonDefaults ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AccelTableBaseDlg::OnButtonOk ), NULL, this );
}
