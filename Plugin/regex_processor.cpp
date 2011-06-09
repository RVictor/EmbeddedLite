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
#include "regex_processor.h"
#include "macros.h"
#include "wx/regex.h"

RegexProcessor::RegexProcessor(const wxString &reStr)
{ 
#ifndef __WXMAC__
	m_re = new wxRegEx(reStr);
#else
	m_re = new wxRegEx(reStr, wxRE_ADVANCED);
#endif
}

RegexProcessor::~RegexProcessor()
{
	delete m_re;
}

bool RegexProcessor::GetGroup(const wxString &str, int grp, wxString &out)
{
	if(m_re && m_re->IsValid()){
		if(m_re->Matches(str)){
			out = m_re->GetMatch(str, grp);
			TrimString(out);
			return true;
		}
	}
	return false;
}
