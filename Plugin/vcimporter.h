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
#ifndef VCIMPORTER_H
#define VCIMPORTER_H

#include "wx/string.h"
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include "map"
#include "project_settings.h"
#include "project.h"

struct VcProjectData
{
	wxString name;
	wxString id;
	wxString filepath;
	wxArrayString deps;
};

class VcImporter {
	wxString                          m_fileName;
	bool                              m_isOk;
	wxFileInputStream *               m_is;
	wxTextInputStream *               m_tis;
	std::map<wxString, VcProjectData> m_projects;
	wxString                          m_compiler;
	wxString                          m_compilerLowercase;
	
public:
	VcImporter(const wxString &fileName, const wxString &defaultCompiler);
	virtual ~VcImporter();
	bool Import(wxString &errMsg);

private:
	//read line, skip empty lines
	bool ReadLine(wxString &line);
	bool OnProject(const wxString &firstLine, wxString &errMsg);
	void RemoveGershaim(wxString &str);
	void CreateSolution();
	void CreateProjects();
	bool ConvertProject(VcProjectData &data);
	void AddConfiguration(ProjectSettingsPtr settings, wxXmlNode *config);
	void CreateFiles(wxXmlNode *parent, wxString vdPath, ProjectPtr proj);
	wxArrayString SplitString(const wxString &s);
};

#endif //VCIMPORTER_H

