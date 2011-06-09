/**
  \file dirtraverser.h 

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef DIRTRAVERSER_H
#define DIRTRAVERSER_H

#include <wx/dir.h>
#include <wx/arrstr.h>
#include <map>

#ifdef WXMAKINGDLL_CODELITE
#    define WXDLLIMPEXP_CL WXEXPORT
#elif defined(WXUSINGDLL_CODELITE)
#    define WXDLLIMPEXP_CL WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_CL
#endif // 

/**
 * A helper class that help us recurse into sub-directories.
 * For each found directory, sink.OnDir() is called and sink.OnFile() is called for every file. 
 * Depending on the return value, the enumeration may continue or stop of one of the files, the 
 * recurse may stop.
 * \ingroup EmbeddedLite
 * \version 1.0
 * first version
 * \date 11-08-2006
 * \author Eran
 */
class WXDLLIMPEXP_CL DirTraverser : public wxDirTraverser
{
	wxArrayString m_files;
	wxString m_filespec;
	wxArrayString m_specArray;
	
	bool m_extlessFiles;
	wxArrayString m_excludeDirs;

public:

	/**
	 * Construct a DirTraverser with a given file spec
	 */
	DirTraverser(const wxString &filespec, bool includExtLessFiles = false);
	
	/**
	 * This function is called once a file is found. The traverse of the directories 
	 * can be stopped based on the return value from this function:
	 * - wxDIR_IGNORE = -1,      // ignore this directory but continue with others
	 * - wxDIR_STOP,             // stop traversing
	 * - wxDIR_CONTINUE          // continue into this directory
	 * \param filename name of the file that was found
	 * \return one of the values wxDIR_STOP, wxDIR_IGNORE or wxDIR_CONTINUE
	 */
	virtual wxDirTraverseResult  OnFile(const wxString& filename);

	/**
	 * This function is called once a directory is found. The traverse of the directories 
	 * can be stopped based on the return value from this function:
	 * - wxDIR_IGNORE = -1,      // ignore this directory but continue with others
	 * - wxDIR_STOP,             // stop traversing
	 * - wxDIR_CONTINUE          // continue into this directory
	 * \param dirname name of the directory that was found
	 * \return one of the values wxDIR_STOP, wxDIR_IGNORE or wxDIR_CONTINUE
	 */
	virtual wxDirTraverseResult OnDir(const wxString& dirname);
	
	void SetExcludeDirs(const wxArrayString& excludeDirs) {this->m_excludeDirs = excludeDirs;}
	const wxArrayString& GetExcludeDirs() const {return m_excludeDirs;}
	
	/**
	 * Return list of files found 
	 * \return Return list of files found 
	 */
	wxArrayString& GetFiles()
	{
		return m_files;
	}
};

#endif //DIRTRAVERSER_H
