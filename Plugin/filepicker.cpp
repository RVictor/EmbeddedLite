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
#include "filepicker.h"
#include "wx/sizer.h"
#include "macros.h"

FilePicker::FilePicker(wxWindow *parent, 
					   wxWindowID id, 
					   const wxString &defaultFile,
					   const wxString &message, 
					   const wxString &wildCard, 
					   const wxString &buttonCaption, 
					   const wxPoint& pos, 
					   const wxSize& size, 
					   long style)
: wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL | wxNO_BORDER)
, m_buttonCaption(buttonCaption)
, m_dlgCaption(message)
, m_dlgStyle(style)
, m_defaultFile(defaultFile)
, m_wildCard(wildCard)
{

#if defined(__WXGTK__) || defined (__WXMAC__)
	if(m_wildCard == wxT("*.*")){
		m_wildCard = wxT("*");
	}
#endif

	CreateControls();
	ConnectEvents();
}

FilePicker::~FilePicker()
{
}

void FilePicker::CreateControls()
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(mainSizer);

	size_t flags = wxRIGHT | wxTOP | wxBOTTOM | wxALIGN_CENTER | wxEXPAND;
	
	m_path = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	mainSizer->Add(m_path, 1, flags, 5);

	m_button = new wxButton(this, wxID_ANY, m_buttonCaption);
	mainSizer->Add(m_button, 0, wxALL, 5);

	Layout();
}

void FilePicker::ConnectEvents()
{
	ConnectButton(m_button, FilePicker::OnButtonClicked);
}

void FilePicker::OnButtonClicked(wxCommandEvent &event)
{
	wxUnusedVar(event);
	wxFileDialog *dlg = new wxFileDialog(this, m_dlgCaption, wxEmptyString, m_defaultFile, m_wildCard, m_dlgStyle);
	if(dlg->ShowModal() == wxID_OK)
	{
		// Get the dirname
		wxString path = dlg->GetPath();
		m_path->SetValue(path);
	}
	dlg->Destroy();
}

