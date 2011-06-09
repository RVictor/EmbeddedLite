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
#ifndef BUILD_SYSTEM_H
#define BUILD_SYSTEM_H

#include "wx/xml/xml.h"
#include "wx/string.h"
#include "smart_ptr.h"
#include "list"

class BuilderConfig
{
	wxString m_name;
	wxString m_toolPath;
	wxString m_toolOptions;
	wxString m_toolJobs;
	bool     m_isActive;

public:
	BuilderConfig(wxXmlNode *node);
	virtual ~BuilderConfig();
	wxXmlNode *ToXml() const;

	void SetIsActive(bool isActive) {
		this->m_isActive = isActive;
	}
	bool GetIsActive() const {
		return m_isActive;
	}
	const wxString &GetName() const {
		return m_name;
	}
	const wxString &GetToolPath() const {
		return m_toolPath;
	}
	const wxString &GetToolOptions() const {
		return m_toolOptions;
	}
	const wxString &GetToolJobs() const {
		return m_toolJobs;
	}
	void SetName(const wxString &name) {
		m_name = name;
	}
	void SetToolPath(const wxString &path) {
		m_toolPath = path;
	}
	void SetToolOptions(const wxString &options) {
		m_toolOptions = options;
	}
	void SetToolJobs(const wxString &jobs) {
		m_toolJobs = jobs;
	}
};

typedef SmartPtr<BuilderConfig>     BuilderConfigPtr;

#endif //BUILD_SYSTEM_H
