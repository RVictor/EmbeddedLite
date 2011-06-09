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
#include "threebuttondlg.h"

ThreeButtonDlg::ThreeButtonDlg( wxWindow* parent, const wxString &msg, const wxString &caption )
: ThreeButtonBaseDlg( parent )
{
	m_message->SetLabel(msg);
	m_buttonYes->SetFocus();
	SetLabel(caption);
	GetSizer()->Fit(this);
	GetSizer()->Layout();
	Centre();
}

void ThreeButtonDlg::OnButtonYes(wxCommandEvent &e)
{
	EndModal(wxID_OK);
}

void ThreeButtonDlg::OnButtonNo(wxCommandEvent &e)
{
	EndModal(wxID_NO);
}

void ThreeButtonDlg::OnButtonCancel(wxCommandEvent &e)
{
	EndModal(wxID_CANCEL);
}
