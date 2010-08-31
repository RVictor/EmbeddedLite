//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : codeforammaterbasedlg.cpp              
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

#include "codeforammaterbasedlg.h"

///////////////////////////////////////////////////////////////////////////

CodeFormatterBaseDlg::CodeFormatterBaseDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticText2 = new wxStaticText( m_panel2, wxID_ANY, wxT("Predefined Style options define the style by setting several other options\nIf other options are also used, the other options will override the\npredefined style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	sbSizer3->Add( m_staticText2, 1, wxALL|wxEXPAND, 5 );
	
	bSizer8->Add( sbSizer3, 0, wxEXPAND|wxALL, 5 );
	
	wxString m_radioBoxPredefinedStyleChoices[] = { wxT("GNU"), wxT("Java"), wxT("K&&R"), wxT("Linux"), wxT("ANSI") };
	int m_radioBoxPredefinedStyleNChoices = sizeof( m_radioBoxPredefinedStyleChoices ) / sizeof( wxString );
	m_radioBoxPredefinedStyle = new wxRadioBox( m_panel2, wxID_ANY, wxT("Predefined Style:"), wxDefaultPosition, wxDefaultSize, m_radioBoxPredefinedStyleNChoices, m_radioBoxPredefinedStyleChoices, 1, wxRA_SPECIFY_COLS );
	m_radioBoxPredefinedStyle->SetSelection( 0 );
	bSizer8->Add( m_radioBoxPredefinedStyle, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticTextPredefineHelp = new wxStaticText( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPredefineHelp->Wrap( -1 );
	sbSizer4->Add( m_staticTextPredefineHelp, 1, wxALL|wxEXPAND, 5 );
	
	bSizer8->Add( sbSizer4, 1, wxEXPAND|wxALL, 5 );
	
	m_panel2->SetSizer( bSizer8 );
	m_panel2->Layout();
	bSizer8->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, wxT("Predefined Styles"), true );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_radioBoxBracketsChoices[] = { wxT("Break Closing"), wxT("Attach"), wxT("Linux"), wxT("Break"), wxT("None") };
	int m_radioBoxBracketsNChoices = sizeof( m_radioBoxBracketsChoices ) / sizeof( wxString );
	m_radioBoxBrackets = new wxRadioBox( m_panel3, wxID_ANY, wxT("Brackets:"), wxDefaultPosition, wxDefaultSize, m_radioBoxBracketsNChoices, m_radioBoxBracketsChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxBrackets->SetSelection( 3 );
	bSizer3->Add( m_radioBoxBrackets, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Indentation:") ), wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_checkBoxIndetClass = new wxCheckBox( m_panel3, wxID_ANY, wxT("Class"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndetClass, 0, wxALL, 5 );
	
	m_checkBoxIndentBrackets = new wxCheckBox( m_panel3, wxID_ANY, wxT("Brackets"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndentBrackets, 0, wxALL, 5 );
	
	m_checkBoxIndetSwitch = new wxCheckBox( m_panel3, wxID_ANY, wxT("Switches"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndetSwitch, 0, wxALL, 5 );
	
	m_checkBoxIndentNamespaces = new wxCheckBox( m_panel3, wxID_ANY, wxT("Namespaces"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndentNamespaces, 0, wxALL, 5 );
	
	m_checkBoxIndetCase = new wxCheckBox( m_panel3, wxID_ANY, wxT("Case"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndetCase, 0, wxALL, 5 );
	
	m_checkBoxIndentLabels = new wxCheckBox( m_panel3, wxID_ANY, wxT("Labels"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndentLabels, 0, wxALL, 5 );
	
	m_checkBoxIndetBlocks = new wxCheckBox( m_panel3, wxID_ANY, wxT("Blocks"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndetBlocks, 0, wxALL, 5 );
	
	m_checkBoxIndentPreprocessors = new wxCheckBox( m_panel3, wxID_ANY, wxT("Preprocessors"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxIndentPreprocessors->SetValue(true);
	
	gSizer1->Add( m_checkBoxIndentPreprocessors, 0, wxALL, 5 );
	
	m_checkBoxIndentMaxInst = new wxCheckBox( m_panel3, wxID_ANY, wxT("Max Instatement Indent"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndentMaxInst, 0, wxALL, 5 );
	
	m_checkBoxIndentMinCond = new wxCheckBox( m_panel3, wxID_ANY, wxT("Min Conditional Indent"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer1->Add( m_checkBoxIndentMinCond, 0, wxALL, 5 );
	
	sbSizer1->Add( gSizer1, 0, wxEXPAND, 5 );
	
	bSizer3->Add( sbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Formatting Options:") ), wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_checkBoxFormatBreakBlocks = new wxCheckBox( m_panel3, wxID_ANY, wxT("Break Blocks"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatBreakBlocks, 0, wxALL, 5 );
	
	m_checkBoxFormatPadParenth = new wxCheckBox( m_panel3, wxID_ANY, wxT("Pad Parenthesis"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatPadParenth, 0, wxALL, 5 );
	
	m_checkBoxFormatBreakBlocksAll = new wxCheckBox( m_panel3, wxID_ANY, wxT("Break Blocks All"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatBreakBlocksAll, 0, wxALL, 5 );
	
	m_checkBoxFormatPadParentOut = new wxCheckBox( m_panel3, wxID_ANY, wxT("Pad Parenthesis Outside"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatPadParentOut, 0, wxALL, 5 );
	
	m_checkBoxFormatBreakElseif = new wxCheckBox( m_panel3, wxID_ANY, wxT("Break else-if"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatBreakElseif, 0, wxALL, 5 );
	
	m_checkBoxFormatPadParentIn = new wxCheckBox( m_panel3, wxID_ANY, wxT("Pad Parenthesis  Inside"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatPadParentIn, 0, wxALL, 5 );
	
	m_checkBoxFormatPadOperators = new wxCheckBox( m_panel3, wxID_ANY, wxT("Pad Operators"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatPadOperators, 0, wxALL, 5 );
	
	m_checkBoxFormatUnPadParent = new wxCheckBox( m_panel3, wxID_ANY, wxT("UnPad Parenthesis"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatUnPadParent, 0, wxALL, 5 );
	
	m_checkBoxFormatOneLineKeepStmnt = new wxCheckBox( m_panel3, wxID_ANY, wxT("One Line Keep Statement"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatOneLineKeepStmnt, 0, wxALL, 5 );
	
	m_checkBoxFormatFillEmptyLines = new wxCheckBox( m_panel3, wxID_ANY, wxT("Fill Empty Lines"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatFillEmptyLines, 0, wxALL, 5 );
	
	m_checkBoxFormatOneLineKeepBlocks = new wxCheckBox( m_panel3, wxID_ANY, wxT("One Line Keep Blocks"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer2->Add( m_checkBoxFormatOneLineKeepBlocks, 0, wxALL, 5 );
	
	sbSizer2->Add( gSizer2, 0, wxEXPAND, 5 );
	
	bSizer3->Add( sbSizer2, 0, wxEXPAND|wxALL, 5 );
	
	m_panel3->SetSizer( bSizer3 );
	m_panel3->Layout();
	bSizer3->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("Custom"), false );
	
	bSizer5->Add( m_notebook1, 3, wxEXPAND | wxALL, 5 );
	
	m_textCtrlPreview = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY );
	m_textCtrlPreview->SetFont( wxFont( 8, 76, 90, 90, false, wxEmptyString ) );
	m_textCtrlPreview->SetMinSize( wxSize( -1,200 ) );
	
	bSizer5->Add( m_textCtrlPreview, 2, wxALL|wxEXPAND, 5 );
	
	bSizer1->Add( bSizer5, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	bSizer2->Add( m_buttonOK, 0, wxALL, 5 );
	
	m_buttonClose = new wxButton( this, wxID_CANCEL, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonClose, 0, wxALL, 5 );
	
	m_buttonHelp = new wxButton( this, wxID_ANY, wxT("&Help"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonHelp, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_radioBoxPredefinedStyle->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( CodeFormatterBaseDlg::OnRadioBoxPredefinedStyle ), NULL, this );
	m_radioBoxBrackets->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( CodeFormatterBaseDlg::OnRadioBoxBrackets ), NULL, this );
	m_checkBoxIndetClass->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentBrackets->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndetSwitch->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentNamespaces->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndetCase->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentLabels->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndetBlocks->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentPreprocessors->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentMaxInst->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentMinCond->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatBreakBlocks->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatPadParenth->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatBreakBlocksAll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatPadParentOut->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatBreakElseif->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatPadParentIn->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatPadOperators->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatUnPadParent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatOneLineKeepStmnt->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatFillEmptyLines->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatOneLineKeepBlocks->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
}

CodeFormatterBaseDlg::~CodeFormatterBaseDlg()
{
	// Disconnect Events
	m_radioBoxPredefinedStyle->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( CodeFormatterBaseDlg::OnRadioBoxPredefinedStyle ), NULL, this );
	m_radioBoxBrackets->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( CodeFormatterBaseDlg::OnRadioBoxBrackets ), NULL, this );
	m_checkBoxIndetClass->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentBrackets->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndetSwitch->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentNamespaces->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndetCase->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentLabels->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndetBlocks->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentPreprocessors->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentMaxInst->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxIndentMinCond->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatBreakBlocks->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatPadParenth->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatBreakBlocksAll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatPadParentOut->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatBreakElseif->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatPadParentIn->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatPadOperators->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatUnPadParent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatOneLineKeepStmnt->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatFillEmptyLines->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
	m_checkBoxFormatOneLineKeepBlocks->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CodeFormatterBaseDlg::OnCheckBox ), NULL, this );
}
