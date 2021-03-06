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
#include "quickdebuginfo.h"
QuickDebugInfo::QuickDebugInfo()
: m_selectedDbg(0)
{
}

QuickDebugInfo::~QuickDebugInfo()
{
}

void QuickDebugInfo::DeSerialize(Archive& arch)
{
	arch.Read(wxT("m_arguments"), m_arguments);
	arch.Read(wxT("m_exePath"), m_exePath);
	arch.Read(wxT("m_selectedDbg"), m_selectedDbg);
	arch.Read(wxT("m_startCmds"), m_startCmds);
	arch.Read(wxT("m_wd"), m_wd);
}
void QuickDebugInfo::Serialize(Archive& arch)
{
	arch.Write(wxT("m_arguments"), m_arguments);
	arch.Write(wxT("m_exePath"), m_exePath);
	arch.Write(wxT("m_selectedDbg"), m_selectedDbg);
	arch.Write(wxT("m_startCmds"), m_startCmds);
	arch.Write(wxT("m_wd"), m_wd);
}
