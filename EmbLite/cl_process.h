/**
  \file cl_process.h

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef EMBEDDEDLITE_TAGSPROCESS_H
#define EMBEDDEDLITE_TAGSPROCESS_H

#include "wx/process.h"
#include "wx/string.h"

#ifdef WXMAKINGDLL_CODELITE
#    define WXDLLIMPEXP_CL WXEXPORT
#elif defined(WXUSINGDLL_CODELITE)
#    define WXDLLIMPEXP_CL WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_CL
#endif // WXMAKINGDLL_CODELITE

/**
 * \ingroup EmbeddedLite
 * \brief this class represents a cross platform process
 *
 * \version 1.0
 * first version
 *
 * \date 05-04-2007
 *
 * \author Eran
 */
class clProcess : public wxProcess
{
	long m_pid;
	int m_uid;
	int m_type;
	wxString m_cmd;
	bool m_redirect;
	
public:
	/**
	 * Constructs a process object. id is only used in the case you want to use wxWidgets events. 
	 * It identifies this object, or another window that will receive the event.
	 */
	clProcess(int id, const wxString &cmdLine, bool redirect = true);

	virtual ~clProcess();

	/**
	 * \return return the process id
	 */
	long GetPid();

	/**
	 * \param set the process ID
	 */
	void SetPid(long pid);

	//int GetId() { return m_id; } 

	/**
	 * Kill the process
	 */
	void Terminate();

	/** 
	 * Start the process
	 * \return the process id
	 */
	long Start(bool hide = true);

	int GetUid() { return m_uid; }
	void SetType(int type) { m_type = type; }
	int GetType() const { return m_type; }
	void SetCommand(const wxString &cmd) { m_cmd = cmd; }
	bool HasInput(wxString &input, wxString &errors);

	/**
	 * Read all process output & error
	 * \param input [output]
	 * \param errors [output] 
	 * \return 
	 */
	bool ReadAll(wxString &input, wxString &errors);

	/**
	 * Write text to the process
	 * \param text 
	 * \return true on success, false otherwise 
	 */
	bool Write(const wxString &text);
	
	//Getters
	const bool& GetRedirect() const {return m_redirect;}
};

#endif // EMBEDDEDLITE_TAGSPROCESS_H
