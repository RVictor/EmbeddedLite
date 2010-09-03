//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : formbuildsettingsdlg.h              
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

#ifndef __formbuildsettingsdlg__
#define __formbuildsettingsdlg__

/**
@file
Subclass of FormBuildSettingsBaseDlg, which is generated by wxFormBuilder.
*/

#include "formbuildsettingsbasedlg.h"
class IManager;

/** Implementing FormBuildSettingsBaseDlg */
class FormBuildSettingsDlg : public FormBuildSettingsBaseDlg
{
	IManager *m_mgr;
	
protected:
	// Handlers for FormBuildSettingsBaseDlg events.
	void OnButtonBrowse( wxCommandEvent& event );
	void OnButtonOK( wxCommandEvent& event );
	void OnButtonCancel( wxCommandEvent& event );
	
public:
	/** Constructor */
	FormBuildSettingsDlg( wxWindow* parent, IManager *mgr );
};

#endif // __formbuildsettingsdlg__