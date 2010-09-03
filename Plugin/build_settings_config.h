//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : build_settings_config.h
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
 #ifndef BUILD_CONFIG_SETTINGS_H
#define BUILD_CONFIG_SETTINGS_H

#include "wx/string.h"
#include "builder.h"
#include "singleton.h"
#include "compiler.h"
#include "wx/xml/xml.h"
#include "wx/filename.h"
#include "build_system.h"

// Cookie class for the editor to provide reentrance operations
// on various methods (such as iteration)
class BuildSettingsConfigCookie {
public:
	wxXmlNode *child;
	wxXmlNode *parent;

public:
	BuildSettingsConfigCookie() : child(NULL), parent(NULL) {}
	~BuildSettingsConfigCookie() {}
};

/**
 * \class BuildSettingsConfig the build system configuration
 */
class BuildSettingsConfig
{
	wxXmlDocument *m_doc;
	wxFileName m_fileName;
protected:
	wxXmlNode* GetCompilerNode(const wxString& name) const;

public:
	BuildSettingsConfig();
	virtual ~BuildSettingsConfig();

	/**
	 * Load the configuration file
	 * @param version XML version which to be loaded, any version different from this one, will cause
	 * codelite to override the user version
	 */
	bool Load(const wxString &version, const wxString& strConfigDir);

	/**
	 * Set or update a given compiler using its name as the index
	 */
	void SetCompiler(CompilerPtr cmp);

	/**
	 * Find and return compiler by name
	 */
	CompilerPtr GetCompiler(const wxString &name) const;

	/**
	 * Returns the first compiler found.
	 * For this enumeration function you must pass in a 'cookie' parameter which is opaque for the application but is necessary for the library to make these functions reentrant
	 * (i.e. allow more than one enumeration on one and the same object simultaneously).
	 */
	CompilerPtr GetFirstCompiler(BuildSettingsConfigCookie &cookie);

	/**
	 * Returns the next compiler.
	 * For this enumeration function you must pass in a 'cookie' parameter which is opaque for the application but is necessary for the library to make these functions reentrant
	 * (i.e. allow more than one enumeration on one and the same object simultaneously).
	 */
	CompilerPtr GetNextCompiler(BuildSettingsConfigCookie &cookie);

	/**
	 * check whether a compiler with a given name already exist
	 */
	bool IsCompilerExist(const wxString &name) const;

	/**
	 * delete compiler
	 */
	void DeleteCompiler(const wxString &name);

	/**
	 * Add build system
	 */
	void SetBuildSystem(BuilderConfigPtr bs);

	/**
	 * get build system from configuration by name
	 */
	BuilderConfigPtr GetBuilderConfig(const wxString &name);
	
	/**
	 * @brief save builder configurtation to the XML file
	 * @param builder
	 */
	void SaveBuilderConfig(BuilderPtr builder);
	
	/*
	 * get name of current selected build system from configuration 
	 */
	 wxString GetSelectedBuildSystem(); 
};

typedef Singleton<BuildSettingsConfig> BuildSettingsConfigST;

#endif //BUILD_CONFIG_SETTINGS_H
