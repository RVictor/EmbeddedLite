//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : abbreviationentry.h              
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

#ifndef __abbreviationentry__
#define __abbreviationentry__

#include "serialized_object.h"

class AbbreviationEntry : public SerializedObject
{
	std::map<wxString, wxString> m_entries;

public:
	AbbreviationEntry();
	virtual ~AbbreviationEntry();

public:
	virtual void DeSerialize(Archive &arch);
	virtual void Serialize(Archive &arch);

	void SetEntries(const std::map<wxString, wxString>& entries) {
		this->m_entries = entries;
	}
	const std::map<wxString, wxString>& GetEntries() const {
		return m_entries;
	}
};
#endif // __abbreviationentry__
