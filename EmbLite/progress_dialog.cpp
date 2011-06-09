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
#include "progress_dialog.h"

///////////////////////////////////////////////////////////////////////////

ProgressDialog::ProgressDialog(const wxString &title, const wxString &message, int maximum, wxWindow *parent) 
: wxProgressDialog(title, message, maximum, parent,  wxPD_APP_MODAL |	wxPD_SMOOTH | wxPD_AUTO_HIDE)
{
	SetSize(400, 250);
	Layout();
}

ProgressDialog::~ProgressDialog()
{
}
