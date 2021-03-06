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
#ifndef DRAWINGUTILS_H
#define DRAWINGUTILS_H

#include "wx/dc.h"
#include "wx/colour.h"

class DrawingUtils
{
public:
	static wxColor LightColour(const wxColour& color, float percent);
	static wxColor DarkColour (const wxColour& color, float percent);
	static wxColor GetPanelBgColour();
	
	static void TruncateText(wxDC& dc, const wxString& text, const int &maxWidth, wxString& fixedText);
	static void PaintStraightGradientBox(wxDC& dc, const wxRect& rect, const wxColour& startColor, const wxColour& endColor, bool vertical);
	static void DrawVerticalButton(wxDC& dc, const wxRect& rect, const bool &focus, const bool &upperTabs, bool vertical, bool hover = false);
	static void DrawHorizontalButton(wxDC& dc, const wxRect& rect, const bool &focus, const bool &upperTabs, bool vertical, bool hover = false);
	static bool IsDark(const wxColour &col);
	static float GetDdkShadowLightFactor();
	static float GetDdkShadowLightFactor2();
	static wxColour GetGradient();
};

#endif //DRAWINGUTILS_H
