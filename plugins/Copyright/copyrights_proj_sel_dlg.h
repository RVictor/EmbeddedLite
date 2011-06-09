//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : copyrights_proj_sel_dlg.h              
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
 #ifndef __copyrights_proj_sel_dlg__
#define __copyrights_proj_sel_dlg__

/**
@file
Subclass of CopyrightsProjectSelBaseDlg, which is generated by wxFormBuilder.
*/

#include "copyrights_proj_sel_base_dlg.h"
class CSolution;
/** Implementing CopyrightsProjectSelBaseDlg */
class CopyrightsProjectSelDlg : public CopyrightsProjectSelBaseDlg
{
protected:
	void OnUnCheckAll(wxCommandEvent &e);
	void OnCheckAll(wxCommandEvent &e);
	
public:
	/** Constructor */
	CopyrightsProjectSelDlg(wxWindow* parent, CSolution* wsp);
	void GetProjects(wxArrayString &projects);
};

#endif // __copyrights_proj_sel_dlg__
