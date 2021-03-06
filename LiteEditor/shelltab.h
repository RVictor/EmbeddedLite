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
#ifndef SHELLTAB_H
#define SHELLTAB_H

#include "outputtabwindow.h"

class AsyncExeCmd;
class CQuickFindBar;


class ShellTab : public OutputTabWindow
{
protected:
	wxSizer      *m_inputSizer;
	wxSizer      *m_vertSizer;
	wxComboBox   *m_input;
	CQuickFindBar *m_findBar;
	AsyncExeCmd  *m_cmd;

	static  void InitStyle    (wxScintilla *sci);

	virtual bool DoSendInput  (const wxString &line);

	virtual void OnProcStarted(wxCommandEvent &e);
	virtual void OnProcOutput (wxCommandEvent &e);
	virtual void OnProcError  (wxCommandEvent &e);
	virtual void OnProcEnded  (wxCommandEvent &e);

	virtual void OnShowInput  (wxCommandEvent &e);
	virtual void OnShowSearch (wxCommandEvent &e);
	virtual void OnSendInput  (wxCommandEvent &e);
	virtual void OnStopProc   (wxCommandEvent &e);
	virtual void OnKeyDown    (wxKeyEvent      &e);
	virtual void OnEnter	  (wxCommandEvent &e);
	virtual void OnUpdateUI   (wxUpdateUIEvent &e);

	DECLARE_EVENT_TABLE()

public:
	ShellTab(wxWindow *parent, wxWindowID id, const wxString &name);
	~ShellTab();
};


class DebugTab : public ShellTab
{
protected:
	bool DoSendInput(const wxString  &line);
	void OnStopProc (wxCommandEvent &e);
	void OnUpdateUI (wxUpdateUIEvent &e);

public:
	DebugTab(wxWindow *parent, wxWindowID id, const wxString &name);
	~DebugTab();

	void AppendLine(const wxString &line);
};

class DebugTabPanel : public wxPanel
{
private:

protected:
	wxCheckBox* m_checkBoxEnableLog;

	// Virtual event handlers, overide them in your derived class
	virtual void OnEnableDbgLog( wxCommandEvent& event );
	virtual void OnEnableDbgLogUI( wxUpdateUIEvent& event );
public:

	DebugTabPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
	~DebugTabPanel();
};
#endif //SHELLTAB_H
