//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : environmentconfig.h
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
 #ifndef __environmentconfig__
#define __environmentconfig__

#include "configurationtoolbase.h"
#include "archive.h"
#include "evnvarlist.h"

class EnvironmentConfig : public ConfigurationToolBase {

	static EnvironmentConfig* ms_instance;
	StringMap m_envSnapshot;

public:
	static EnvironmentConfig* Instance();
	static void Release();
	bool Load(const wxString& strConfigDir);
	wxString ExpandVariables(const wxString &in);
	void ApplyEnv(StringMap *overrideMap);
	void UnApplyEnv();
	EvnVarList GetSettings();
	void       SetSettings(EvnVarList &vars);
private:
	EnvironmentConfig();
	virtual ~EnvironmentConfig();

public:
	virtual wxString GetRootName();
};

class EnvSetter
{
	EnvironmentConfig *m_env;
public:
	EnvSetter(StringMap *om = NULL) : m_env(EnvironmentConfig::Instance()) {
		m_env->ApplyEnv(om);
	}

	EnvSetter(EnvironmentConfig *conf, StringMap *om = NULL) : m_env(conf) {
		if(m_env) {
			m_env->ApplyEnv(om);
		}
	}
	~EnvSetter() {
		if(m_env) {
			m_env->UnApplyEnv();
			m_env = NULL;
		}
	}
};
#endif // __environmentconfig__
