/**
  \file 

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include "precompiled_header.h"
#include <map>
#include "ctags_manager.h"
#include "cl_calltip.h"

#ifdef __VISUALC__
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#endif

struct tagCallTipInfo {
	wxString sig;
	wxString retValue;
	std::vector<std::pair<int, int> > paramLen;
};

clCallTip::clCallTip(const std::vector<TagEntryPtr> &tips)
		: m_curr(0)
{
	Initialize(tips);
}

clCallTip::clCallTip(const clCallTip& rhs)
{
	*this = rhs;
}

clCallTip& clCallTip::operator =(const clCallTip& rhs)
{
	if (this == &rhs)
		return *this;
	m_tips = rhs.m_tips;
	return *this;
}

wxString clCallTip::First()
{
	m_curr = 0;
	if (m_tips.empty())
		return wxEmptyString;
	return TipAt(0);
}

wxString clCallTip::TipAt(int at)
{
	wxString tip;
	if ( m_tips.size() > 1 ) {
		tip << m_tips.at(at).str ;
	} else {
		tip << m_tips.at( 0 ).str;
	}
	return tip;
}

wxString clCallTip::Next()
{
	// format a tip string and return it
	if ( m_tips.empty() )
		return wxEmptyString;

	m_curr++;
	if ( m_curr >= (int)m_tips.size() ) {
		m_curr = 0;
	} // if( m_curr >= m_tips.size() )

	return TipAt(m_curr);
}

wxString clCallTip::Prev()
{
	// format a tip string and return it
	if ( m_tips.empty() )
		return wxEmptyString;

	m_curr--;
	if (m_curr < 0) {
		m_curr = (int)m_tips.size()-1;
	}
	return TipAt(m_curr);
}

wxString clCallTip::All()
{
	wxString tip;
	for (size_t i=0; i<m_tips.size(); i++) {
		tip << m_tips.at(i).str << wxT("\n");
	}
	tip.RemoveLast();
	return tip;
}

int clCallTip::Count() const
{
	return (int)m_tips.size();
}

void clCallTip::Initialize(const std::vector<TagEntryPtr> &tips)
{
	std::map<wxString, tagCallTipInfo> mymap;
	for (size_t i=0; i< tips.size(); i++) {
		tagCallTipInfo cti;
		TagEntryPtr t = tips.at(i);
		if ( t->IsMethod() ) {

			wxString raw_sig ( t->GetSignature().Trim().Trim(false) );

			// evaluate the return value of the tag
			cti.retValue = TagsManagerST::Get()->GetFunctionReturnValueFromPattern(t);
			
			bool hasDefaultValues = (raw_sig.Find(wxT("=")) != wxNOT_FOUND);

			// the key for unique entries is the function prototype without the variables names and
			// any default values
			wxString  key           = TagsManagerST::Get()->NormalizeFunctionSig(raw_sig, Normalize_Func_Reverse_Macro);

			// the signature that we want to keep is one with name & default values, so try and get the maximum out of the
			// function signature
			wxString  full_signature = TagsManagerST::Get()->NormalizeFunctionSig(raw_sig, Normalize_Func_Name | Normalize_Func_Default_value | Normalize_Func_Reverse_Macro, &cti.paramLen);
			cti.sig                  = full_signature;

			if (hasDefaultValues) {
				// incase default values exist in this prototype,
				// update/insert this signature
				mymap[key] = cti;
			}

			// make sure we dont add duplicates
			if ( mymap.find(key) == mymap.end() ) {
				// add it
				mymap[key] = cti;
			}

		} else {
			// macro
			wxString macroName = t->GetName();
			wxString pattern = t->GetPattern();

			int where = pattern.Find(macroName);
			if (where != wxNOT_FOUND) {
				//remove the #define <name> from the pattern
				pattern = pattern.Mid(where + macroName.Length());
				pattern = pattern.Trim().Trim(false);
				if (pattern.StartsWith(wxT("("))) {
					//this macro has the form of a function
					pattern = pattern.BeforeFirst(wxT(')'));
					pattern.Append(wxT(')'));
					cti.sig = pattern.Trim().Trim(false);
					mymap[cti.sig] = cti;
				}
			}
		}
	}

	std::map<wxString, tagCallTipInfo>::iterator iter = mymap.begin();
	m_tips.clear();
	for (; iter != mymap.end(); iter++) {
		wxString tip;
		if ( iter->second.retValue.empty() == false ) {
			tip <<  iter->second.retValue.Trim(false).Trim() << wxT(" : ");
		}
		tip << iter->second.sig;
		clTipInfo ti;
		ti.paramLen = iter->second.paramLen;
		ti.str = tip;
		m_tips.push_back(ti);
	}
}

void clCallTip::GetHighlightPos(int index, int& start, int& len)
{
	start = wxNOT_FOUND;
	len   = wxNOT_FOUND;
	if (m_curr >= 0 && m_curr < (int)m_tips.size()) {
		clTipInfo ti = m_tips.at(m_curr);
		int base = ti.str.Find(wxT("("));
		
		// sanity
		if (base != wxNOT_FOUND && index < (int)ti.paramLen.size() && index >= 0) {
			start = ti.paramLen.at(index).first + base;
			len =  ti.paramLen.at(index).second;
		}
	}
}

wxString clCallTip::Current()
{
	// format a tip string and return it
	if ( m_tips.empty() )
		return wxEmptyString;
	
	if ( m_curr >= (int)m_tips.size() || m_curr < 0) {
		m_curr = 0;
	}
	return TipAt(m_curr);
}
