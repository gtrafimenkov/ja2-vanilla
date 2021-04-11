// *****************************************************************************
//
// Filename :	line.h
//
// Purpose :
//
// Modification history :
//
// *****************************************************************************

#ifndef ___LINE___H
#define ___LINE___H

// *****************************************************************************
//
//				Includes
//
// *****************************************************************************

#include "SGP/SGP.h"
#include "SGP/Types.h"

//**************************************************************************
//
//				Example Usage
//
//**************************************************************************

//	// don't send pitch, send width in pixels
//	SetClippingRegionAndImageWidth( uiPitch, 15, 15, 30, 30 );
//
//	LineDraw( TRUE, 10, 10, 200, 200, colour, pImageData);
//    OR
//	RectangleDraw( TRUE, 10, 10, 200, 200, colour, pImageData);

// *****************************************************************************
//
//				Prototypes
//
// *****************************************************************************

// *****************************************************************************

void SetClippingRegionAndImageWidth(int iImageWidth, int iClipStartX, int iClipStartY,
                                    int iClipWidth, int iClipHeight);

// NOTE:
//	Don't send fClip==TRUE to LineDraw if you don't have to. So if you know
//  that your line will be within the region you want it to be in, set
//	fClip == FALSE.
void PixelDraw(BOOLEAN fClip, INT32 xp, INT32 yp, INT16 sColor, UINT8 *pScreen);
void LineDraw(BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color, char *ScreenPtr);
void LineDraw(BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color,
              UINT8 *ScreenPtr);
void LineDraw8(BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color,
               char *ScreenPtr);
void RectangleDraw(BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color,
                   UINT8 *ScreenPtr);
void RectangleDraw8(BOOL fClip, int XStart, int YStart, int XEnd, int YEnd, short Color,
                    UINT8 *ScreenPtr);

// *****************************************************************************

#endif

// EOF *************************************************************************
