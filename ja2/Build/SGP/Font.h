#ifndef __FONT_H_
#define __FONT_H_

#include "SGP/Types.h"
#include "SGP/HImage.h"
#include "SGP/VObject.h"

#define DEFAULT_SHADOW 2
#define MILITARY_SHADOW 67
#define NO_SHADOW 0

#ifdef JA2

// these are bogus! No palette is set yet!
// font foreground color symbols
#define FONT_FCOLOR_WHITE 208
#define FONT_FCOLOR_RED 162
#define FONT_FCOLOR_NICERED 164
#define FONT_FCOLOR_BLUE 203
#define FONT_FCOLOR_GREEN 184
#define FONT_FCOLOR_YELLOW 144
#define FONT_FCOLOR_BROWN 184
#define FONT_FCOLOR_ORANGE 76
#define FONT_FCOLOR_PURPLE 160

// font background color symbols
#define FONT_BCOLOR_WHITE 208
#define FONT_BCOLOR_RED 162
#define FONT_BCOLOR_BLUE 203
#define FONT_BCOLOR_GREEN 184
#define FONT_BCOLOR_YELLOW 144
#define FONT_BCOLOR_BROWN 80
#define FONT_BCOLOR_ORANGE 76
#define FONT_BCOLOR_PURPLE 160

#else

// font foreground color symbols
#define FONT_FCOLOR_WHITE 0x0000
#define FONT_FCOLOR_RED 0x0000
#define FONT_FCOLOR_BLUE 0x0000
#define FONT_FCOLOR_GREEN 0x0000
#define FONT_FCOLOR_YELLOW 0x0000
#define FONT_FCOLOR_BROWN 0x0000
#define FONT_FCOLOR_ORANGE 0x0000
#define FONT_FCOLOR_PURPLE 0x0000

// font background color symbols
#define FONT_BCOLOR_WHITE 0x0000
#define FONT_BCOLOR_RED 0x0000
#define FONT_BCOLOR_BLUE 0x0000
#define FONT_BCOLOR_GREEN 0x0000
#define FONT_BCOLOR_YELLOW 0x0000
#define FONT_BCOLOR_BROWN 0x0000
#define FONT_BCOLOR_ORANGE 0x0000
#define FONT_BCOLOR_PURPLE 0x0000

// font glyphs for spell targeting types
#define FONT_GLYPH_TARGET_POINT 0xFFF0
#define FONT_GLYPH_TARGET_CONE 0xFFF1
#define FONT_GLYPH_TARGET_SINGLE 0xFFF2
#define FONT_GLYPH_TARGET_GROUP 0xFFF3
#define FONT_GLYPH_TARGET_NONE 0xFFF4

#endif

// typedefs

typedef struct {
  UINT16 usNumberOfSymbols;
  UINT16 *DynamicArrayOf16BitValues;

} FontTranslationTable;

extern INT32 FontDefault;
extern UINT32 FontDestBuffer;
extern UINT32 FontDestPitch;
extern UINT32 FontDestBPP;
extern SGPRect FontDestRegion;
extern BOOLEAN FontDestWrap;

#define SetFontDestObject(x)                                                           \
  (SetFontDestBuffer(x, FontDestRegion.left, FontDestRegion.top, FontDestRegion.right, \
                     FontDestRegion.bottom, FontDestWrap))

#define SetFontDestClip(x1, y1, x2, y2) \
  (SetFontDestBuffer(FontDestBuffer, x1, y1, x2, y2, FontDestWrap))
#define SetFontDestWrap(x)                                                    \
  (SetFontDestBuffer(FontDestBuffer, FontDestRegion.left, FontDestRegion.top, \
                     FontDestRegion.right, FontDestRegion.bottom, x))
extern UINT32 FontDestPitch;
extern SGPRect FontDestRegion;
extern UINT32 SaveFontDestPitch;
extern SGPRect SaveFontDestRegion;

// functions

void SetFontColors(UINT16 usColors);
void SetFontForeground(UINT8 ubForeground);
void SetFontBackground(UINT8 ubBackground);
void SetFontShadow(UINT8 ubBackground);

// Kris:  added these
void SetRGBFontForeground(UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue);
void SetRGBFontBackground(UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue);
void SetRGBFontShadow(UINT32 uiRed, UINT32 uiGreen, UINT32 uiBlue);

BOOLEAN ResetFontObjectPalette(INT32 iFont);
UINT16 *SetFontObjectPalette8BPP(INT32 iFont, SGPPaletteEntry *pPal8);
UINT16 *SetFontObjectPalette16BPP(INT32 iFont, UINT16 *pPal16);
UINT16 *GetFontObjectPalette16BPP(INT32 iFont);

void DestroyEnglishTransTable(void);

extern HVOBJECT GetFontObject(INT32 iFont);
extern UINT32 gprintf(INT32 x, INT32 y, STR16 pFontString, ...);
extern UINT32 gprintfDirty(INT32 x, INT32 y, STR16 pFontString, ...);
extern UINT32 mprintf(INT32 x, INT32 y, STR16 pFontString, ...);
extern UINT32 gprintf_buffer(UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x,
                             INT32 y, STR16 pFontString, ...);
extern UINT32 mprintf_buffer(UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x,
                             INT32 y, STR16 pFontString, ...);

// Function for displaying coded test. Since it's slower to do this, it's separate from  the normal
// fuctions
#define FONT_CODE_BEGINCOLOR 180
#define FONT_CODE_RESETCOLOR 181

UINT32 mprintf_buffer_coded(UINT8 *pDestBuf, UINT32 uiDestPitchBYTES, UINT32 FontType, INT32 x,
                            INT32 y, STR16 pFontString, ...);
UINT32 mprintf_coded(INT32 x, INT32 y, STR16 pFontString, ...);

extern BOOLEAN SetFontDestBuffer(UINT32 DestBuffer, INT32 x1, INT32 y1, INT32 x2, INT32 y2,
                                 BOOLEAN wrap);
extern BOOLEAN SetFont(INT32 iFontIndex);

extern INT32 LoadFontFile(STR8 pFileName);
extern UINT16 GetFontHeight(INT32 FontNum);
extern BOOLEAN InitializeFontManager(UINT16 usDefaultPixDepth, FontTranslationTable *pTransTable);
extern void ShutdownFontManager(void);
extern void UnloadFont(UINT32 FontIndex);

extern FontTranslationTable *CreateEnglishTransTable();

extern INT16 GetIndex(UINT16 siChar);
extern UINT32 GetWidth(HVOBJECT hSrcVObject, INT16 ssIndex);

extern INT16 StringPixLengthArgFastHelp(INT32 usUseFont, INT32 usBoldFont, UINT32 uiCharCount,
                                        STR16 pFontString);
extern INT16 StringPixLengthArg(INT32 usUseFont, UINT32 uiCharCount, STR16 pFontString, ...);
extern INT16 StringPixLength(STR16 string, INT32 UseFont);
extern INT16 StringNPixLength(STR16 string, UINT32 uiMaxCount, INT32 UseFont);
extern void SaveFontSettings(void);
extern void RestoreFontSettings(void);

void VarFindFontRightCoordinates(INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight,
                                 INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY, STR16 pFontString,
                                 ...);
void VarFindFontCenterCoordinates(INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight,
                                  INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY, STR16 pFontString,
                                  ...);
// void FindFontRightCoordinates( INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, STR16 pStr,
// INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY );
void FindFontRightCoordinates(INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, STR16 pStr,
                              INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY);
void FindFontCenterCoordinates(INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, STR16 pStr,
                               INT32 iFontIndex, INT16 *psNewX, INT16 *psNewY);
void FindFontCenterCoordinates(INT16 sLeft, INT16 sTop, INT16 sWidth, INT16 sHeight, STR16 pStr,
                               INT32 iFontIndex, UINT16 *psNewX, UINT16 *psNewY);
// extern FontBase *LoadFontFile(UINT8 *pFileName);
// extern UINT8    *GetFontPalette(UINT8 *pFileName);
// extern UINT16    GetMaxFontWidth(FontBase *pBase);
// extern void      UnloadFont(FontBase *pBase);
// extern BOOLEAN   SetFontPalette(FontBase *pFont, UINT16 siDepthPix, SGPPaletteEntry
// *pNewPalette);
// make sure the pFontString is terminated by 0
// extern BOOLEAN   PrintFontString(STR16 pFontString, UINT8 *pDestBuffer, UINT16 siDestWidth,
// UINT16 siDestPixelDepth, UINT16 siDestPitch, UINT16 siDestHeight, UINT16 siX, UINT16 siY, UINT16
// siTotalWidth, UINT16 siTotalHeight, BOOLEAN MultiLine, FontBase *pFontBase); extern BOOLEAN
// SetFont16BitData(FontBase *pFontBase, UINT16 *pData16);

#endif
