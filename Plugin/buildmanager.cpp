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
#include "buildmanager.h"
#include "builder.h"
#include "builder_gnumake.h"
#include "builder_gnumake_onestep.h"


BuildManager::BuildManager()
{
	// register all builders here
	AddBuilder(new BuilderGnuMake());
	AddBuilder(new BuilderGnuMakeOneStep());
}

BuildManager::~BuildManager()
{
	m_builders.clear();
}

void BuildManager::AddBuilder(BuilderPtr builder)
{
	if(!builder){
		return;
	}

	m_builders[builder->GetName()] = builder;
}

void BuildManager::RemoveBuilder(const wxString &name)
{

	std::map<wxString, BuilderPtr>::iterator iter = m_builders.find(name);
	if(iter != m_builders.end()){
		m_builders.erase(iter);
	}
}

void BuildManager::GetBuilders(std::list<wxString> &list)
{
	std::map<wxString, BuilderPtr>::iterator iter = m_builders.begin();
	for(; iter != m_builders.end(); iter++){
		list.push_back(iter->second->GetName());
	}
}

BuilderPtr BuildManager::GetBuilder(const wxString &name)
{
	std::map<wxString, BuilderPtr>::iterator iter = m_builders.begin();
	for(; iter != m_builders.end(); iter++){
		if(iter->first == name){
			return iter->second;
		}
	}
	
	// return the default builder
	return m_builders.begin()->second;
}

BuilderPtr BuildManager::GetSelectedBuilder()
{
	// Gnu Makefile C/C++ is the default builder
	BuilderPtr defaultBuilder = m_builders.begin()->second;
	
	std::list<wxString> builders;
	GetBuilders(builders);
	
	std::list<wxString>::iterator iter = builders.begin();
	for(; iter != builders.end(); iter++) {
		
		wxString builderName = *iter;
		BuilderPtr builder = BuildManagerST::Get()->GetBuilder(builderName);
		if(builder->IsActive())
			return builder;
		
	}
	
	return defaultBuilder;
}
