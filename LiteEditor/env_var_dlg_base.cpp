///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 25 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "env_var_dlg_base.h"

///////////////////////////////////////////////////////////////////////////

EnvVarsTableDlgBase::EnvVarsTableDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Define here set of environment variables which will be applied by EmbeddedLite before launching processes.\n\nVariables are defined in the format of NAME=VALUE"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	sbSizer1->Add( m_staticText1, 0, wxALL|wxEXPAND, 5 );
	
	bSizer1->Add( sbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Available environment sets:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	m_staticText2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer1->Add( m_staticText2, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook1 = new wxChoicebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrlDefault = new wxScintilla(m_panel1);
	bSizer5->Add( m_textCtrlDefault, 1, wxEXPAND, 5 );
	
	m_panel1->SetSizer( bSizer5 );
	m_panel1->Layout();
	bSizer5->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, wxT("Default"), false );
	bSizer2->Add( m_notebook1, 1, wxEXPAND|wxALL, 5 );
	
	bSizer6->Add( bSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_buttonNewSet = new wxButton( this, wxID_ANY, wxT("&New Set..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_buttonNewSet, 0, wxALL, 5 );
	
	m_buttonDeleteSet = new wxButton( this, wxID_ANY, wxT("Delete Set"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_buttonDeleteSet, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer7, 0, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer6, 1, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOk = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOk->SetDefault(); 
	bSizer3->Add( m_buttonOk, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_buttonCancel, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonNewSet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnvVarsTableDlgBase::OnNewSet ), NULL, this );
	m_buttonDeleteSet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnvVarsTableDlgBase::OnDeleteSet ), NULL, this );
	m_buttonDeleteSet->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EnvVarsTableDlgBase::OnDeleteSetUI ), NULL, this );
	m_buttonOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnvVarsTableDlgBase::OnButtonOk ), NULL, this );
}

EnvVarsTableDlgBase::~EnvVarsTableDlgBase()
{
	// Disconnect Events
	m_buttonNewSet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnvVarsTableDlgBase::OnNewSet ), NULL, this );
	m_buttonDeleteSet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnvVarsTableDlgBase::OnDeleteSet ), NULL, this );
	m_buttonDeleteSet->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EnvVarsTableDlgBase::OnDeleteSetUI ), NULL, this );
	m_buttonOk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnvVarsTableDlgBase::OnButtonOk ), NULL, this );
}

EnvVarSetPage::EnvVarSetPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl = new wxScintilla(this);
	bSizer6->Add( m_textCtrl, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer6 );
	this->Layout();
}

EnvVarSetPage::~EnvVarSetPage()
{
}
