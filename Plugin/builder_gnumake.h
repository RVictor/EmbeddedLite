//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : builder_gnumake.h
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
#ifndef BUILDER_GNUMAKE_H
#define BUILDER_GNUMAKE_H

#include "builder.h"
#include "workspace.h"
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
/*
 * Build using a generated (Gnu) Makefile - this is made as a traditional multistep build :
 *  sources -> (preprocess) -> compile -> link -> exec/lib.
 */
class BuilderGnuMake : public Builder
{
public:
	BuilderGnuMake();
	BuilderGnuMake(const wxString &name, const wxString &buildTool, const wxString &buildToolOptions);
	virtual ~BuilderGnuMake();

	// Implement the Builder Interface
	virtual bool     Export(const wxString &project, const wxString &confToBuild, bool isProjectOnly, bool force, wxString &errMsg);
	virtual wxString GetBuildCommand(const wxString &project, const wxString &confToBuild);
	virtual wxString GetCleanCommand(const wxString &project, const wxString &confToBuild);
	virtual wxString GetPOBuildCommand(const wxString &project, const wxString &confToBuild);
	virtual wxString GetPOCleanCommand(const wxString &project, const wxString &confToBuild);
	virtual wxString GetSingleFileCmd(const wxString &project, const wxString &confToBuild, const wxString &fileName);
	virtual wxString GetPreprocessFileCmd(const wxString &project, const wxString &confToBuild, const wxString &fileName, wxString &errMsg);
	virtual wxString GetPORebuildCommand(const wxString &project, const wxString &confToBuild);

protected:
	virtual void CreateListMacros(ProjectPtr proj, const wxString &confToBuild, wxString &text);
	void CreateSrcList(ProjectPtr proj, const wxString &confToBuild, wxString &text);
	void CreateObjectList(ProjectPtr proj, const wxString &confToBuild, wxString &text);
	virtual void CreateLinkTargets(const wxString &type, BuildConfigPtr bldConf, wxString &text, wxString &targetName);
	virtual void CreateFileTargets(ProjectPtr proj, const wxString &confToBuild, wxString &text);
	void CreateCleanTargets(ProjectPtr proj, const wxString &confToBuild, wxString &text);
	// Override default methods defined in the builder interface
	virtual wxString GetBuildToolCommand(bool isCommandlineCommand) const;

private:
	void GenerateMakefile(ProjectPtr proj, const wxString &confToBuild, bool force);
	void CreateConfigsVariables(ProjectPtr proj, BuildConfigPtr bldConf, wxString &text);
	void CreateMakeDirsTarget(BuildConfigPtr bldConf, const wxString &targetName, wxString &text);
	void CreateTargets(const wxString &type, BuildConfigPtr bldConf, wxString &text);
	void CreatePreBuildEvents(BuildConfigPtr bldConf, wxString &text);
	void CreatePostBuildEvents(BuildConfigPtr bldConf, wxString &text);
	void CreatePreCompiledHeaderTarget(BuildConfigPtr bldConf, wxString &text);
	void CreateCustomPreBuildEvents(BuildConfigPtr bldConf, wxString &text);
	void CreateCustomPostBuildEvents(BuildConfigPtr bldConf, wxString &text);

	wxString GetCdCmd(const wxFileName &path1, const wxFileName &path2);

	wxString ParseIncludePath(const wxString &paths, const wxString &projectName, const wxString &selConf);
	wxString ParseLibPath(const wxString &paths, const wxString &projectName, const wxString &selConf);
	wxString ParseLibs(const wxString &libs);
	wxString ParsePreprocessor(const wxString &prep);
	bool HasPrebuildCommands(BuildConfigPtr bldConf) const;
	wxString GetProjectMakeCommand(const wxFileName &wspfile, const wxFileName& projectPath, ProjectPtr proj, const wxString &confToBuild);
	wxString GetProjectMakeCommand(ProjectPtr proj, const wxString &confToBuild, const wxString &target, bool addCleanTarget, bool cleanOnly);
	wxString DoGetCompilerMacro(const wxString &filename);
};
#endif // BUILDER_GNUMAKE_H
