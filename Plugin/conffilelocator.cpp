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
#include "conffilelocator.h"
#include <wx/filename.h>

ConfFileLocator* ConfFileLocator::ms_instance = 0;

ConfFileLocator::ConfFileLocator()
{
}

ConfFileLocator::~ConfFileLocator()
{
}

ConfFileLocator* ConfFileLocator::Instance()
{
	if(ms_instance == 0){
		ms_instance = new ConfFileLocator();
	}
	return ms_instance;
}

void ConfFileLocator::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}

void ConfFileLocator::Initialize(const wxString& installpath, const wxString& startuppath)
{
	m_installPath = installpath;
	m_startupPath = startuppath;
}

wxString ConfFileLocator::Locate(const wxString& baseName)
{
	wxFileName privateFile(GetLocalCopy(baseName));
	wxFileName defaultFile(GetDefaultCopy(baseName));
	
	if(privateFile.FileExists()){
		return privateFile.GetFullPath();
	} else {
		return defaultFile.GetFullPath();
	}
}
//todo
wxString ConfFileLocator::GetLocalCopy(const wxString& baseName)
{
	wxFileName FileName(baseName);

  return FileName.IsAbsolute() ? baseName : m_startupPath + wxT("/") + baseName;
}
//todo
wxString ConfFileLocator::GetDefaultCopy(const wxString& baseName)
{
	wxFileName FileName(baseName);

	return FileName.IsAbsolute() ? 
    baseName + wxT(".default") : m_installPath + wxT("/") + baseName + wxT(".default");
}
