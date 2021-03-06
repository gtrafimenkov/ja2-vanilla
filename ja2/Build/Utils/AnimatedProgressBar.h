#ifndef __ANIMATED_PROGRESSBAR_H
#define __ANIMATED_PROGRESSBAR_H

#include "SGP/Types.h"

#define MAX_PROGRESSBARS 4

typedef struct PROGRESSBAR {
  UINT8 ubProgressBarID;
  UINT16 usBarLeft, usBarTop, usBarRight, usBarBottom;
  BOOLEAN fPanel;
  UINT16 usPanelLeft, usPanelTop, usPanelRight, usPanelBottom;
  UINT16 usColor, usLtColor, usDkColor;
  STR16 swzTitle;
  UINT16 usTitleFont;
  UINT8 ubTitleFontForeColor, ubTitleFontShadowColor;
  UINT16 usMsgFont;
  UINT8 ubMsgFontForeColor, ubMsgFontShadowColor;
  UINT8 ubRelativeStartPercentage, ubRelativeEndPercentage;
  UINT8 ubColorFillRed;
  UINT8 ubColorFillGreen;
  UINT8 ubColorFillBlue;
  double rStart, rEnd;
  BOOLEAN fDisplayText;
  BOOLEAN fUseSaveBuffer;  // use the save buffer when display the text
  double rLastActual;
} PROGRESSBAR;

extern PROGRESSBAR* pBar[MAX_PROGRESSBARS];

void CreateLoadingScreenProgressBar();
void RemoveLoadingScreenProgressBar();

// This creates a single progress bar given the coordinates without a panel (containing a title and
// background). A panel is automatically created if you specify a title using SetProgressBarTitle
BOOLEAN CreateProgressBar(UINT8 ubProgressBarID, UINT16 usLeft, UINT16 usTop, UINT16 usRight,
                          UINT16 usBottom);

// You may also define a panel to go in behind the progress bar.  You can now assign a title to go
// with the panel.
void DefineProgressBarPanel(UINT32 ubID, UINT8 r, UINT8 g, UINT8 b, UINT16 usLeft, UINT16 usTop,
                            UINT16 usRight, UINT16 usBottom);

// Assigning a title for the panel will automatically position the text horizontally centered on the
// panel and vertically centered from the top of the panel, to the top of the progress bar.
void SetProgressBarTitle(UINT32 ubID, STR16 pString, UINT32 usFont, UINT8 ubForeColor,
                         UINT8 ubShadowColor);

// Unless you set up the attributes, any text you pass to SetRelativeStartAndEndPercentage will
// default to FONT12POINT1 in a black color.
void SetProgressBarMsgAttributes(UINT32 ubID, UINT32 usFont, UINT8 ubForeColor,
                                 UINT8 ubShadowColor);

// When finished, the progress bar needs to be removed.
void RemoveProgressBar(UINT8 ubID);

// An important setup function.  The best explanation is through example.  The example being the
// loading of a file -- there are many stages of the map loading.  In JA2, the first step is to load
// the tileset. Because it is a large chunk of the total loading of the map, we may gauge that it
// takes up 30% of the total load.  Because it is also at the beginning, we would pass in the
// arguments ( 0, 30, "text" ). As the process animates using UpdateProgressBar( 0 to 100 ), the
// total progress bar will only reach 30% at the 100% mark within UpdateProgressBar.  At that time,
// you would go onto the next step, resetting the relative start and end percentage from 30 to
// whatever, until your done.
void SetRelativeStartAndEndPercentage(UINT8 ubID, UINT32 uiRelStartPerc, UINT32 uiRelEndPerc,
                                      STR16 str);

// This part renders the progress bar at the percentage level that you specify.  If you have set
// relative percentage values in the above function, then the uiPercentage will be reflected based
// off of the relative percentages.
void RenderProgressBar(UINT8 ubID, UINT32 uiPercentage);

// Sets the color of the progress bars main color.
void SetProgressBarColor(UINT8 ubID, UINT8 ubColorFillRed, UINT8 ubColorFillGreen,
                         UINT8 ubColorFillBlue);

// Pass in TRUE to display the strings.
void SetProgressBarTextDisplayFlag(UINT8 ubID, BOOLEAN fDisplayText, BOOLEAN fUseSaveBuffer,
                                   BOOLEAN fSaveScreenToFrameBuffer);

#endif
