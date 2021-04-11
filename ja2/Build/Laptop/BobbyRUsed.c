#include "Laptop/LaptopAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "Laptop/Laptop.h"
#include "Laptop/BobbyRUsed.h"
#include "Laptop/BobbyR.h"
#include "Laptop/BobbyRGuns.h"
#include "Utils/Utilities.h"
#include "SGP/WCheck.h"
#include "Utils/WordWrap.h"
#include "Utils/Cursors.h"
#include "Utils/Text.h"
#include "Laptop/LaptopSave.h"
#endif

UINT32 guiUsedBackground;
UINT32 guiUsedGrid;

void GameInitBobbyRUsed() {}

BOOLEAN EnterBobbyRUsed() {
  VOBJECT_DESC VObjectDesc;

  // load the background graphic and add it
  VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
  FilenameForBPP("LAPTOP\\usedbackground.sti", VObjectDesc.ImageFile);
  CHECKF(AddVideoObject(&VObjectDesc, &guiUsedBackground));

  // load the gunsgrid graphic and add it
  VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
  FilenameForBPP("LAPTOP\\usedgrid.sti", VObjectDesc.ImageFile);
  CHECKF(AddVideoObject(&VObjectDesc, &guiUsedGrid));

  InitBobbyBrTitle();

  SetFirstLastPagesForUsed();
  //	CalculateFirstAndLastIndexs();

  // Draw menu bar
  InitBobbyMenuBar();

  RenderBobbyRUsed();

  return (TRUE);
}

void ExitBobbyRUsed() {
  DeleteVideoObjectFromIndex(guiUsedBackground);
  DeleteVideoObjectFromIndex(guiUsedGrid);
  DeleteBobbyMenuBar();
  DeleteBobbyBrTitle();
  DeleteMouseRegionForBigImage();

  giCurrentSubPage = gusCurWeaponIndex;
  guiLastBobbyRayPage = LAPTOP_MODE_BOBBY_R_USED;
}

void HandleBobbyRUsed() {}

void RenderBobbyRUsed() {
  HVOBJECT hPixHandle;

  WebPageTileBackground(BOBBYR_NUM_HORIZONTAL_TILES, BOBBYR_NUM_VERTICAL_TILES,
                        BOBBYR_BACKGROUND_WIDTH, BOBBYR_BACKGROUND_HEIGHT, guiUsedBackground);

  // Display title at top of page
  DisplayBobbyRBrTitle();

  // GunForm
  GetVideoObject(&hPixHandle, guiUsedGrid);
  BltVideoObject(FRAME_BUFFER, hPixHandle, 0, giOffsW + BOBBYR_GRIDLOC_X,
                 giOffsH + BOBBYR_GRIDLOC_Y, VO_BLT_SRCTRANSPARENCY, NULL);

  DisplayItemInfo(BOBBYR_USED_ITEMS);

  UpdateButtonText(guiCurrentLaptopMode);
  MarkButtonsDirty();
  RenderWWWProgramTitleBar();
  InvalidateRegion(giOffsW + LAPTOP_SCREEN_UL_X, giOffsH + LAPTOP_SCREEN_WEB_UL_Y,
                   giOffsW + LAPTOP_SCREEN_LR_X, giOffsH + LAPTOP_SCREEN_WEB_LR_Y);
}
