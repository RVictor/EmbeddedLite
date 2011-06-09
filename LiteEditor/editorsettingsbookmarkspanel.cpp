//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : editorsettingsbookmarkspanel.cpp              
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

#include "editorsettingsbookmarkspanel.h"

EditorSettingsBookmarksPanel::EditorSettingsBookmarksPanel( wxWindow* parent )
: EditorSettingsBookmarksBasePanel( parent )
, TreeBookNode<EditorSettingsBookmarksPanel>()
{
	m_backgroundColor->SetColour(wxColour(255,0,255));
	m_foregroundColor->SetColour(wxColour(255,0,255));
	m_highlightColor->SetColour(wxString(wxT("LIGHT BLUE")));

	//get the editor's options from the disk
	OptionsConfigPtr options = EditorConfigST::Get()->GetOptions();
	m_displaySelection->SetValue(options->GetDisplayBookmarkMargin());
	m_bookMarkShape->SetStringSelection(options->GetBookmarkShape());
	m_backgroundColor->SetColour(options->GetBookmarkBgColour());
	m_foregroundColor->SetColour(options->GetBookmarkFgColour());

	wxString val1 = EditorConfigST::Get()->GetStringValue(wxT("WordHighlightColour"));
	if (val1.IsEmpty() == false) {
		m_highlightColor->SetColour(val1);
	}
}


void EditorSettingsBookmarksPanel::Save(OptionsConfigPtr options)
{
	options->SetDisplayBookmarkMargin( m_displaySelection->IsChecked() );
	options->SetBookmarkShape( m_bookMarkShape->GetStringSelection());
	options->SetBookmarkBgColour( m_backgroundColor->GetColour() );
	options->SetBookmarkFgColour( m_foregroundColor->GetColour() );
	EditorConfigST::Get()->SaveStringValue(wxT("WordHighlightColour"), m_highlightColor->GetColour().GetAsString());
}
