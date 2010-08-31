///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 25 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "editoroptionsgeneralguidespanelbase.h"

///////////////////////////////////////////////////////////////////////////

EditorOptionsGeneralGuidesPanelBase::EditorOptionsGeneralGuidesPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_displayLineNumbers = new wxCheckBox( this, wxID_ANY, _("Display line numbers"), wxDefaultPosition, wxDefaultSize, 0 );
	m_displayLineNumbers->SetToolTip( _("Show line numbers margin") );
	
	fgSizer1->Add( m_displayLineNumbers, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_showIndentationGuideLines = new wxCheckBox( this, wxID_ANY, _("Show indentation guidelines"), wxDefaultPosition, wxDefaultSize, 0 );
	m_showIndentationGuideLines->SetToolTip( _("Turn on indentation highlights guides (small vertical lines)") );
	
	fgSizer1->Add( m_showIndentationGuideLines, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_checkBoxMatchBraces = new wxCheckBox( this, wxID_ANY, _("Highlight matched braces"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxMatchBraces->SetToolTip( _("Highlight matched braces") );
	
	fgSizer1->Add( m_checkBoxMatchBraces, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_checkBoxAutoCompleteBraces = new wxCheckBox( this, wxID_ANY, _("Auto Complete Braces"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxAutoCompleteBraces->SetToolTip( _("Auto add matching close brace") );
	
	fgSizer1->Add( m_checkBoxAutoCompleteBraces, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_checkBoxAdjustScrollbarSize = new wxCheckBox( this, wxID_ANY, _("Auto adjust editor's horizontal scrollbar width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxAdjustScrollbarSize->SetToolTip( _("Adujst the horizontal scrollbar width to the editor's content") );
	
	fgSizer1->Add( m_checkBoxAdjustScrollbarSize, 0, wxALL, 5 );
	
	m_checkBoxDisableSemicolonShift = new wxCheckBox( this, wxID_ANY, _("Disable semicolon shift"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxDisableSemicolonShift->SetToolTip( _("Auto swap between semicolon and closing brace") );
	
	fgSizer1->Add( m_checkBoxDisableSemicolonShift, 0, wxALL|wxEXPAND, 5 );
	
	m_checkBoxHideChangeMarkerMargin = new wxCheckBox( this, wxID_ANY, _("Hide change marker margin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxHideChangeMarkerMargin->SetToolTip( _("Hide / Display the change marker margin (red/green marks when line is modified)") );
	
	fgSizer1->Add( m_checkBoxHideChangeMarkerMargin, 0, wxALL, 5 );
	
	
	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Whitespace visibility:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	wxString m_whitespaceStyleChoices[] = { _("Invisible"), _("Visible always"), _("Visible after indentation"), _("Indentation only") };
	int m_whitespaceStyleNChoices = sizeof( m_whitespaceStyleChoices ) / sizeof( wxString );
	m_whitespaceStyle = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_whitespaceStyleNChoices, m_whitespaceStyleChoices, 0 );
	m_whitespaceStyle->SetSelection( 0 );
	fgSizer1->Add( m_whitespaceStyle, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("EOL Mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	m_staticText4->SetToolTip( _("Set the editor's EOL mode (End Of Line)") );
	
	fgSizer1->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	wxString m_choiceEOLChoices[] = { _("Default"), _("Mac (CR)"), _("Windows (CRLF)"), _("Unix (LF)") };
	int m_choiceEOLNChoices = sizeof( m_choiceEOLChoices ) / sizeof( wxString );
	m_choiceEOL = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceEOLNChoices, m_choiceEOLChoices, 0 );
	m_choiceEOL->SetSelection( 0 );
	m_choiceEOL->SetToolTip( _("Set the editor's EOL mode (End Of Line). When set to 'Default' EmbeddedLite will set the EOL according to the hosting OS") );
	
	fgSizer1->Add( m_choiceEOL, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	bSizer1->Add( fgSizer1, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_highlightCaretLine = new wxCheckBox( this, wxID_ANY, _("Highlight caret line"), wxDefaultPosition, wxDefaultSize, 0 );
	m_highlightCaretLine->SetToolTip( _("Highlight the caret line") );
	
	fgSizer2->Add( m_highlightCaretLine, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Caret line backgound color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer2->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_caretLineColourPicker = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE|wxCLRP_SHOW_LABEL );
	fgSizer2->Add( m_caretLineColourPicker, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_staticText41 = new wxStaticText( this, wxID_ANY, _("Caret line alpha:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer2->Add( m_staticText41, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_sliderCaretLineAlpha = new wxSlider( this, wxID_ANY, 50, 0, 255, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_sliderCaretLineAlpha, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	bSizer1->Add( fgSizer2, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	// Connect Events
	m_highlightCaretLine->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EditorOptionsGeneralGuidesPanelBase::OnHighlightCaretLine ), NULL, this );
	m_staticText1->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorOptionsGeneralGuidesPanelBase::OnhighlightCaretLineUI ), NULL, this );
	m_caretLineColourPicker->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorOptionsGeneralGuidesPanelBase::OnhighlightCaretLineUI ), NULL, this );
	m_staticText41->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorOptionsGeneralGuidesPanelBase::OnhighlightCaretLineUI ), NULL, this );
	m_sliderCaretLineAlpha->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorOptionsGeneralGuidesPanelBase::OnhighlightCaretLineUI ), NULL, this );
}

EditorOptionsGeneralGuidesPanelBase::~EditorOptionsGeneralGuidesPanelBase()
{
	// Disconnect Events
	m_highlightCaretLine->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EditorOptionsGeneralGuidesPanelBase::OnHighlightCaretLine ), NULL, this );
	m_staticText1->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorOptionsGeneralGuidesPanelBase::OnhighlightCaretLineUI ), NULL, this );
	m_caretLineColourPicker->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorOptionsGeneralGuidesPanelBase::OnhighlightCaretLineUI ), NULL, this );
	m_staticText41->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorOptionsGeneralGuidesPanelBase::OnhighlightCaretLineUI ), NULL, this );
	m_sliderCaretLineAlpha->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( EditorOptionsGeneralGuidesPanelBase::OnhighlightCaretLineUI ), NULL, this );
}
