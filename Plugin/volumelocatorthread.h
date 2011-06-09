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
#ifdef __WXMSW__


#ifndef VOLUMELOCATORTHREAD_H
#define VOLUMELOCATORTHREAD_H

#include "wx/thread.h"
#include "wx/event.h"

#if wxUSE_FSVOLUME
#define wxEVT_THREAD_VOLUME_COMPLETED 3453

class VolumeLocatorThread : public wxThread
{
	wxEvtHandler *m_owner;

protected:
	void* Entry();

public:
	VolumeLocatorThread(wxEvtHandler *owner);
	~VolumeLocatorThread();
};

#endif //VOLUMELOCATORTHREAD_H
#endif // wxUSE_FSVOLUME
#endif //__WXMSW__



