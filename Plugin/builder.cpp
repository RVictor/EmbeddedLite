//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : builder.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include "builder.h"
#include "build_settings_config.h"
#include "workspace.h"
#include "buildmanager.h"
#include "macros.h"

Builder::Builder(const wxString &name, const wxString &buildTool, const wxString &buildToolOptions)
: m_name(name)
, m_buildTool(buildTool)
, m_buildToolOptions(buildToolOptions)
, m_isActive(false)
{
	//override values from configuration file
	BuilderConfigPtr config = BuildSettingsConfigST::Get()->GetBuilderConfig(m_name);
	if(config) {
		m_buildTool        = config->GetToolPath();
		m_buildToolOptions = config->GetToolOptions();
		m_isActive         = config->GetIsActive();
		m_buildToolJobs    = config->GetToolJobs();
	} else {
		m_isActive = (m_name == wxT("GNU makefile for g++/gcc"));
	}
}

Builder::~Builder()
{
}

wxString Builder::NormalizeConfigName(const wxString &confgName)
{
	wxString normalized(confgName);
	TrimString(normalized);
	normalized.Replace(wxT(" "), wxT("_"));
	return normalized;
}

void Builder::SetActive()
{
	std::list<wxString> builders;
	BuildManagerST::Get()->GetBuilders(builders);
	std::list<wxString>::iterator iter = builders.begin();
	for(; iter != builders.end(); iter++) {
		
		wxString builderName = *iter;
		BuilderPtr builder = BuildManagerST::Get()->GetBuilder(builderName);
		
		if(builder && builder->m_name == m_name)
			builder->m_isActive = true;
		
		else if(builder)
			builder->m_isActive = false;
	}
}
