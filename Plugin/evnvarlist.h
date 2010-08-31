//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : evnvarlist.h
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
#ifndef __evnvarlist__
#define __evnvarlist__

#include "serialized_object.h"
#include <set>

class EvnVarList : public SerializedObject
{
	std::map<wxString, wxString> m_envVarSets;
	wxString                     m_activeSet;

protected:
	wxString DoGetSetVariablesStr(const wxString &setName, wxString &selectedSetName);

public:
	EvnVarList();
	virtual ~EvnVarList();

	void SetActiveSet(const wxString& activeSet) {
		if(activeSet != wxT("<Use Active Set>"))
			this->m_activeSet = activeSet;
	}
	
	const wxString& GetActiveSet() const {
		return m_activeSet;
	}
	void SetEnvVarSets(const std::map<wxString, wxString>& envVarSets) {
		this->m_envVarSets = envVarSets;
	}
	const std::map<wxString, wxString>& GetEnvVarSets() const {
		return m_envVarSets;
	}

	void AddVariable(const wxString &setName, const wxString &name, const wxString &value);
	std::map<wxString, wxString> GetVariables(const wxString &setName = wxT(""));
	bool IsSetExist(const wxString &setName);
public:
	virtual void DeSerialize(Archive &arch);
	virtual void Serialize  (Archive &arch);
};
#endif // __evnvarlist__
