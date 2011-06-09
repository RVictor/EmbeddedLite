#ifndef __includepathlocator__
#define __includepathlocator__

#include <wx/arrstr.h>
#include "imanager.h"

class IncludePathLocator {
	IManager *m_mgr;
public:
	IncludePathLocator(IManager *mgr);
	~IncludePathLocator();
	
	void Locate(wxArrayString &paths, wxArrayString &excludePaths, const wxString& strConfigDir);
};
#endif // __includepathlocator__
