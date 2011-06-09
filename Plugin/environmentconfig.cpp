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
#include "wx/utils.h"
#include "xmlutils.h"
#include "wx/regex.h"
#include "wx/filename.h"
#include "environmentconfig.h"
#include "evnvarlist.h"
#include "wx_xml_compatibility.h"

EnvironmentConfig* EnvironmentConfig::ms_instance = 0;

//------------------------------------------------------------------------------

EnvironmentConfig::EnvironmentConfig()
{
}

EnvironmentConfig::~EnvironmentConfig()
{
}

EnvironmentConfig* EnvironmentConfig::Instance()
{
	if (ms_instance == 0) {
		ms_instance = new EnvironmentConfig();
	}
	return ms_instance;
}

void EnvironmentConfig::Release()
{
	if (ms_instance) {
		delete ms_instance;
	}
	ms_instance = 0;
}

wxString EnvironmentConfig::GetRootName()
{
	return wxT("EnvironmentVariables");
}

bool EnvironmentConfig::Load(const wxString& strConfigDir)
{
	bool loaded = ConfigurationToolBase::Load( strConfigDir + wxT("/environment_variables.xml") );
	if(loaded) {
		// make sure that we are using the new format
		wxXmlNode *node = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("ArchiveObject"));
		if(node) {
			node = XmlUtils::FindFirstByTagName(node, wxT("StringMap"));
			if(node) {
				
				// this is an old version, convert it to the new format
				EvnVarList vars;
				std::map<wxString, wxString> envSets;
				wxString content;
				
				wxXmlNode *child = node->GetChildren();
				while(child) {
					if(child->GetName() == wxT("MapEntry")) {
						wxString key = child->GetPropVal(wxT("Key"),   wxT(""));
						wxString val = child->GetPropVal(wxT("Value"), wxT(""));
						content << key << wxT("=") << val << wxT("\n");
					}
					child = child->GetNext();
				}
				envSets[wxT("Default")] = content.Trim().Trim(false);
				vars.SetEnvVarSets(envSets);
				SetSettings(vars);
			}
		}
	}
	return loaded;
}

wxString EnvironmentConfig::ExpandVariables(const wxString &in)
{
	static wxRegEx reVarPattern(wxT("\\$\\(( *)([a-zA-Z0-9_]+)( *)\\)"));
	wxString result(in);

	EnvSetter env(this);

	EvnVarList vars;
	ReadObject(wxT("Variables"), &vars);
	std::map<wxString, wxString> variables = vars.GetEnvVarSets();

	while (reVarPattern.Matches(result)) {
		wxString varName = reVarPattern.GetMatch(result, 2);
		wxString text = reVarPattern.GetMatch(result);

		wxString replacement;
		if(varName == wxT("MAKE")) {
			//ignore this variable, since it is probably was passed here
			//by the makefile generator
			replacement = wxT("___MAKE___");
		}else{
			//search for workspace variable with this name
			wxGetEnv(varName, &replacement);
		}
		result.Replace(text, replacement);
	}

	//restore the ___MAKE___ back to $(MAKE)
	result.Replace(wxT("___MAKE___"), wxT("$(MAKE)"));
	return result;
}

void EnvironmentConfig::ApplyEnv(StringMap *overrideMap)
{
	//read the environments variables
	EvnVarList vars;
	ReadObject(wxT("Variables"), &vars);

	// get the active environment variables set
	std::map<wxString, wxString> variables = vars.GetVariables();

	// if we have an "override map" place all the entries from the override map
	// into the global map before applying the environment
	if(overrideMap) {
		StringMap::iterator it = overrideMap->begin();
		for(; it != overrideMap->end(); it++){
			variables[it->first] = it->second;
		}
	}

	std::map<wxString, wxString>::iterator iter = variables.begin();
	m_envSnapshot.clear();
	for ( ; iter != variables.end(); iter++ ) {
		wxString key = iter->first;
		wxString val = iter->second;

		//keep old value before changing it
		wxString oldVal(wxEmptyString);
		wxGetEnv(key, &oldVal);
		m_envSnapshot[key] = oldVal;

		//allow value to include itself
		//so this is valid:
		//PATH=$(PATH):C:\SomePath
		//but note that the following is not valid: PATH=$(OtherVarName):C:\SomePath
		wxString varName(wxT("$(") + key + wxT(")"));
		val.Replace(varName, oldVal);

		//set the new value
		wxSetEnv(key, val);
	}
}

void EnvironmentConfig::UnApplyEnv()
{
	//loop over the old values and restore them
	StringMap::iterator iter = m_envSnapshot.begin();
	for ( ; iter != m_envSnapshot.end(); iter++ ) {
		wxString key = iter->first;
		wxString value = iter->second;

		wxSetEnv(key, value);
	}
	m_envSnapshot.clear();
}

EvnVarList EnvironmentConfig::GetSettings()
{
	EvnVarList vars;
	ReadObject(wxT("Variables"), &vars);
	return vars;
}

void EnvironmentConfig::SetSettings(EvnVarList &vars)
{
	WriteObject(wxT("Variables"), &vars);
}

