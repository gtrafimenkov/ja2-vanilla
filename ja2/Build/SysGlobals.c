#include "JA2All.h"
#ifdef PRECOMPILEDHEADERS
#else
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include "SGP/Types.h"
#include "ScreenIDs.h"
#include "SysGlobals.h"
#include "GameLoop.h"
#endif

// External globals
CHAR8 gubErrorText[200];
CHAR8 gubFilename[200];
BOOLEAN gfEditMode = FALSE;
CHAR8 gDebugStr[128];
CHAR8 gSystemDebugStr[128];
INT8 gbFPSDisplay = SHOW_MIN_FPS;
BOOLEAN gfResetInputCheck = FALSE;
BOOLEAN gfGlobalError = FALSE;

UINT32 guiGameCycleCounter = 0;

BOOLEAN SET_ERROR(const char *String, ...) {
  va_list ArgPtr;

  va_start(ArgPtr, String);
  vsprintf(gubErrorText, String, ArgPtr);
  va_end(ArgPtr);

  SetPendingNewScreen(ERROR_SCREEN);

  gfGlobalError = TRUE;

  return (FALSE);
}
