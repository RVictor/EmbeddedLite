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
#ifndef __progress_dialog__
#define __progress_dialog__

#include <wx/wx.h>
#include <wx/progdlg.h>

///////////////////////////////////////////////////////////////////////////

#ifdef WXMAKINGDLL_CODELITE
#    define WXDLLIMPEXP_CL WXEXPORT
#elif defined(WXUSINGDLL_CODELITE)
#    define WXDLLIMPEXP_CL WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_CL
#endif
///////////////////////////////////////////////////////////////////////////////
/// Class ProgressDialog
///////////////////////////////////////////////////////////////////////////////
class WXDLLIMPEXP_CL ProgressDialog : public wxProgressDialog {
public:
	ProgressDialog(const wxString &title, const wxString &message, int maximum, wxWindow *parent);
	virtual ~ProgressDialog();
};

#endif //__progress_dialog__
