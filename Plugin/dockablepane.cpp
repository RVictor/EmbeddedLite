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
#include <wx/settings.h>
#include <wx/app.h>
#include <wx/dcbuffer.h>
#include <wx/xrc/xmlres.h>
#include "custom_notebook.h"
#include <wx/sizer.h>
#include "dockablepane.h"

const wxEventType wxEVT_CMD_NEW_DOCKPANE = XRCID("new_dockpane");
const wxEventType wxEVT_CMD_DELETE_DOCKPANE = XRCID("delete_dockpane");

BEGIN_EVENT_TABLE(DockablePane, wxPanel)
EVT_ERASE_BACKGROUND(DockablePane::OnEraseBg)
EVT_PAINT(DockablePane::OnPaint)
END_EVENT_TABLE()

DockablePane::DockablePane(wxWindow* parent, Notebook* book, wxWindow* child, const wxString& title, const wxBitmap& bmp, wxSize size)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, size)
, m_child(child)
, m_book(book)
, m_text(title)
, m_bmp(bmp)
{
	wxBoxSizer *sz = new wxBoxSizer(wxVERTICAL);
	SetSizer(sz);

    Connect(XRCID("close_pane"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DockablePane::ClosePane ));

	m_child->Reparent(this);

	sz->Add(m_child, 1, wxEXPAND|wxALL, 2);
	sz->Layout();

    wxCommandEvent event(wxEVT_CMD_NEW_DOCKPANE);
	event.SetClientData(this);
	parent->GetEventHandler()->AddPendingEvent(event);
}

DockablePane::~DockablePane()
{
}

void DockablePane::ClosePane(wxCommandEvent& e)
{
	wxUnusedVar(e);

    if (m_book) {
        // first detach the child from this pane
        wxSizer *sz = GetSizer();
        sz->Detach(m_child);

        // now we can add it to the noteook (it will be automatically be reparented to the notebook)
        m_book->AddPage(m_child, m_text, wxT(""), m_bmp, false);
	}

	wxCommandEvent event(wxEVT_CMD_DELETE_DOCKPANE);
	event.SetClientData(this);
	GetParent()->GetEventHandler()->AddPendingEvent(event);
}

void DockablePane::OnPaint(wxPaintEvent& e)
{
	wxBufferedPaintDC dc(this);

	dc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)));
	dc.SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)));

	dc.DrawRectangle(GetClientSize());
}

