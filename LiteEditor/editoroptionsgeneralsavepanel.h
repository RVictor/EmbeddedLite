//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : editoroptionsgeneralsavepanel.h              
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

#ifndef __editoroptionsgeneralsavepanel__
#define __editoroptionsgeneralsavepanel__

/**
@file
Subclass of EditorOptionsGeneralSavePanelBase, which is generated by wxFormBuilder.
*/

#include "editoroptionsgeneralsavepanelbase.h"
#include "treebooknodebase.h"

/** Implementing EditorOptionsGeneralSavePanelBase */
class EditorOptionsGeneralSavePanel
: public EditorOptionsGeneralSavePanelBase
, public TreeBookNode<EditorOptionsGeneralSavePanel>
{
public:
	/** Constructor */
	EditorOptionsGeneralSavePanel( wxWindow* parent );
	void Save(OptionsConfigPtr options);
};

#endif // __editoroptionsgeneralsavepanel__
