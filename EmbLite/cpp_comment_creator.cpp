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
#include "cpp_comment_creator.h"
#include <wx/datetime.h>
#include <vector>
#include "language.h"

#define trimMe(str){\
		str = str.Trim();\
		str = str.Trim(false);\
	}

CppCommentCreator::CppCommentCreator(TagEntryPtr tag, wxChar keyPrefix)
		: CommentCreator(keyPrefix)
		, m_tag(tag)
{
}

CppCommentCreator::~CppCommentCreator()
{
}

wxString CppCommentCreator::CreateComment()
{
	if ( m_tag->GetKind() == wxT("class") || m_tag->GetKind() == wxT("struct") )
		return wxT("$(ClassPattern)\n");
		
	else if (m_tag->GetKind() == wxT("function"))
		return FunctionComment();
		
	else if (m_tag->GetKind() == wxT("prototype"))
		return FunctionComment();
		
	return wxEmptyString;
}

wxString CppCommentCreator::FunctionComment()
{
	wxDateTime now = wxDateTime::Now();
	wxString comment;

	//parse the function signature
	std::vector<TagEntryPtr> tags;
	Language *lang = LanguageST::Get();
	lang->GetLocalVariables(m_tag->GetSignature(), tags);

	comment << wxT("$(FunctionPattern)\n");
	for (size_t i=0; i<tags.size(); i++)
		comment << wxT(" * ") << m_keyPrefix << wxT("param ") << tags.at(i)->GetName() << wxT("\n");


	if (m_tag->GetKind() == wxT("function")) {
		clFunction f;
		if (lang->FunctionFromPattern(m_tag, f)) {
			wxString type = _U(f.m_returnValue.m_type.c_str());
			trimMe(type);
			if (type != wxT("void")){//void has no return value
				comment << wxT(" * ") << m_keyPrefix << wxT("return \n");
			}
		}

	} else {
		Variable var;
		lang->VariableFromPattern(m_tag->GetPattern(), m_tag->GetName(), var);
		wxString type = _U(var.m_type.c_str());
		wxString name = _U(var.m_name.c_str());
		trimMe(type);
		trimMe(name);

		if (	type != wxT("void") //void has no return value
		        && name != type) {	//constructor
			comment << wxT(" * ") << m_keyPrefix << wxT("return \n");
		}
	}
	return comment;
}
