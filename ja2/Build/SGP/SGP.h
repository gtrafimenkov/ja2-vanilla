#ifndef __SGP_
#define __SGP_

#include "Local.h"
#include "SGP/Types.h"
#include "SGP/Timer.h"
#include "SGP/Debug.h"

#include "SysGlobals.h"

#if defined(JA2) || defined(UTIL)
#include "SGP/Video.h"
#else
#include "video2.h"
#endif

#ifndef JA2
#include "SGP/Input.h"
#include "SGP/MemMan.h"
#include "SGP/FileMan.h"
#include "SGP/DBMan.h"
#include "SGP/SoundMan.h"
#include "SGP/PCX.h"
#include "SGP/Line.h"
#include "GameLoop.h"
#include "SGP/Font.h"
#include "SGP/English.h"
#include "SGP/MutexManager.h"
#include "SGP/VObject.h"
#include "SGP/Random.h"
#include "SGP/Shading.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern BOOLEAN gfProgramIsRunning;  // Turn this to FALSE to exit program
// extern UINT32			giStartMem;
extern CHAR8 gzCommandLine[100];  // Command line given
extern UINT8 gbPixelDepth;        // GLOBAL RUN-TIME SETTINGS
extern BOOLEAN gfDontUseDDBlits;  // GLOBAL FOR USE OF DD BLITTING

//************************************************
extern INT32 giScrW;  // разрешение экрана по горизонтали
extern INT32 giScrH;  // разрешение экрана по вертикали

extern INT32 giOffsW;  // смещение рабочей области экрана по горизонтали
extern INT32 giOffsH;  // смещение рабочей области экрана по вертикали

//***18.11.2008***
extern INT16 gsRenderOffsetX;  //смещение спрайтов фигурки и курсоров в renderworld.c
extern INT16 gsRenderOffsetY;  //смещение спрайтов фигурки и курсоров в renderworld.c
//*******************************************

#if !defined(JA2) && !defined(UTILS)
extern BOOLEAN gfLoadAtStartup;
extern CHAR8 *gzStringDataOverride;
extern BOOLEAN gfUsingBoundsChecker;
extern BOOLEAN gfCapturingVideo;

#endif

// function prototypes
void SGPExit(void);
void ShutdownWithErrorBox(CHAR8 *pcMessage);

#ifdef __cplusplus
}
#endif

#endif
