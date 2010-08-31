//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : buildmanager.h
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
 #ifndef BUILDMANAGER_H
#define BUILDMANAGER_H

#include <map>
#include <list>
#include "wx/string.h"
#include "singleton.h"
#include "builder.h"

//class BuildManager;

/**
 * \ingroup SDK
 * The build manager class which contains all the builders currently
 * available to the editor
 *
 * Each and every call to this class is protected by a critical section to make this
 * class safe when used inside the compiler thread
 *
 * \version 1.0
 * first version
 *
 * \date 05-19-2007
 *
 * \author Eran
 *
 */
class BuildManager {

	friend class Singleton<BuildManager>;
	std::map<wxString, BuilderPtr> m_builders;

public:
	typedef std::map<wxString, BuilderPtr>::const_iterator ConstIterator;

private:
	BuildManager();
	virtual ~BuildManager();

public:
	void AddBuilder(BuilderPtr builder);
	void RemoveBuilder(const wxString &name);
	void GetBuilders(std::list<wxString> &list);
	BuilderPtr GetBuilder(const wxString &name);
	BuilderPtr GetSelectedBuilder();
};

typedef Singleton<BuildManager> BuildManagerST;

#endif // BUILDMANAGER_H

