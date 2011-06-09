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
#ifndef BUILD_CONFIGURATION_COMMON_H
#define BUILD_CONFIGURATION_COMMON_H

#include "configuration_object.h"
#include <wx/arrstr.h>
#include <wx/string.h>

#ifndef WXDLLIMPEXP_LE_SDK
#ifdef WXMAKINGDLL_LE_SDK
#    define WXDLLIMPEXP_LE_SDK WXEXPORT
#elif defined(WXUSINGDLL_LE_SDK)
#    define WXDLLIMPEXP_LE_SDK WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_LE_SDK
#endif // WXMAKINGDLL_LE_SDK
#endif

class WXDLLIMPEXP_LE_SDK BuildConfigCommon : public ConfObject
{
	wxArrayString m_includePath;
	wxString m_compileOptions;
	wxArrayString m_AsmIncludePath;
	wxString m_assemblerOptions;
	wxString m_linkOptions;
	wxArrayString m_libs;
	wxArrayString m_libPath;
	wxArrayString m_preprocessor;
	wxString m_resCompileOptions;
	wxString m_resCmpIncludePath;
	
	wxString m_confType; // xml node name 

public:
	BuildConfigCommon(wxXmlNode* node, wxString confType = wxT("Configuration"));
	virtual ~BuildConfigCommon();
	wxXmlNode *ToXml() const;
	BuildConfigCommon *Clone() const;

	//--------------------------------
	// Setters / Getters
	//--------------------------------
	
	wxString GetPreprocessor() const;
	void GetPreprocessor(wxArrayString &arr) {
		arr = m_preprocessor;
	}
	void SetPreprocessor(const wxString &prepr);

	const wxString &GetCompileOptions() const {
		return m_compileOptions;
	}
	void SetCompileOptions(const wxString &options) {
		m_compileOptions = options;
	}

	const wxString &GetAssemblerOptions() const {
		return m_assemblerOptions;
	}
	void SetAssemblerOptions(const wxString &options) {
		m_assemblerOptions= options;
	}

	const wxString &GetLinkOptions() const {
		return m_linkOptions;
	}
	void SetLinkOptions(const wxString &options) {
		m_linkOptions = options;
	}

	wxString GetIncludePath() const;
	void SetIncludePath(const wxString &path);
	void SetIncludePath(const wxArrayString &paths) {
		m_includePath = paths;
	}

	wxString GetAsmIncludePath() const;
	void SetAsmIncludePath(const wxString &path);
	void SetAsmIncludePath(const wxArrayString &paths) {
		m_AsmIncludePath = paths;
	}
  
	wxString GetLibraries() const;
	void SetLibraries(const wxString &libs);
	void SetLibraries(const wxArrayString &libs) {
		m_libs = libs;
	}

	wxString GetLibPath() const;
	void SetLibPath(const wxString &path);
	void SetLibPath(const wxArrayString &libPaths) {
		m_libPath = libPaths;
	}

	const wxString& GetResCmpIncludePath() const {
		return m_resCmpIncludePath;
	}
	void SetResCmpIncludePath(const wxString &path) {
		m_resCmpIncludePath = path;
	}
	
	const wxString &GetResCompileOptions() const {
		return m_resCompileOptions;
	}
	void SetResCmpOptions(const wxString &options) {
		m_resCompileOptions = options;
	}
};

typedef SmartPtr<BuildConfigCommon> BuildConfigCommonPtr;

#endif // BUILD_CONFIGURATION_COMMON_H
