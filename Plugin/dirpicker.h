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
#ifndef DIRPICKER_H
#define DIRPICKER_H

#include "wx/panel.h"
#include "wx/button.h"
#include "wx/textctrl.h"
#include "wx/combobox.h"

enum {
	wxDP_USE_TEXTCTRL = 1,
	wxDP_USE_COMBOBOX = 2
};

/**
 * This control allows the user to select a directory. with a text control on its right side
 */
class DirPicker : public wxPanel {
	wxTextCtrl *m_path;
	wxComboBox *m_combo;

	wxButton *m_button;
	wxString m_buttonCaption;
	wxString m_dlgCaption;
	long m_style;
	wxString m_defaultPos;

private:
	void CreateControls();
	void ConnectEvents();

protected:
	/**
	 * Handle button click event
	 * \param &event
	 */
	virtual void OnButtonClicked(wxCommandEvent &event);
	virtual void OnText(wxCommandEvent &event);

public:
	DirPicker(	wxWindow *parent,
				wxWindowID id = wxID_ANY,
				const wxString &buttonCaption = wxT("..."),
				const wxString &defaultPos = wxEmptyString,
				const wxString &message = wxT("Select a folder:"),
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
				long style = wxDP_USE_TEXTCTRL);

	virtual ~DirPicker();

	/**
	 * \return the path
	 */
	wxString GetPath();

	/**
	 * set a value into the text control
	 * \param path path to set
	 */
	void SetPath(const wxString &path);

	/**
	 * Set combox values, this option is vailable only when using wxDP_USE_COMBOBOX
	 */
	void SetValues(const wxArrayString &values, int sel=0);

	wxArrayString GetValues() const;
	DECLARE_EVENT_TABLE()
};

#endif // DIRPICKER_H
