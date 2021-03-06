#include "SGP/SGPAll.h"
#include "Res/Resource.h"
#ifdef PRECOMPILEDHEADERS
#elif defined(WIZ8_PRECOMPILED_HEADERS)
#include "WIZ8 SGP ALL.H"
#else
#include "SGP/Types.h"
#include "SGP/Video.h"
#include "SGP/VObjectBlitters.h"
#include "SGP/SGP.h"
#include <stdio.h>
#include "TileEngine/RenderWorld.h"
#include "TileEngine/RenderDirty.h"
#include "TileEngine/IsometricUtils.h"
#include "FadeScreen.h"
#include "SGP/ImpTGA.h"
#include "Utils/TimerControl.h"
#include "SGP/FileMan.h"
#endif

#ifndef _MT
#define _MT
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Local Defines
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_DIRTY_REGIONS 128

#define VIDEO_OFF 0x00
#define VIDEO_ON 0x01
#define VIDEO_SHUTTING_DOWN 0x02
#define VIDEO_SUSPENDED 0x04

#define THREAD_OFF 0x00
#define THREAD_ON 0x01
#define THREAD_SUSPENDED 0x02

#define CURRENT_MOUSE_DATA 0
#define PREVIOUS_MOUSE_DATA 1

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Local Typedefs
//
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  BOOLEAN fRestore;
  UINT16 usMouseXPos, usMouseYPos;
  UINT16 usLeft, usTop, usRight, usBottom;
  RECT Region;
  LPDIRECTDRAWSURFACE _pSurface;
  LPDIRECTDRAWSURFACE2 pSurface;

} MouseCursorBackground;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// LOCAL globals
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//
// Video state variables
//

static UINT16 gusScreenWidth;
static UINT16 gusScreenHeight;
static UINT8 gubScreenPixelDepth;

static RECT gScrollRegion;

#define MAX_NUM_FRAMES 25

BOOLEAN gfVideoCapture = FALSE;
UINT32 guiFramePeriod = (1000 / 15);
UINT32 guiLastFrame;
UINT16 *gpFrameData[MAX_NUM_FRAMES];
INT32 giNumFrames = 0;

//
// Direct Draw objects for both the Primary and Backbuffer surfaces
//

static LPDIRECTDRAW _gpDirectDrawObject = NULL;
static LPDIRECTDRAW2 gpDirectDrawObject = NULL;

static LPDIRECTDRAWSURFACE _gpPrimarySurface = NULL;
static LPDIRECTDRAWSURFACE2 gpPrimarySurface = NULL;
static LPDIRECTDRAWSURFACE2 gpBackBuffer = NULL;

//
// Direct Draw Objects for the frame buffer
//

static LPDIRECTDRAWSURFACE _gpFrameBuffer = NULL;
static LPDIRECTDRAWSURFACE2 gpFrameBuffer = NULL;

#ifdef WINDOWED_MODE

static LPDIRECTDRAWSURFACE _gpBackBuffer = NULL;

extern RECT rcWindow;

#endif

//
// Globals for mouse cursor
//

static UINT16 gusMouseCursorWidth;
static UINT16 gusMouseCursorHeight;
static INT16 gsMouseCursorXOffset;
static INT16 gsMouseCursorYOffset;

static LPDIRECTDRAWSURFACE _gpMouseCursor = NULL;
static LPDIRECTDRAWSURFACE2 gpMouseCursor = NULL;

static LPDIRECTDRAWSURFACE _gpMouseCursorOriginal = NULL;
static LPDIRECTDRAWSURFACE2 gpMouseCursorOriginal = NULL;

static MouseCursorBackground gMouseCursorBackground[2];

static HVOBJECT gpCursorStore;

BOOLEAN gfFatalError = FALSE;
char gFatalErrorString[512];

// 8-bit palette stuff

SGPPaletteEntry gSgpPalette[256];
LPDIRECTDRAWPALETTE gpDirectDrawPalette;

//
// Make sure we record the value of the hWindow (main window frame for the application)
//

HWND ghWindow;

//
// Refresh thread based variables
//

UINT32 guiFrameBufferState;    // BUFFER_READY, BUFFER_DIRTY
UINT32 guiMouseBufferState;    // BUFFER_READY, BUFFER_DIRTY, BUFFER_DISABLED
UINT32 guiVideoManagerState;   // VIDEO_ON, VIDEO_OFF, VIDEO_SUSPENDED, VIDEO_SHUTTING_DOWN
UINT32 guiRefreshThreadState;  // THREAD_ON, THREAD_OFF, THREAD_SUSPENDED

//
// Dirty rectangle management variables
//

void (*gpFrameBufferRefreshOverride)(void);
SGPRect gListOfDirtyRegions[MAX_DIRTY_REGIONS];
UINT32 guiDirtyRegionCount;
BOOLEAN gfForceFullScreenRefresh;

SGPRect gDirtyRegionsEx[MAX_DIRTY_REGIONS];
UINT32 gDirtyRegionsFlagsEx[MAX_DIRTY_REGIONS];
UINT32 guiDirtyRegionExCount;

SGPRect gBACKUPListOfDirtyRegions[MAX_DIRTY_REGIONS];
UINT32 gBACKUPuiDirtyRegionCount;
BOOLEAN gBACKUPfForceFullScreenRefresh;

//
// Screen output stuff
//

BOOLEAN gfPrintFrameBuffer;
UINT32 guiPrintFrameBufferIndex;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// External Variables
//
///////////////////////////////////////////////////////////////////////////////////////////////////

extern UINT16 gusRedMask;
extern UINT16 gusGreenMask;
extern UINT16 gusBlueMask;
extern INT16 gusRedShift;
extern INT16 gusBlueShift;
extern INT16 gusGreenShift;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Local Function Prototypes
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void AddRegionEx(INT32 iLeft, INT32 iTop, INT32 iRight, INT32 iBottom, UINT32 uiFlags);
void SnapshotSmall(void);
void VideoMovieCapture(BOOLEAN fEnable);
void RefreshMovieCache();

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN InitializeVideoManager(HINSTANCE hInstance, UINT16 usCommandShow, void *WindowProc) {
  UINT32 uiIndex, uiPitch;
  HRESULT ReturnCode;
  HWND hWindow;
  WNDCLASS WindowClass;
  CHAR8 ClassName[] = APPLICATION_NAME;
  DDSURFACEDESC SurfaceDescription;
  DDCOLORKEY ColorKey;
  PTR pTmpPointer;

#ifndef WINDOWED_MODE
  DDSCAPS SurfaceCaps;
#endif

  //
  // Register debug topics
  //

  RegisterDebugTopic(TOPIC_VIDEO, "Video");
  DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Initializing the video manager");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Register and Realize our display window. The DirectX surface will eventually overlay on top
  // of this surface.
  //
  // <<<<<<<<< Don't change this >>>>>>>>
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////

  WindowClass.style = CS_HREDRAW | CS_VREDRAW;
  WindowClass.lpfnWndProc = (WNDPROC)WindowProc;
  WindowClass.cbClsExtra = 0;
  WindowClass.cbWndExtra = 0;
  WindowClass.hInstance = hInstance;
  WindowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
  WindowClass.hCursor = NULL;
  WindowClass.hbrBackground = NULL;
  WindowClass.lpszMenuName = NULL;
  WindowClass.lpszClassName = ClassName;
  RegisterClass(&WindowClass);

  //
  // Get a window handle for our application (gotta have on of those)
  // Don't change this
  //
#ifdef WINDOWED_MODE
  hWindow = CreateWindowEx(0, ClassName, "Windowed JA2 !!", WS_POPUP, 0, 0, SCREEN_WIDTH,
                           SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
#else
  hWindow = CreateWindowEx(WS_EX_TOPMOST, ClassName, ClassName, WS_POPUP | WS_VISIBLE, 0, 0,
                           GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL,
                           hInstance, NULL);
#endif
  if (hWindow == NULL) {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to create window frame for Direct Draw");
    return FALSE;
  }

  //
  // Excellent. Now we record the hWindow variable for posterity (not)
  //

  memset(gpFrameData, 0, sizeof(gpFrameData));

  ghWindow = hWindow;

  //
  // Display our full screen window
  //

  ShowCursor(FALSE);
  ShowWindow(hWindow, usCommandShow);
  UpdateWindow(hWindow);
  SetFocus(hWindow);

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Start up Direct Draw
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////

  //
  // Create the Direct Draw Object
  //

  ReturnCode = DirectDrawCreate(NULL, &_gpDirectDrawObject, NULL);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  ReturnCode = IDirectDraw_QueryInterface(_gpDirectDrawObject, (const IID &)IID_IDirectDraw2,
                                          (LPVOID *)&gpDirectDrawObject);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  //
  // Set the exclusive mode
  //
#ifdef WINDOWED_MODE
  ReturnCode = IDirectDraw2_SetCooperativeLevel(gpDirectDrawObject, ghWindow, DDSCL_NORMAL);
#else
  ReturnCode = IDirectDraw2_SetCooperativeLevel(gpDirectDrawObject, ghWindow,
                                                DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
#endif
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  //
  // Set the display mode
  //
#ifndef WINDOWED_MODE
  ReturnCode = IDirectDraw2_SetDisplayMode(gpDirectDrawObject, SCREEN_WIDTH, SCREEN_HEIGHT,
                                           gbPixelDepth, 0, 0);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }
#endif

  gusScreenWidth = SCREEN_WIDTH;
  gusScreenHeight = SCREEN_HEIGHT;
  gubScreenPixelDepth = gbPixelDepth;

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Setup all the surfaces
  //
  /////////////////////////////////////////////////////////////////////////////////////////////////

  //
  // Initialize Primary Surface along with BackBuffer
  //

  ZEROMEM(SurfaceDescription);
#ifdef WINDOWED_MODE

  // Create a primary surface and a backbuffer in system memory
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  SurfaceDescription.dwFlags = DDSD_CAPS;
  SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

  ReturnCode =
      IDirectDraw2_CreateSurface(gpDirectDrawObject, &SurfaceDescription, &_gpPrimarySurface, NULL);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  ReturnCode = IDirectDrawSurface_QueryInterface(
      _gpPrimarySurface, (const IID &)IID_IDirectDrawSurface2, (LPVOID *)&gpPrimarySurface);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  // Backbuffer
  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  SurfaceDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  SurfaceDescription.dwWidth = SCREEN_WIDTH;
  SurfaceDescription.dwHeight = SCREEN_HEIGHT;
  ReturnCode =
      IDirectDraw2_CreateSurface(gpDirectDrawObject, &SurfaceDescription, &_gpBackBuffer, NULL);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  ReturnCode = IDirectDrawSurface_QueryInterface(
      _gpBackBuffer, (const IID &)IID_IDirectDrawSurface2, (LPVOID *)&gpBackBuffer);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

#else
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  SurfaceDescription.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
  SurfaceDescription.dwBackBufferCount = 1;

  ReturnCode =
      IDirectDraw2_CreateSurface(gpDirectDrawObject, &SurfaceDescription, &_gpPrimarySurface, NULL);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  ReturnCode = IDirectDrawSurface_QueryInterface(
      _gpPrimarySurface, (const IID &)IID_IDirectDrawSurface2, (LPVOID *)&gpPrimarySurface);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  SurfaceCaps.dwCaps = DDSCAPS_BACKBUFFER;
  ReturnCode =
      IDirectDrawSurface2_GetAttachedSurface(gpPrimarySurface, &SurfaceCaps, &gpBackBuffer);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

#endif

  //
  // Initialize the frame buffer
  //

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  SurfaceDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  SurfaceDescription.dwWidth = SCREEN_WIDTH;
  SurfaceDescription.dwHeight = SCREEN_HEIGHT;
  ReturnCode =
      IDirectDraw2_CreateSurface(gpDirectDrawObject, &SurfaceDescription, &_gpFrameBuffer, NULL);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  ReturnCode = IDirectDrawSurface_QueryInterface(
      _gpFrameBuffer, (const IID &)IID_IDirectDrawSurface2, (LPVOID *)&gpFrameBuffer);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  //
  // Blank out the frame buffer
  //

  pTmpPointer = LockFrameBuffer(&uiPitch);
  memset(pTmpPointer, 0, 480 * uiPitch);
  UnlockFrameBuffer();

  //
  // Initialize the main mouse surfaces
  //

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  SurfaceDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  // SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  SurfaceDescription.dwWidth = MAX_CURSOR_WIDTH;
  SurfaceDescription.dwHeight = MAX_CURSOR_HEIGHT;
  ReturnCode =
      IDirectDraw2_CreateSurface(gpDirectDrawObject, &SurfaceDescription, &_gpMouseCursor, NULL);
  if (ReturnCode != DD_OK) {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0,
             String("Failed to create MouseCursor witd %ld", ReturnCode & 0x0f));
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  ReturnCode = IDirectDrawSurface_QueryInterface(
      _gpMouseCursor, (const IID &)IID_IDirectDrawSurface2, (LPVOID *)&gpMouseCursor);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  ColorKey.dwColorSpaceLowValue = 0;
  ColorKey.dwColorSpaceHighValue = 0;
  ReturnCode = IDirectDrawSurface2_SetColorKey(gpMouseCursor, DDCKEY_SRCBLT, &ColorKey);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  //
  // Initialize the main mouse original surface
  //

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  SurfaceDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  SurfaceDescription.dwWidth = MAX_CURSOR_WIDTH;
  SurfaceDescription.dwHeight = MAX_CURSOR_HEIGHT;
  ReturnCode = IDirectDraw2_CreateSurface(gpDirectDrawObject, &SurfaceDescription,
                                          &_gpMouseCursorOriginal, NULL);
  if (ReturnCode != DD_OK) {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to create MouseCursorOriginal");
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  ReturnCode = IDirectDrawSurface_QueryInterface(_gpMouseCursorOriginal,
                                                 (const IID &)IID_IDirectDrawSurface2,
                                                 (LPVOID *)&gpMouseCursorOriginal);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  //
  // Initialize the main mouse background surfaces. There are two of them (one for each of the
  // Primary and Backbuffer surfaces
  //

  for (uiIndex = 0; uiIndex < 1; uiIndex++) {
    //
    // Initialize various mouse background variables
    //

    gMouseCursorBackground[uiIndex].fRestore = FALSE;

    //
    // Initialize the direct draw surfaces for the mouse background
    //

    ZEROMEM(SurfaceDescription);
    SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
    SurfaceDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    // SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    SurfaceDescription.dwWidth = MAX_CURSOR_WIDTH;
    SurfaceDescription.dwHeight = MAX_CURSOR_HEIGHT;
    ReturnCode = IDirectDraw2_CreateSurface(gpDirectDrawObject, &SurfaceDescription,
                                            &(gMouseCursorBackground[uiIndex]._pSurface), NULL);
    if (ReturnCode != DD_OK) {
      DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to create MouseCursorBackground");
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return FALSE;
    }

    ReturnCode = IDirectDrawSurface_QueryInterface(
        (IDirectDrawSurface *)gMouseCursorBackground[uiIndex]._pSurface,
        (const IID &)IID_IDirectDrawSurface2,
        (LPVOID *)&(gMouseCursorBackground[uiIndex].pSurface));
    if (ReturnCode != DD_OK) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return FALSE;
    }
  }

  //
  // Initialize the mutex sections
  //

  // ATE: Keep these mutexes for now!
  if (InitializeMutex(REFRESH_THREAD_MUTEX, "RefreshThreadMutex") == FALSE) {
    return FALSE;
  }
  if (InitializeMutex(FRAME_BUFFER_MUTEX, "FrameBufferMutex") == FALSE) {
    return FALSE;
  }
  if (InitializeMutex(MOUSE_BUFFER_MUTEX, "MouseBufferMutex") == FALSE) {
    return FALSE;
  }

  //
  // Initialize state variables
  //

  guiFrameBufferState = BUFFER_DIRTY;
  guiMouseBufferState = BUFFER_DISABLED;
  guiVideoManagerState = VIDEO_ON;
  guiRefreshThreadState = THREAD_OFF;
  guiDirtyRegionCount = 0;
  gfForceFullScreenRefresh = TRUE;
  gpFrameBufferRefreshOverride = NULL;
  gpCursorStore = NULL;
  gfPrintFrameBuffer = FALSE;
  guiPrintFrameBufferIndex = 0;

  //
  // This function must be called to setup RGB information
  //

  GetRGBDistribution();

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ShutdownVideoManager(void) {
  // UINT32  uiRefreshThreadState;

  DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Shutting down the video manager");

  //
  // Toggle the state of the video manager to indicate to the refresh thread that it needs to shut
  // itself down
  //

  IDirectDrawSurface2_Release(gpMouseCursorOriginal);
  IDirectDrawSurface2_Release(gpMouseCursor);
  IDirectDrawSurface2_Release(gMouseCursorBackground[0].pSurface);
  IDirectDrawSurface2_Release(gpBackBuffer);
  IDirectDrawSurface2_Release(gpPrimarySurface);

  IDirectDraw2_RestoreDisplayMode(gpDirectDrawObject);
  IDirectDraw2_SetCooperativeLevel(gpDirectDrawObject, ghWindow, DDSCL_NORMAL);
  IDirectDraw2_Release(gpDirectDrawObject);

  // destroy the window
  // DestroyWindow( ghWindow );

  guiVideoManagerState = VIDEO_OFF;

  if (gpCursorStore != NULL) {
    DeleteVideoObject(gpCursorStore);
    gpCursorStore = NULL;
  }

  // ATE: Release mouse cursor!
  FreeMouseCursor();

  UnRegisterDebugTopic(TOPIC_VIDEO, "Video");
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SuspendVideoManager(void) { guiVideoManagerState = VIDEO_SUSPENDED; }

void DoTester() {
  IDirectDraw2_RestoreDisplayMode(gpDirectDrawObject);
  IDirectDraw2_SetCooperativeLevel(gpDirectDrawObject, ghWindow, DDSCL_NORMAL);
  ShowCursor(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN RestoreVideoManager(void) {
  HRESULT ReturnCode;

  //
  // Make sure the video manager is indeed suspended before moving on
  //

  if (guiVideoManagerState == VIDEO_SUSPENDED) {
    //
    // Restore the Primary and Backbuffer
    //

    ReturnCode = IDirectDrawSurface2_Restore(gpPrimarySurface);
    if (ReturnCode != DD_OK) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return FALSE;
    }

    ReturnCode = IDirectDrawSurface2_Restore(gpBackBuffer);
    if (ReturnCode != DD_OK) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return FALSE;
    }

    //
    // Restore the mouse surfaces and make sure to initialize the gpMouseCursor surface
    //

    ReturnCode = IDirectDrawSurface2_Restore(gMouseCursorBackground[0].pSurface);
    if (ReturnCode != DD_OK) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return FALSE;
    }

    ReturnCode = IDirectDrawSurface2_Restore(gpMouseCursor);
    if (ReturnCode != DD_OK) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return FALSE;
    } else {
      guiMouseBufferState = BUFFER_DIRTY;
    }

    //
    // Set the video state to VIDEO_ON
    //

    guiFrameBufferState = BUFFER_DIRTY;
    guiMouseBufferState = BUFFER_DIRTY;
    gfForceFullScreenRefresh = TRUE;
    guiVideoManagerState = VIDEO_ON;
    return TRUE;
  } else {
    return FALSE;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void GetCurrentVideoSettings(UINT16 *usWidth, UINT16 *usHeight, UINT8 *ubBitDepth) {
  *usWidth = (UINT16)gusScreenWidth;
  *usHeight = (UINT16)gusScreenHeight;
  *ubBitDepth = (UINT8)gubScreenPixelDepth;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN CanBlitToFrameBuffer(void) {
  BOOLEAN fCanBlit;

  //
  // W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ----
  //
  // This function is intended to be called by a thread which has already locked the
  // FRAME_BUFFER_MUTEX mutual exclusion section. Anything else will cause the application to
  // yack
  //

  fCanBlit = (guiFrameBufferState == BUFFER_READY);

  return fCanBlit;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN CanBlitToMouseBuffer(void) {
  BOOLEAN fCanBlit;

  //
  // W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ----
  //
  // This function is intended to be called by a thread which has already locked the
  // MOUSE_BUFFER_MUTEX mutual exclusion section. Anything else will cause the application to
  // yack
  //

  fCanBlit = (guiMouseBufferState == BUFFER_READY);

  return fCanBlit;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void InvalidateRegion(INT32 iLeft, INT32 iTop, INT32 iRight, INT32 iBottom) {
  if (gfForceFullScreenRefresh == TRUE) {
    //
    // There's no point in going on since we are forcing a full screen refresh
    //

    return;
  }

  if (guiDirtyRegionCount < MAX_DIRTY_REGIONS) {
    //
    // Well we haven't broken the MAX_DIRTY_REGIONS limit yet, so we register the new region
    //

    // DO SOME PREMIMARY CHECKS FOR VALID RECTS
    if (iLeft < 0) iLeft = 0;

    if (iTop < 0) iTop = 0;

    if (iRight > SCREEN_WIDTH) iRight = SCREEN_WIDTH;

    if (iBottom > SCREEN_HEIGHT) iBottom = SCREEN_HEIGHT;

    if ((iRight - iLeft) <= 0) return;

    if ((iBottom - iTop) <= 0) return;

    gListOfDirtyRegions[guiDirtyRegionCount].iLeft = iLeft;
    gListOfDirtyRegions[guiDirtyRegionCount].iTop = iTop;
    gListOfDirtyRegions[guiDirtyRegionCount].iRight = iRight;
    gListOfDirtyRegions[guiDirtyRegionCount].iBottom = iBottom;

    //		gDirtyRegionFlags[ guiDirtyRegionCount ] = TRUE;

    guiDirtyRegionCount++;

  } else {
    //
    // The MAX_DIRTY_REGIONS limit has been exceeded. Therefore we arbitrarely invalidate the entire
    // screen and force a full screen refresh
    //
    guiDirtyRegionExCount = 0;
    guiDirtyRegionCount = 0;
    gfForceFullScreenRefresh = TRUE;
  }
}

void InvalidateRegionEx(INT32 iLeft, INT32 iTop, INT32 iRight, INT32 iBottom, UINT32 uiFlags) {
  INT32 iOldBottom;

  iOldBottom = iBottom;

  // Check if we are spanning the rectangle - if so slit it up!
  if (iTop <= gsVIEWPORT_WINDOW_END_Y && iBottom > gsVIEWPORT_WINDOW_END_Y) {
    // Add new top region
    iBottom = gsVIEWPORT_WINDOW_END_Y;
    AddRegionEx(iLeft, iTop, iRight, iBottom, uiFlags);

    // Add new bottom region
    iTop = gsVIEWPORT_WINDOW_END_Y;
    iBottom = iOldBottom;
    AddRegionEx(iLeft, iTop, iRight, iBottom, uiFlags);

  } else {
    AddRegionEx(iLeft, iTop, iRight, iBottom, uiFlags);
  }
}

void AddRegionEx(INT32 iLeft, INT32 iTop, INT32 iRight, INT32 iBottom, UINT32 uiFlags) {
  if (guiDirtyRegionExCount < MAX_DIRTY_REGIONS) {
    // DO SOME PREMIMARY CHECKS FOR VALID RECTS
    if (iLeft < 0) iLeft = 0;

    if (iTop < 0) iTop = 0;

    if (iRight > SCREEN_WIDTH) iRight = SCREEN_WIDTH;

    if (iBottom > SCREEN_HEIGHT) iBottom = SCREEN_HEIGHT;

    if ((iRight - iLeft) <= 0) return;

    if ((iBottom - iTop) <= 0) return;

    gDirtyRegionsEx[guiDirtyRegionExCount].iLeft = iLeft;
    gDirtyRegionsEx[guiDirtyRegionExCount].iTop = iTop;
    gDirtyRegionsEx[guiDirtyRegionExCount].iRight = iRight;
    gDirtyRegionsEx[guiDirtyRegionExCount].iBottom = iBottom;

    gDirtyRegionsFlagsEx[guiDirtyRegionExCount] = uiFlags;

    guiDirtyRegionExCount++;

  } else {
    guiDirtyRegionExCount = 0;
    guiDirtyRegionCount = 0;
    gfForceFullScreenRefresh = TRUE;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void InvalidateRegions(SGPRect *pArrayOfRegions, UINT32 uiRegionCount) {
  if (gfForceFullScreenRefresh == TRUE) {
    //
    // There's no point in going on since we are forcing a full screen refresh
    //

    return;
  }

  if ((guiDirtyRegionCount + uiRegionCount) < MAX_DIRTY_REGIONS) {
    UINT32 uiIndex;

    for (uiIndex = 0; uiIndex < uiRegionCount; uiIndex++) {
      //
      // Well we haven't broken the MAX_DIRTY_REGIONS limit yet, so we register the new region
      //

      gListOfDirtyRegions[guiDirtyRegionCount].iLeft = pArrayOfRegions[uiIndex].iLeft;
      gListOfDirtyRegions[guiDirtyRegionCount].iTop = pArrayOfRegions[uiIndex].iTop;
      gListOfDirtyRegions[guiDirtyRegionCount].iRight = pArrayOfRegions[uiIndex].iRight;
      gListOfDirtyRegions[guiDirtyRegionCount].iBottom = pArrayOfRegions[uiIndex].iBottom;

      guiDirtyRegionCount++;
    }
  } else {
    guiDirtyRegionCount = 0;
    gfForceFullScreenRefresh = TRUE;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void InvalidateScreen(void) {
  //
  // W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ----
  //
  // This function is intended to be called by a thread which has already locked the
  // FRAME_BUFFER_MUTEX mutual exclusion section. Anything else will cause the application to
  // yack
  //

  guiDirtyRegionCount = 0;
  guiDirtyRegionExCount = 0;
  gfForceFullScreenRefresh = TRUE;
  guiFrameBufferState = BUFFER_DIRTY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void InvalidateFrameBuffer(void) {
  //
  // W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ----
  //
  // This function is intended to be called by a thread which has already locked the
  // FRAME_BUFFER_MUTEX mutual exclusion section. Anything else will cause the application to
  // yack
  //

  guiFrameBufferState = BUFFER_DIRTY;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SetFrameBufferRefreshOverride(PTR pFrameBufferRefreshOverride) {
  gpFrameBufferRefreshOverride = (void(__cdecl *)(void))pFrameBufferRefreshOverride;
}

//#define SCROLL_TEST

///////////////////////////////////////////////////////////////////////////////////////////////////
void ScrollJA2Background(UINT32 uiDirection, INT16 sScrollXIncrement, INT16 sScrollYIncrement,
                         LPDIRECTDRAWSURFACE2 pSource, LPDIRECTDRAWSURFACE2 pDest,
                         BOOLEAN fRenderStrip, UINT32 uiCurrentMouseBackbuffer) {
  UINT16 usWidth, usHeight;
  UINT8 ubBitDepth;
  HRESULT ReturnCode;
  static RECT Region;
  static UINT16 usMouseXPos, usMouseYPos;
  static RECT StripRegions[2], MouseRegion;
  UINT16 usNumStrips = 0;
  INT32 cnt;
  INT16 sShiftX, sShiftY;
  INT32 uiCountY;

  GetCurrentVideoSettings(&usWidth, &usHeight, &ubBitDepth);
  usHeight = (gsVIEWPORT_WINDOW_END_Y - gsVIEWPORT_WINDOW_START_Y);

  StripRegions[0].left = gsVIEWPORT_START_X;
  StripRegions[0].right = gsVIEWPORT_END_X;
  StripRegions[0].top = gsVIEWPORT_WINDOW_START_Y;
  StripRegions[0].bottom = gsVIEWPORT_WINDOW_END_Y;
  StripRegions[1].left = gsVIEWPORT_START_X;
  StripRegions[1].right = gsVIEWPORT_END_X;
  StripRegions[1].top = gsVIEWPORT_WINDOW_START_Y;
  StripRegions[1].bottom = gsVIEWPORT_WINDOW_END_Y;

  MouseRegion.left = gMouseCursorBackground[uiCurrentMouseBackbuffer].usLeft;
  MouseRegion.top = gMouseCursorBackground[uiCurrentMouseBackbuffer].usTop;
  MouseRegion.right = gMouseCursorBackground[uiCurrentMouseBackbuffer].usRight;
  MouseRegion.bottom = gMouseCursorBackground[uiCurrentMouseBackbuffer].usBottom;

  usMouseXPos = gMouseCursorBackground[uiCurrentMouseBackbuffer].usMouseXPos;
  usMouseYPos = gMouseCursorBackground[uiCurrentMouseBackbuffer].usMouseYPos;

  switch (uiDirection) {
    case SCROLL_LEFT:

      Region.left = 0;
      Region.top = gsVIEWPORT_WINDOW_START_Y;
      Region.right = usWidth - (sScrollXIncrement);
      Region.bottom = gsVIEWPORT_WINDOW_START_Y + usHeight;

      do {
        ReturnCode =
            IDirectDrawSurface2_SGPBltFast(pDest, sScrollXIncrement, gsVIEWPORT_WINDOW_START_Y,
                                           pSource, (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);

          if (ReturnCode == DDERR_SURFACELOST) {
            break;
          }
        }
      } while (ReturnCode != DD_OK);

      // memset z-buffer
      for (uiCountY = gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, sScrollXIncrement * 2);
      }

      StripRegions[0].right = (INT16)(gsVIEWPORT_START_X + sScrollXIncrement);
      usMouseXPos += sScrollXIncrement;

      usNumStrips = 1;
      break;

    case SCROLL_RIGHT:

      Region.left = sScrollXIncrement;
      Region.top = gsVIEWPORT_WINDOW_START_Y;
      Region.right = usWidth;
      Region.bottom = gsVIEWPORT_WINDOW_START_Y + usHeight;

      do {
        ReturnCode = IDirectDrawSurface2_SGPBltFast(pDest, 0, gsVIEWPORT_WINDOW_START_Y, pSource,
                                                    (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);

          if (ReturnCode == DDERR_SURFACELOST) {
            break;
          }
        }
      } while (ReturnCode != DD_OK);

      // memset z-buffer
      for (uiCountY = gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset(
            (UINT8 *)gpZBuffer + (uiCountY * 1280) + ((gsVIEWPORT_END_X - sScrollXIncrement) * 2),
            0, sScrollXIncrement * 2);
      }

      // for(uiCountY=0; uiCountY < usHeight; uiCountY++)
      //{
      //	memcpy(pDestBuf+(uiCountY*uiDestPitchBYTES),
      //					pSrcBuf+(uiCountY*uiDestPitchBYTES)+sScrollXIncrement*uiBPP,
      //					uiDestPitchBYTES-sScrollXIncrement*uiBPP);
      //}

      StripRegions[0].left = (INT16)(gsVIEWPORT_END_X - sScrollXIncrement);
      usMouseXPos -= sScrollXIncrement;

      usNumStrips = 1;
      break;

    case SCROLL_UP:

      Region.left = 0;
      Region.top = gsVIEWPORT_WINDOW_START_Y;
      Region.right = usWidth;
      Region.bottom = gsVIEWPORT_WINDOW_START_Y + usHeight - sScrollYIncrement;

      do {
        ReturnCode =
            IDirectDrawSurface2_SGPBltFast(pDest, 0, gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement,
                                           pSource, (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);

          if (ReturnCode == DDERR_SURFACELOST) {
            break;
          }
        }
      } while (ReturnCode != DD_OK);

      for (uiCountY = sScrollYIncrement - 1 + gsVIEWPORT_WINDOW_START_Y;
           uiCountY >= gsVIEWPORT_WINDOW_START_Y; uiCountY--) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, 1280);
      }

      // for(uiCountY=usHeight-1; uiCountY >= sScrollYIncrement; uiCountY--)
      //{
      //	memcpy(pDestBuf+(uiCountY*uiDestPitchBYTES),
      //					pSrcBuf+((uiCountY-sScrollYIncrement)*uiDestPitchBYTES),
      //					uiDestPitchBYTES);
      //}
      StripRegions[0].bottom = (INT16)(gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement);
      usNumStrips = 1;

      usMouseYPos += sScrollYIncrement;

      break;

    case SCROLL_DOWN:

      Region.left = 0;
      Region.top = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;
      Region.right = usWidth;
      Region.bottom = gsVIEWPORT_WINDOW_START_Y + usHeight;

      do {
        ReturnCode = IDirectDrawSurface2_SGPBltFast(pDest, 0, gsVIEWPORT_WINDOW_START_Y, pSource,
                                                    (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);

          if (ReturnCode == DDERR_SURFACELOST) {
            break;
          }
        }
      } while (ReturnCode != DD_OK);

      // Zero out z
      for (uiCountY = (gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement);
           uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, 1280);
      }

      // for(uiCountY=0; uiCountY < (usHeight-sScrollYIncrement); uiCountY++)
      //{
      //	memcpy(pDestBuf+(uiCountY*uiDestPitchBYTES),
      //					pSrcBuf+((uiCountY+sScrollYIncrement)*uiDestPitchBYTES),
      //					uiDestPitchBYTES);
      //}

      StripRegions[0].top = (INT16)(gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement);
      usNumStrips = 1;

      usMouseYPos -= sScrollYIncrement;

      break;

    case SCROLL_UPLEFT:

      Region.left = 0;
      Region.top = gsVIEWPORT_WINDOW_START_Y;
      Region.right = usWidth - (sScrollXIncrement);
      Region.bottom = gsVIEWPORT_WINDOW_START_Y + usHeight - sScrollYIncrement;

      do {
        ReturnCode = IDirectDrawSurface2_SGPBltFast(pDest, sScrollXIncrement,
                                                    gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement,
                                                    pSource, (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);

          if (ReturnCode == DDERR_SURFACELOST) {
            break;
          }
        }
      } while (ReturnCode != DD_OK);

      // memset z-buffer
      for (uiCountY = gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, sScrollXIncrement * 2);
      }
      for (uiCountY = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement - 1;
           uiCountY >= gsVIEWPORT_WINDOW_START_Y; uiCountY--) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, 1280);
      }

      StripRegions[0].right = (INT16)(gsVIEWPORT_START_X + sScrollXIncrement);
      StripRegions[1].bottom = (INT16)(gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement);
      StripRegions[1].left = (INT16)(gsVIEWPORT_START_X + sScrollXIncrement);
      usNumStrips = 2;

      usMouseYPos += sScrollYIncrement;
      usMouseXPos += sScrollXIncrement;

      break;

    case SCROLL_UPRIGHT:

      Region.left = sScrollXIncrement;
      Region.top = gsVIEWPORT_WINDOW_START_Y;
      Region.right = usWidth;
      Region.bottom = gsVIEWPORT_WINDOW_START_Y + usHeight - sScrollYIncrement;

      do {
        ReturnCode =
            IDirectDrawSurface2_SGPBltFast(pDest, 0, gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement,
                                           pSource, (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);

          if (ReturnCode == DDERR_SURFACELOST) {
            break;
          }
        }
      } while (ReturnCode != DD_OK);

      // memset z-buffer
      for (uiCountY = gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset(
            (UINT8 *)gpZBuffer + (uiCountY * 1280) + ((gsVIEWPORT_END_X - sScrollXIncrement) * 2),
            0, sScrollXIncrement * 2);
      }
      for (uiCountY = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement - 1;
           uiCountY >= gsVIEWPORT_WINDOW_START_Y; uiCountY--) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, 1280);
      }

      StripRegions[0].left = (INT16)(gsVIEWPORT_END_X - sScrollXIncrement);
      StripRegions[1].bottom = (INT16)(gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement);
      StripRegions[1].right = (INT16)(gsVIEWPORT_END_X - sScrollXIncrement);
      usNumStrips = 2;

      usMouseYPos += sScrollYIncrement;
      usMouseXPos -= sScrollXIncrement;

      break;

    case SCROLL_DOWNLEFT:

      Region.left = 0;
      Region.top = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;
      Region.right = usWidth - (sScrollXIncrement);
      Region.bottom = gsVIEWPORT_WINDOW_START_Y + usHeight;

      do {
        ReturnCode =
            IDirectDrawSurface2_SGPBltFast(pDest, sScrollXIncrement, gsVIEWPORT_WINDOW_START_Y,
                                           pSource, (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);

          if (ReturnCode == DDERR_SURFACELOST) {
            break;
          }
        }
      } while (ReturnCode != DD_OK);

      // memset z-buffer
      for (uiCountY = gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, sScrollXIncrement * 2);
      }
      for (uiCountY = (gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement);
           uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, 1280);
      }

      StripRegions[0].right = (INT16)(gsVIEWPORT_START_X + sScrollXIncrement);

      StripRegions[1].top = (INT16)(gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement);
      StripRegions[1].left = (INT16)(gsVIEWPORT_START_X + sScrollXIncrement);
      usNumStrips = 2;

      usMouseYPos -= sScrollYIncrement;
      usMouseXPos += sScrollXIncrement;

      break;

    case SCROLL_DOWNRIGHT:

      Region.left = sScrollXIncrement;
      Region.top = gsVIEWPORT_WINDOW_START_Y + sScrollYIncrement;
      Region.right = usWidth;
      Region.bottom = gsVIEWPORT_WINDOW_START_Y + usHeight;

      do {
        ReturnCode = IDirectDrawSurface2_SGPBltFast(pDest, 0, gsVIEWPORT_WINDOW_START_Y, pSource,
                                                    (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);

          if (ReturnCode == DDERR_SURFACELOST) {
            break;
          }
        }
      } while (ReturnCode != DD_OK);

      // memset z-buffer
      for (uiCountY = gsVIEWPORT_WINDOW_START_Y; uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset(
            (UINT8 *)gpZBuffer + (uiCountY * 1280) + ((gsVIEWPORT_END_X - sScrollXIncrement) * 2),
            0, sScrollXIncrement * 2);
      }
      for (uiCountY = (gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement);
           uiCountY < gsVIEWPORT_WINDOW_END_Y; uiCountY++) {
        memset((UINT8 *)gpZBuffer + (uiCountY * 1280), 0, 1280);
      }

      StripRegions[0].left = (INT16)(gsVIEWPORT_END_X - sScrollXIncrement);
      StripRegions[1].top = (INT16)(gsVIEWPORT_WINDOW_END_Y - sScrollYIncrement);
      StripRegions[1].right = (INT16)(gsVIEWPORT_END_X - sScrollXIncrement);
      usNumStrips = 2;

      usMouseYPos -= sScrollYIncrement;
      usMouseXPos -= sScrollXIncrement;

      break;
  }

  if (fRenderStrip) {
    // Memset to 0
#ifdef SCROLL_TEST
    {
      DDBLTFX BlitterFX;

      BlitterFX.dwSize = sizeof(DDBLTFX);
      BlitterFX.dwFillColor = 0;

      DDBltSurface((LPDIRECTDRAWSURFACE2)pDest, NULL, NULL, NULL, DDBLT_COLORFILL, &BlitterFX);
    }
#endif

    for (cnt = 0; cnt < usNumStrips; cnt++) {
      RenderStaticWorldRect((INT16)StripRegions[cnt].left, (INT16)StripRegions[cnt].top,
                            (INT16)StripRegions[cnt].right, (INT16)StripRegions[cnt].bottom, TRUE);
      // Optimize Redundent tiles too!
      // ExamineZBufferRect( (INT16)StripRegions[ cnt ].left, (INT16)StripRegions[ cnt ].top,
      // (INT16)StripRegions[ cnt ].right, (INT16)StripRegions[ cnt ].bottom );

      do {
        ReturnCode = IDirectDrawSurface2_SGPBltFast(
            pDest, StripRegions[cnt].left, StripRegions[cnt].top, gpFrameBuffer,
            (LPRECT) & (StripRegions[cnt]), DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);
        }

        if (ReturnCode == DDERR_SURFACELOST) {
          break;
        }
      } while (ReturnCode != DD_OK);
    }

    sShiftX = 0;
    sShiftY = 0;

    switch (uiDirection) {
      case SCROLL_LEFT:

        sShiftX = sScrollXIncrement;
        sShiftY = 0;
        break;

      case SCROLL_RIGHT:

        sShiftX = -sScrollXIncrement;
        sShiftY = 0;
        break;

      case SCROLL_UP:

        sShiftX = 0;
        sShiftY = sScrollYIncrement;
        break;

      case SCROLL_DOWN:

        sShiftX = 0;
        sShiftY = -sScrollYIncrement;
        break;

      case SCROLL_UPLEFT:

        sShiftX = sScrollXIncrement;
        sShiftY = sScrollYIncrement;
        break;

      case SCROLL_UPRIGHT:

        sShiftX = -sScrollXIncrement;
        sShiftY = sScrollYIncrement;
        break;

      case SCROLL_DOWNLEFT:

        sShiftX = sScrollXIncrement;
        sShiftY = -sScrollYIncrement;
        break;

      case SCROLL_DOWNRIGHT:

        sShiftX = -sScrollXIncrement;
        sShiftY = -sScrollYIncrement;
        break;
    }

    // RESTORE SHIFTED
    RestoreShiftedVideoOverlays(sShiftX, sShiftY);

    // SAVE NEW
    SaveVideoOverlaysArea(BACKBUFFER);

    // BLIT NEW
    ExecuteVideoOverlaysToAlternateBuffer(BACKBUFFER);

#if 0

		// Erase mouse from old position
		if (gMouseCursorBackground[ uiCurrentMouseBackbuffer ].fRestore == TRUE )
		{

			do
			{
				ReturnCode = IDirectDrawSurface2_SGPBltFast(gpBackBuffer, usMouseXPos, usMouseYPos, gMouseCursorBackground[uiCurrentMouseBackbuffer].pSurface, (LPRECT)&MouseRegion, DDBLTFAST_NOCOLORKEY);
				if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
				{
					DirectXAttempt ( ReturnCode, __LINE__, __FILE__ );

					if (ReturnCode == DDERR_SURFACELOST)
					{

					}
				}
			} while (ReturnCode != DD_OK);
		}

#endif
  }

  // InvalidateRegion( sLeftDraw, sTopDraw, sRightDraw, sBottomDraw );

  // UpdateSaveBuffer();
  // SaveBackgroundRects();
}

void RefreshScreen(void *DummyVariable) {
  static UINT32 uiRefreshThreadState, uiIndex;
  UINT16 usScreenWidth, usScreenHeight;
  static BOOLEAN fShowMouse;
  HRESULT ReturnCode;
  static RECT Region;
  static POINT MousePos;
  static BOOLEAN fFirstTime = TRUE;
  UINT32 uiTime;

  usScreenWidth = usScreenHeight = 0;

  if (fFirstTime) {
    fShowMouse = FALSE;
  }

  // DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Looping in refresh");

  ///////////////////////////////////////////////////////////////////////////////////////////////
  //
  // REFRESH_THREAD_MUTEX
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////

  switch (guiVideoManagerState) {
    case VIDEO_ON:  //
      // Excellent, everything is cosher, we continue on
      //
      uiRefreshThreadState = guiRefreshThreadState = THREAD_ON;
      usScreenWidth = gusScreenWidth;
      usScreenHeight = gusScreenHeight;
      break;
    case VIDEO_OFF:  //
      // Hot damn, the video manager is suddenly off. We have to bugger out of here. Don't forget to
      // leave the critical section
      //
      guiRefreshThreadState = THREAD_OFF;
      return;
    case VIDEO_SUSPENDED:  //
      // This are suspended. Make sure the refresh function does try to access any of the direct
      // draw surfaces
      //
      uiRefreshThreadState = guiRefreshThreadState = THREAD_SUSPENDED;
      break;
    case VIDEO_SHUTTING_DOWN:  //
                               // Well things are shutting down. So we need to bugger out of there.
                               // Don't forget to leave the critical section before returning
                               //
      guiRefreshThreadState = THREAD_OFF;
      return;
  }

  //
  // Get the current mouse position
  //

  GetCursorPos(&MousePos);

  /////////////////////////////////////////////////////////////////////////////////////////////
  //
  // FRAME_BUFFER_MUTEX
  //
  /////////////////////////////////////////////////////////////////////////////////////////////

  // RESTORE OLD POSITION OF MOUSE
  if (gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore == TRUE) {
    Region.left = gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft;
    Region.top = gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop;
    Region.right = gMouseCursorBackground[CURRENT_MOUSE_DATA].usRight;
    Region.bottom = gMouseCursorBackground[CURRENT_MOUSE_DATA].usBottom;

    do {
      ReturnCode = IDirectDrawSurface2_SGPBltFast(
          gpBackBuffer, gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos,
          gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos,
          gMouseCursorBackground[CURRENT_MOUSE_DATA].pSurface, (LPRECT)&Region,
          DDBLTFAST_NOCOLORKEY);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);

        if (ReturnCode == DDERR_SURFACELOST) {
          goto ENDOFLOOP;
        }
      }
    } while (ReturnCode != DD_OK);

    // Save position into other background region
    memcpy(&(gMouseCursorBackground[PREVIOUS_MOUSE_DATA]),
           &(gMouseCursorBackground[CURRENT_MOUSE_DATA]), sizeof(MouseCursorBackground));
  }

  //
  // Ok we were able to get a hold of the frame buffer stuff. Check to see if it needs updating
  // if not, release the frame buffer stuff right away
  //
  if (guiFrameBufferState == BUFFER_DIRTY) {
    // Well the frame buffer is dirty.
    //

    if (gpFrameBufferRefreshOverride != NULL) {
      //
      // Method (3) - We are using a function override to refresh the frame buffer. First we
      // call the override function then we must set the override pointer to NULL
      //

      (*gpFrameBufferRefreshOverride)();
      gpFrameBufferRefreshOverride = NULL;
    }

    if (gfFadeInitialized && gfFadeInVideo) {
      gFadeFunction();
    } else
    //
    // Either Method (1) or (2)
    //
    {
      if (gfForceFullScreenRefresh == TRUE) {
        //
        // Method (1) - We will be refreshing the entire screen
        //

        Region.left = 0;
        Region.top = 0;
        Region.right = usScreenWidth;
        Region.bottom = usScreenHeight;

        do {
          ReturnCode = IDirectDrawSurface2_SGPBltFast(gpBackBuffer, 0, 0, gpFrameBuffer,
                                                      (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
          if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
            DirectXAttempt(ReturnCode, __LINE__, __FILE__);

            if (ReturnCode == DDERR_SURFACELOST) {
              goto ENDOFLOOP;
            }
          }
        } while (ReturnCode != DD_OK);

      } else {
        for (uiIndex = 0; uiIndex < guiDirtyRegionCount; uiIndex++) {
          Region.left = gListOfDirtyRegions[uiIndex].iLeft;
          Region.top = gListOfDirtyRegions[uiIndex].iTop;
          Region.right = gListOfDirtyRegions[uiIndex].iRight;
          Region.bottom = gListOfDirtyRegions[uiIndex].iBottom;

          do {
            ReturnCode =
                IDirectDrawSurface2_SGPBltFast(gpBackBuffer, Region.left, Region.top, gpFrameBuffer,
                                               (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
            if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
              DirectXAttempt(ReturnCode, __LINE__, __FILE__);
            }

            if (ReturnCode == DDERR_SURFACELOST) {
              goto ENDOFLOOP;
            }
          } while (ReturnCode != DD_OK);
        }

        // Now do new, extended dirty regions
        for (uiIndex = 0; uiIndex < guiDirtyRegionExCount; uiIndex++) {
          Region.left = gDirtyRegionsEx[uiIndex].iLeft;
          Region.top = gDirtyRegionsEx[uiIndex].iTop;
          Region.right = gDirtyRegionsEx[uiIndex].iRight;
          Region.bottom = gDirtyRegionsEx[uiIndex].iBottom;

          // Do some checks if we are in the process of scrolling!
          if (gfRenderScroll) {
            // Check if we are completely out of bounds
            if (Region.top <= gsVIEWPORT_WINDOW_END_Y && Region.bottom <= gsVIEWPORT_WINDOW_END_Y) {
              continue;
            }
          }

          do {
            ReturnCode =
                IDirectDrawSurface2_SGPBltFast(gpBackBuffer, Region.left, Region.top, gpFrameBuffer,
                                               (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
            if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
              DirectXAttempt(ReturnCode, __LINE__, __FILE__);
            }

            if (ReturnCode == DDERR_SURFACELOST) {
              goto ENDOFLOOP;
            }
          } while (ReturnCode != DD_OK);
        }
      }
    }
    if (gfRenderScroll) {
      ScrollJA2Background(guiScrollDirection, gsScrollXIncrement, gsScrollYIncrement,
                          gpPrimarySurface, gpBackBuffer, TRUE, PREVIOUS_MOUSE_DATA);
    }
    gfIgnoreScrollDueToCenterAdjust = FALSE;

    //
    // Update the guiFrameBufferState variable to reflect that the frame buffer can now be handled
    //

    guiFrameBufferState = BUFFER_READY;
  }

  //
  // Do we want to print the frame stuff ??
  //

  if (gfVideoCapture) {
    uiTime = GetTickCount();
    if ((uiTime < guiLastFrame) || (uiTime > (guiLastFrame + guiFramePeriod))) {
      SnapshotSmall();
      guiLastFrame = uiTime;
    }
  }

  if (gfPrintFrameBuffer == TRUE) {
    LPDIRECTDRAWSURFACE _pTmpBuffer;
    LPDIRECTDRAWSURFACE2 pTmpBuffer;
    DDSURFACEDESC SurfaceDescription;
    FILE *OutputFile;
    CHAR8 FileName[64];
    INT32 iIndex;
    STRING512 ExecDir;
    UINT16 *p16BPPData;

    GetExecutableDirectory(ExecDir);
    SetFileManCurrentDirectory(ExecDir);

    //
    // Create temporary system memory surface. This is used to correct problems with the backbuffer
    // surface which can be interlaced or have a funky pitch
    //

    ZEROMEM(SurfaceDescription);
    SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
    SurfaceDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    SurfaceDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    SurfaceDescription.dwWidth = usScreenWidth;
    SurfaceDescription.dwHeight = usScreenHeight;
    ReturnCode =
        IDirectDraw2_CreateSurface(gpDirectDrawObject, &SurfaceDescription, &_pTmpBuffer, NULL);
    if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    }

    ReturnCode = IDirectDrawSurface_QueryInterface((IDirectDrawSurface *)_pTmpBuffer,
                                                   (const IID &)IID_IDirectDrawSurface2,
                                                   (LPVOID *)&pTmpBuffer);
    if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    }

    //
    // Copy the primary surface to the temporary surface
    //

    Region.left = 0;
    Region.top = 0;
    Region.right = usScreenWidth;
    Region.bottom = usScreenHeight;

    do {
      ReturnCode = IDirectDrawSurface2_SGPBltFast(pTmpBuffer, 0, 0, gpPrimarySurface, &Region,
                                                  DDBLTFAST_NOCOLORKEY);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      }
    } while (ReturnCode != DD_OK);

    //
    // Ok now that temp surface has contents of backbuffer, copy temp surface to disk
    //

    sprintf(FileName, "SCREEN%03d.TGA", guiPrintFrameBufferIndex++);
    if ((OutputFile = fopen(FileName, "wb")) != NULL) {
      fprintf(OutputFile, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0x80, 0x02, 0xe0, 0x01, 0x10, 0);

      //
      // Lock temp surface
      //

      ZEROMEM(SurfaceDescription);
      SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
      ReturnCode = IDirectDrawSurface2_Lock(pTmpBuffer, NULL, &SurfaceDescription, 0, NULL);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      }

      //
      // Copy 16 bit buffer to file
      //

      // 5/6/5.. create buffer...
      if (gusRedMask == 0xF800 && gusGreenMask == 0x07E0 && gusBlueMask == 0x001F) {
        p16BPPData = (UINT16 *)MemAlloc(640 * 2);
      }

      for (iIndex = 479; iIndex >= 0; iIndex--) {
        // ATE: OK, fix this such that it converts pixel format to 5/5/5
        // if current settings are 5/6/5....
        if (gusRedMask == 0xF800 && gusGreenMask == 0x07E0 && gusBlueMask == 0x001F) {
          // Read into a buffer...
          memcpy(p16BPPData, (((UINT8 *)SurfaceDescription.lpSurface) + (iIndex * 640 * 2)),
                 640 * 2);

          // Convert....
          ConvertRGBDistribution565To555(p16BPPData, 640);

          // Write
          fwrite(p16BPPData, 640 * 2, 1, OutputFile);
        } else {
          fwrite((void *)(((UINT8 *)SurfaceDescription.lpSurface) + (iIndex * 640 * 2)), 640 * 2, 1,
                 OutputFile);
        }
      }

      // 5/6/5.. Delete buffer...
      if (gusRedMask == 0xF800 && gusGreenMask == 0x07E0 && gusBlueMask == 0x001F) {
        MemFree(p16BPPData);
      }

      fclose(OutputFile);

      //
      // Unlock temp surface
      //

      ZEROMEM(SurfaceDescription);
      SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
      ReturnCode = IDirectDrawSurface2_Unlock(pTmpBuffer, &SurfaceDescription);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      }
    }

    //
    // Release temp surface
    //

    gfPrintFrameBuffer = FALSE;
    IDirectDrawSurface2_Release(pTmpBuffer);

    strcat(ExecDir, "\\Data");
    SetFileManCurrentDirectory(ExecDir);
  }

  //
  // Ok we were able to get a hold of the frame buffer stuff. Check to see if it needs updating
  // if not, release the frame buffer stuff right away
  //

  if (guiMouseBufferState == BUFFER_DIRTY) {
    //
    // Well the mouse buffer is dirty. Upload the whole thing
    //

    Region.left = 0;
    Region.top = 0;
    Region.right = gusMouseCursorWidth;
    Region.bottom = gusMouseCursorHeight;

    do {
      ReturnCode = IDirectDrawSurface2_SGPBltFast(gpMouseCursor, 0, 0, gpMouseCursorOriginal,
                                                  (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      }
    } while (ReturnCode != DD_OK);

    guiMouseBufferState = BUFFER_READY;
  }

  //
  // Check current state of the mouse cursor
  //

  if (fShowMouse == FALSE) {
    if (guiMouseBufferState == BUFFER_READY) {
      fShowMouse = TRUE;
    } else {
      fShowMouse = FALSE;
    }
  } else {
    if (guiMouseBufferState == BUFFER_DISABLED) {
      fShowMouse = FALSE;
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  //
  // End of MOUSE_BUFFER_MUTEX
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////////////////////
  //
  // If fMouseState == TRUE
  //
  // (1) Save mouse background from gpBackBuffer to gpMouseCursorBackground
  // (2) If step (1) is successfull blit mouse cursor onto gpBackBuffer
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////

  if (fShowMouse == TRUE) {
    //
    // Step (1) - Save mouse background
    //

    Region.left = MousePos.x - gsMouseCursorXOffset;
    Region.top = MousePos.y - gsMouseCursorYOffset;
    Region.right = Region.left + gusMouseCursorWidth;
    Region.bottom = Region.top + gusMouseCursorHeight;

    if (Region.right > usScreenWidth) {
      Region.right = usScreenWidth;
    }

    if (Region.bottom > usScreenHeight) {
      Region.bottom = usScreenHeight;
    }

    if ((Region.right > Region.left) && (Region.bottom > Region.top)) {
      //
      // Make sure the mouse background is marked for restore and coordinates are saved for the
      // future restore
      //

      gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore = TRUE;
      gMouseCursorBackground[CURRENT_MOUSE_DATA].usRight =
          (UINT16)Region.right - (UINT16)Region.left;
      gMouseCursorBackground[CURRENT_MOUSE_DATA].usBottom =
          (UINT16)Region.bottom - (UINT16)Region.top;
      if (Region.left < 0) {
        gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft = (UINT16)(0 - Region.left);
        gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos = 0;
        Region.left = 0;
      } else {
        gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos =
            (UINT16)MousePos.x - gsMouseCursorXOffset;
        gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft = 0;
      }
      if (Region.top < 0) {
        gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos = 0;
        gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop = (UINT16)(0 - Region.top);
        Region.top = 0;
      } else {
        gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos =
            (UINT16)MousePos.y - gsMouseCursorYOffset;
        gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop = 0;
      }

      if ((Region.right > Region.left) && (Region.bottom > Region.top)) {
        // Save clipped region
        gMouseCursorBackground[CURRENT_MOUSE_DATA].Region = Region;

        //
        // Ok, do the actual data save to the mouse background
        //

        do {
          ReturnCode =
              IDirectDrawSurface2_SGPBltFast(gMouseCursorBackground[CURRENT_MOUSE_DATA].pSurface,
                                             gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft,
                                             gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop,
                                             gpBackBuffer, &Region, DDBLTFAST_NOCOLORKEY);
          if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
            DirectXAttempt(ReturnCode, __LINE__, __FILE__);
          }

          if (ReturnCode == DDERR_SURFACELOST) {
            goto ENDOFLOOP;
          }
        } while (ReturnCode != DD_OK);

        //
        // Step (2) - Blit mouse cursor to back buffer
        //

        Region.left = gMouseCursorBackground[CURRENT_MOUSE_DATA].usLeft;
        Region.top = gMouseCursorBackground[CURRENT_MOUSE_DATA].usTop;
        Region.right = gMouseCursorBackground[CURRENT_MOUSE_DATA].usRight;
        Region.bottom = gMouseCursorBackground[CURRENT_MOUSE_DATA].usBottom;

        do {
          ReturnCode = IDirectDrawSurface2_SGPBltFast(
              gpBackBuffer, gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos,
              gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos, gpMouseCursor, &Region,
              DDBLTFAST_SRCCOLORKEY);
          if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
            DirectXAttempt(ReturnCode, __LINE__, __FILE__);
          }

          if (ReturnCode == DDERR_SURFACELOST) {
            goto ENDOFLOOP;
          }
        } while (ReturnCode != DD_OK);
      } else {
        //
        // Hum, the mouse was not blitted this round. Henceforth we will flag fRestore as FALSE
        //

        gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore = FALSE;
      }

    } else {
      //
      // Hum, the mouse was not blitted this round. Henceforth we will flag fRestore as FALSE
      //

      gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore = FALSE;
    }
  } else {
    //
    // Well since there was no mouse handling this round, we disable the mouse restore
    //

    gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore = FALSE;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  //
  // (1) Flip Pages
  // (2) If the page flipping worked, then we copy the contents of the primary surface back
  //     to the backbuffer
  // (3) If step (2) was successfull we then restore the mouse background onto the backbuffer
  //     if fShowMouse is TRUE
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////

  //
  // Step (1) - Flip pages
  //
#ifdef WINDOWED_MODE

  do {
    ReturnCode = IDirectDrawSurface_Blt(gpPrimarySurface,  // dest surface
                                        &rcWindow,         // dest rect
                                        gpBackBuffer,      // src surface
                                        NULL,              // src rect (all of it)
                                        DDBLT_WAIT, NULL);

    if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);

      if (ReturnCode == DDERR_SURFACELOST) {
        goto ENDOFLOOP;
      }
    }

  } while (ReturnCode != DD_OK);

#else

  do {
    ReturnCode = IDirectDrawSurface_Flip(_gpPrimarySurface, NULL, DDFLIP_WAIT);
    //    if ((ReturnCode != DD_OK)&&(ReturnCode != DDERR_WASSTILLDRAWING))
    if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);

      if (ReturnCode == DDERR_SURFACELOST) {
        goto ENDOFLOOP;
      }
    }

  } while (ReturnCode != DD_OK);

#endif

  //
  // Step (2) - Copy Primary Surface to the Back Buffer
  //
  if (gfRenderScroll) {
    Region.left = 0;
    Region.top = 0;
    Region.right = 640;
    Region.bottom = 360;

    do {
      ReturnCode = IDirectDrawSurface2_SGPBltFast(gpBackBuffer, 0, 0, gpPrimarySurface, &Region,
                                                  DDBLTFAST_NOCOLORKEY);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);

        if (ReturnCode == DDERR_SURFACELOST) {
          goto ENDOFLOOP;
        }
      }
    } while (ReturnCode != DD_OK);

    // Get new background for mouse
    //
    // Ok, do the actual data save to the mouse background

    //

    gfRenderScroll = FALSE;
    gfScrollStart = FALSE;
  }

  // COPY MOUSE AREAS FROM PRIMARY BACK!

  // FIRST OLD ERASED POSITION
  if (gMouseCursorBackground[PREVIOUS_MOUSE_DATA].fRestore == TRUE) {
    Region = gMouseCursorBackground[PREVIOUS_MOUSE_DATA].Region;

    do {
      ReturnCode = IDirectDrawSurface2_SGPBltFast(
          gpBackBuffer, gMouseCursorBackground[PREVIOUS_MOUSE_DATA].usMouseXPos,
          gMouseCursorBackground[PREVIOUS_MOUSE_DATA].usMouseYPos, gpPrimarySurface,
          (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
      if (ReturnCode != DD_OK && ReturnCode != DDERR_WASSTILLDRAWING) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);

        if (ReturnCode == DDERR_SURFACELOST) {
          goto ENDOFLOOP;
        }
      }
    } while (ReturnCode != DD_OK);
  }

  // NOW NEW MOUSE AREA
  if (gMouseCursorBackground[CURRENT_MOUSE_DATA].fRestore == TRUE) {
    Region = gMouseCursorBackground[CURRENT_MOUSE_DATA].Region;

    do {
      ReturnCode = IDirectDrawSurface2_SGPBltFast(
          gpBackBuffer, gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseXPos,
          gMouseCursorBackground[CURRENT_MOUSE_DATA].usMouseYPos, gpPrimarySurface, (LPRECT)&Region,
          DDBLTFAST_NOCOLORKEY);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);

        if (ReturnCode == DDERR_SURFACELOST) {
          goto ENDOFLOOP;
        }
      }
    } while (ReturnCode != DD_OK);
  }

  if (gfForceFullScreenRefresh == TRUE) {
    //
    // Method (1) - We will be refreshing the entire screen
    //
    Region.left = 0;
    Region.top = 0;
    Region.right = SCREEN_WIDTH;
    Region.bottom = SCREEN_HEIGHT;

    do {
      ReturnCode = IDirectDrawSurface2_SGPBltFast(gpBackBuffer, 0, 0, gpPrimarySurface, &Region,
                                                  DDBLTFAST_NOCOLORKEY);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);

        if (ReturnCode == DDERR_SURFACELOST) {
          goto ENDOFLOOP;
        }
      }
    } while (ReturnCode != DD_OK);

    guiDirtyRegionCount = 0;
    guiDirtyRegionExCount = 0;
    gfForceFullScreenRefresh = FALSE;
  } else {
    for (uiIndex = 0; uiIndex < guiDirtyRegionCount; uiIndex++) {
      Region.left = gListOfDirtyRegions[uiIndex].iLeft;
      Region.top = gListOfDirtyRegions[uiIndex].iTop;
      Region.right = gListOfDirtyRegions[uiIndex].iRight;
      Region.bottom = gListOfDirtyRegions[uiIndex].iBottom;

      do {
        ReturnCode =
            IDirectDrawSurface2_SGPBltFast(gpBackBuffer, Region.left, Region.top, gpPrimarySurface,
                                           (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
        if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
          DirectXAttempt(ReturnCode, __LINE__, __FILE__);
        }

        if (ReturnCode == DDERR_SURFACELOST) {
          goto ENDOFLOOP;
        }
      } while (ReturnCode != DD_OK);
    }

    guiDirtyRegionCount = 0;
    gfForceFullScreenRefresh = FALSE;
  }

  // Do extended dirty regions!
  for (uiIndex = 0; uiIndex < guiDirtyRegionExCount; uiIndex++) {
    Region.left = gDirtyRegionsEx[uiIndex].iLeft;
    Region.top = gDirtyRegionsEx[uiIndex].iTop;
    Region.right = gDirtyRegionsEx[uiIndex].iRight;
    Region.bottom = gDirtyRegionsEx[uiIndex].iBottom;

    if ((Region.top < gsVIEWPORT_WINDOW_END_Y) && gfRenderScroll) {
      continue;
    }

    do {
      ReturnCode =
          IDirectDrawSurface2_SGPBltFast(gpBackBuffer, Region.left, Region.top, gpPrimarySurface,
                                         (LPRECT)&Region, DDBLTFAST_NOCOLORKEY);
      if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
        DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      }

      if (ReturnCode == DDERR_SURFACELOST) {
        goto ENDOFLOOP;
      }
    } while (ReturnCode != DD_OK);
  }

  guiDirtyRegionExCount = 0;

ENDOFLOOP:

  fFirstTime = FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Direct X object access functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

LPDIRECTDRAW2 GetDirectDraw2Object(void) {
  Assert(gpDirectDrawObject != NULL);

  return gpDirectDrawObject;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

LPDIRECTDRAWSURFACE2 GetPrimarySurfaceObject(void) {
  Assert(gpPrimarySurface != NULL);

  return gpPrimarySurface;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

LPDIRECTDRAWSURFACE2 GetBackBufferObject(void) {
  Assert(gpPrimarySurface != NULL);

  return gpBackBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

LPDIRECTDRAWSURFACE2 GetFrameBufferObject(void) {
  Assert(gpPrimarySurface != NULL);

  return gpFrameBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

LPDIRECTDRAWSURFACE2 GetMouseBufferObject(void) {
  Assert(gpPrimarySurface != NULL);

  return gpMouseCursor;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Buffer access functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

PTR LockPrimarySurface(UINT32 *uiPitch) {
  HRESULT ReturnCode;
  DDSURFACEDESC SurfaceDescription;

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);

  do {
    ReturnCode = IDirectDrawSurface2_Lock(gpPrimarySurface, NULL, &SurfaceDescription, 0, NULL);
    if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
      DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to lock backbuffer");
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return NULL;
    }

  } while (ReturnCode != DD_OK);

  *uiPitch = SurfaceDescription.lPitch;
  return SurfaceDescription.lpSurface;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UnlockPrimarySurface(void) {
  DDSURFACEDESC SurfaceDescription;
  HRESULT ReturnCode;

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Unlock(gpPrimarySurface, &SurfaceDescription);
  if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PTR LockBackBuffer(UINT32 *uiPitch) {
  HRESULT ReturnCode;
  DDSURFACEDESC SurfaceDescription;

  //
  // W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ----
  //
  // This function is intended to be called by a thread which has already locked the
  // FRAME_BUFFER_MUTEX mutual exclusion section. Anything else will cause the application to
  // yack
  //

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);

  do {
    ReturnCode = IDirectDrawSurface2_Lock(gpBackBuffer, NULL, &SurfaceDescription, 0, NULL);
    if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
      DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to lock backbuffer");
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return NULL;
    }

  } while (ReturnCode != DD_OK);

  *uiPitch = SurfaceDescription.lPitch;
  return SurfaceDescription.lpSurface;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UnlockBackBuffer(void) {
  DDSURFACEDESC SurfaceDescription;
  HRESULT ReturnCode;

  //
  // W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ---- W A R N I N G ----
  //
  // This function is intended to be called by a thread which has already locked the
  // FRAME_BUFFER_MUTEX mutual exclusion section. Anything else will cause the application to
  // yack
  //

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Unlock(gpBackBuffer, &SurfaceDescription);
  if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PTR LockFrameBuffer(UINT32 *uiPitch) {
  HRESULT ReturnCode;
  DDSURFACEDESC SurfaceDescription;

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);

  do {
    ReturnCode = IDirectDrawSurface2_Lock(gpFrameBuffer, NULL, &SurfaceDescription, 0, NULL);
    if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
      DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to lock backbuffer");
      DirectXAttempt(ReturnCode, __LINE__, __FILE__);
      return NULL;
    }

  } while (ReturnCode != DD_OK);

  *uiPitch = SurfaceDescription.lPitch;

  return SurfaceDescription.lpSurface;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UnlockFrameBuffer(void) {
  DDSURFACEDESC SurfaceDescription;
  HRESULT ReturnCode;

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Unlock(gpFrameBuffer, &SurfaceDescription);
  if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PTR LockMouseBuffer(UINT32 *uiPitch) {
  HRESULT ReturnCode;
  DDSURFACEDESC SurfaceDescription;

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Lock(gpMouseCursorOriginal, NULL, &SurfaceDescription, 0, NULL);
  if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return NULL;
  }

  *uiPitch = SurfaceDescription.lPitch;

  return SurfaceDescription.lpSurface;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UnlockMouseBuffer(void) {
  DDSURFACEDESC SurfaceDescription;
  HRESULT ReturnCode;

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Unlock(gpMouseCursorOriginal, &SurfaceDescription);
  if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// RGB color management functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN GetRGBDistribution(void) {
  DDSURFACEDESC SurfaceDescription;
  UINT16 usBit;
  HRESULT ReturnCode;

  Assert(gpPrimarySurface != NULL);

  // ONLY DO IF WE ARE IN 16BIT MODE
  if (gbPixelDepth == 8) {
    return (TRUE);
  }

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  SurfaceDescription.dwFlags = DDSD_PIXELFORMAT;
  ReturnCode = IDirectDrawSurface2_GetSurfaceDesc(gpPrimarySurface, &SurfaceDescription);
  if (ReturnCode != DD_OK) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
    return FALSE;
  }

  //
  // Ok we now have the surface description, we now can get the information that we need
  //

  gusRedMask = (UINT16)SurfaceDescription.ddpfPixelFormat.dwRBitMask;
  gusGreenMask = (UINT16)SurfaceDescription.ddpfPixelFormat.dwGBitMask;
  gusBlueMask = (UINT16)SurfaceDescription.ddpfPixelFormat.dwBBitMask;

  // RGB 5,5,5
  if ((gusRedMask == 0x7c00) && (gusGreenMask == 0x03e0) && (gusBlueMask == 0x1f))
    guiTranslucentMask = 0x3def;
  // RGB 5,6,5
  else  // if((gusRedMask==0xf800) && (gusGreenMask==0x03e0) && (gusBlueMask==0x1f))
    guiTranslucentMask = 0x7bef;

  usBit = 0x8000;
  gusRedShift = 8;
  while (!(gusRedMask & usBit)) {
    usBit >>= 1;
    gusRedShift--;
  }

  usBit = 0x8000;
  gusGreenShift = 8;
  while (!(gusGreenMask & usBit)) {
    usBit >>= 1;
    gusGreenShift--;
  }

  usBit = 0x8000;
  gusBlueShift = 8;
  while (!(gusBlueMask & usBit)) {
    usBit >>= 1;
    gusBlueShift--;
  }

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN GetPrimaryRGBDistributionMasks(UINT32 *RedBitMask, UINT32 *GreenBitMask,
                                       UINT32 *BlueBitMask) {
  *RedBitMask = gusRedMask;
  *GreenBitMask = gusGreenMask;
  *BlueBitMask = gusBlueMask;

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN SetMouseCursorFromObject(UINT32 uiVideoObjectHandle, UINT16 usVideoObjectSubIndex,
                                 UINT16 usOffsetX, UINT16 usOffsetY) {
  BOOLEAN ReturnValue;
  PTR pTmpPointer;
  UINT32 uiPitch;
  ETRLEObject pETRLEPointer;

  //
  // Erase cursor background
  //

  pTmpPointer = LockMouseBuffer(&uiPitch);
  memset(pTmpPointer, 0, MAX_CURSOR_HEIGHT * uiPitch);
  UnlockMouseBuffer();

  //
  // Get new cursor data
  //

  ReturnValue = BltVideoObjectFromIndex(MOUSE_BUFFER, uiVideoObjectHandle, usVideoObjectSubIndex, 0,
                                        0, VO_BLT_SRCTRANSPARENCY, NULL);
  guiMouseBufferState = BUFFER_DIRTY;

  if (GetVideoObjectETRLEPropertiesFromIndex(uiVideoObjectHandle, &pETRLEPointer,
                                             usVideoObjectSubIndex)) {
    gsMouseCursorXOffset = usOffsetX;
    gsMouseCursorYOffset = usOffsetY;
    gusMouseCursorWidth = pETRLEPointer.usWidth + pETRLEPointer.sOffsetX;
    gusMouseCursorHeight = pETRLEPointer.usHeight + pETRLEPointer.sOffsetY;

    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "=================================================");
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0,
             String("Mouse Create with [ %d. %d ] [ %d, %d]", pETRLEPointer.sOffsetX,
                    pETRLEPointer.sOffsetY, pETRLEPointer.usWidth, pETRLEPointer.usHeight));
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "=================================================");

  } else {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to get mouse info");
  }

  return ReturnValue;
}

BOOLEAN EraseMouseCursor() {
  PTR pTmpPointer;
  UINT32 uiPitch;

  //
  // Erase cursor background
  //

  pTmpPointer = LockMouseBuffer(&uiPitch);
  memset(pTmpPointer, 0, MAX_CURSOR_HEIGHT * uiPitch);
  UnlockMouseBuffer();

  // Don't set dirty
  return (TRUE);
}

BOOLEAN SetMouseCursorProperties(INT16 sOffsetX, INT16 sOffsetY, UINT16 usCursorHeight,
                                 UINT16 usCursorWidth) {
  gsMouseCursorXOffset = sOffsetX;
  gsMouseCursorYOffset = sOffsetY;
  gusMouseCursorWidth = usCursorWidth;
  gusMouseCursorHeight = usCursorHeight;
  return (TRUE);
}

BOOLEAN BltToMouseCursor(UINT32 uiVideoObjectHandle, UINT16 usVideoObjectSubIndex, UINT16 usXPos,
                         UINT16 usYPos) {
  BOOLEAN ReturnValue;

  ReturnValue = BltVideoObjectFromIndex(MOUSE_BUFFER, uiVideoObjectHandle, usVideoObjectSubIndex,
                                        usXPos, usYPos, VO_BLT_SRCTRANSPARENCY, NULL);

  return ReturnValue;
}

void DirtyCursor() { guiMouseBufferState = BUFFER_DIRTY; }

void EnableCursor(BOOLEAN fEnable) {
  if (fEnable) {
    guiMouseBufferState = BUFFER_DISABLED;
  } else {
    guiMouseBufferState = BUFFER_READY;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN HideMouseCursor(void) {
  guiMouseBufferState = BUFFER_DISABLED;

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
BOOLEAN LoadCursorFile(PTR pFilename)
{
  VOBJECT_DESC VideoObjectDescription;

  //
  // Make sure the old cursor store is destroyed
  //

  if (gpCursorStore != NULL)
  {
    DeleteVideoObject(gpCursorStore);
    gpCursorStore = NULL;
  }

  //
  // Get the source file with all the cursors inside
  //

  VideoObjectDescription.fCreateFlags = VOBJECT_CREATE_FROMFILE;
        strcpy(VideoObjectDescription.ImageFile, pFilename);
  gpCursorStore = CreateVideoObject(&VideoObjectDescription);

  //
  // Were we successful in creating the cursor store ?
  //

  if (gpCursorStore == NULL)
  {
    return FALSE;
  }

  return TRUE;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOLEAN SetCurrentCursor(UINT16 usVideoObjectSubIndex, UINT16 usOffsetX, UINT16 usOffsetY) {
  BOOLEAN ReturnValue;
  PTR pTmpPointer;
  UINT32 uiPitch;
  ETRLEObject pETRLEPointer;

  //
  // Make sure we have a cursor store
  //

  if (gpCursorStore == NULL) {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "ERROR : Cursor store is not loaded");
    return FALSE;
  }

  //
  // Ok, then blit the mouse cursor to the MOUSE_BUFFER (which is really gpMouseBufferOriginal)
  //
  //
  // Erase cursor background
  //

  pTmpPointer = LockMouseBuffer(&uiPitch);
  memset(pTmpPointer, 0, MAX_CURSOR_HEIGHT * uiPitch);
  UnlockMouseBuffer();

  //
  // Get new cursor data
  //

  ReturnValue = BltVideoObject(MOUSE_BUFFER, gpCursorStore, usVideoObjectSubIndex, 0, 0,
                               VO_BLT_SRCTRANSPARENCY, NULL);
  guiMouseBufferState = BUFFER_DIRTY;

  if (GetVideoObjectETRLEProperties(gpCursorStore, &pETRLEPointer, usVideoObjectSubIndex)) {
    gsMouseCursorXOffset = usOffsetX;
    gsMouseCursorYOffset = usOffsetY;
    gusMouseCursorWidth = pETRLEPointer.usWidth + pETRLEPointer.sOffsetX;
    gusMouseCursorHeight = pETRLEPointer.usHeight + pETRLEPointer.sOffsetY;

    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "=================================================");
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0,
             String("Mouse Create with [ %d. %d ] [ %d, %d]", pETRLEPointer.sOffsetX,
                    pETRLEPointer.sOffsetY, pETRLEPointer.usWidth, pETRLEPointer.usHeight));
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "=================================================");
  } else {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, "Failed to get mouse info");
  }

  return ReturnValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void StartFrameBufferRender(void) { return; }

///////////////////////////////////////////////////////////////////////////////////////////////////

void EndFrameBufferRender(void) {
  guiFrameBufferState = BUFFER_DIRTY;

  return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void PrintScreen(void) { gfPrintFrameBuffer = TRUE; }

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN Set8BPPPalette(SGPPaletteEntry *pPalette) {
  HRESULT ReturnCode;

  // If we are in 256 colors, then we have to initialize the palette system to 0 (faded out)
  memcpy(gSgpPalette, pPalette, sizeof(SGPPaletteEntry) * 256);

  ReturnCode =
      IDirectDraw_CreatePalette(gpDirectDrawObject, (DDPCAPS_8BIT | DDPCAPS_ALLOW256),
                                (LPPALETTEENTRY)(&gSgpPalette[0]), &gpDirectDrawPalette, NULL);
  if (ReturnCode != DD_OK) {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, String("Failed to create palette (Rc = %d)", ReturnCode));
    return (FALSE);
  }
  // Apply the palette to the surfaces
  ReturnCode = IDirectDrawSurface_SetPalette(gpPrimarySurface, gpDirectDrawPalette);
  if (ReturnCode != DD_OK) {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, String("Failed to apply 8-bit palette to primary surface"));
    return (FALSE);
  }

  ReturnCode = IDirectDrawSurface_SetPalette(gpBackBuffer, gpDirectDrawPalette);
  if (ReturnCode != DD_OK) {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, String("Failed to apply 8-bit palette to back buffer"));
    return (FALSE);
  }

  ReturnCode = IDirectDrawSurface_SetPalette(gpFrameBuffer, gpDirectDrawPalette);
  if (ReturnCode != DD_OK) {
    DebugMsg(TOPIC_VIDEO, DBG_LEVEL_0, String("Failed to apply 8-bit palette to frame buffer"));
    return (FALSE);
  }

  return (TRUE);
}

void FatalError(STR8 pError, ...) {
  va_list argptr;

  va_start(argptr, pError);  // Set up variable argument pointer
  vsprintf(gFatalErrorString, pError, argptr);
  va_end(argptr);

  gfFatalError = TRUE;

  // Release DDraw
  IDirectDraw2_RestoreDisplayMode(gpDirectDrawObject);
  IDirectDraw2_Release(gpDirectDrawObject);
  ShowWindow(ghWindow, SW_HIDE);

  // destroy the window
  // DestroyWindow( ghWindow );

  gfProgramIsRunning = FALSE;

  MessageBox(ghWindow, gFatalErrorString, "JA2 Fatal Error", MB_OK | MB_TASKMODAL);
}

/*********************************************************************************
 * SnapshotSmall
 *
 *		Grabs a screen from the [rimary surface, and stuffs it into a 16-bit (RGB 5,5,5),
 * uncompressed Targa file. Each time the routine is called, it increments the
 * file number by one. The files are create in the current directory, usually the
 * EXE directory. This routine produces 1/4 sized images.
 *
 *********************************************************************************/

#pragma pack(push, 1)

typedef struct {
  UINT8 ubIDLength;
  UINT8 ubColorMapType;
  UINT8 ubTargaType;
  UINT16 usColorMapOrigin;
  UINT16 usColorMapLength;
  UINT8 ubColorMapEntrySize;
  UINT16 usOriginX;
  UINT16 usOriginY;
  UINT16 usImageWidth;
  UINT16 usImageHeight;
  UINT8 ubBitsPerPixel;
  UINT8 ubImageDescriptor;

} TARGA_HEADER;

#pragma pack(pop)

void SnapshotSmall(void) {
  INT32 iCountX, iCountY;
  DDSURFACEDESC SurfaceDescription;
  UINT16 *pVideo, *pDest;

  HRESULT ReturnCode;

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Lock(gpPrimarySurface, NULL, &SurfaceDescription, 0, NULL);
  if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
    return;
  }

  //	sprintf( cFilename, "JA%5.5d.TGA", uiPicNum++ );

  //	if( ( disk = fopen(cFilename, "wb"))==NULL )
  //		return;

  //	memset(&Header, 0, sizeof(TARGA_HEADER));

  //	Header.ubTargaType=2;			// Uncompressed 16/24/32 bit
  //	Header.usImageWidth=320;
  //	Header.usImageHeight=240;
  //	Header.ubBitsPerPixel=16;

  //	fwrite(&Header, sizeof(TARGA_HEADER), 1, disk);

  // Get the write pointer
  pVideo = (UINT16 *)SurfaceDescription.lpSurface;

  pDest = gpFrameData[giNumFrames];

  for (iCountY = SCREEN_HEIGHT - 1; iCountY >= 0; iCountY -= 1) {
    for (iCountX = 0; iCountX < SCREEN_WIDTH; iCountX += 1) {
      //		uiData=(UINT16)*(pVideo+(iCountY*640*2)+ ( iCountX * 2 ) );

      //				1111 1111 1100 0000
      //				f		 f		c
      //		usPixel555=	(UINT16)(uiData&0xffff);
      //			usPixel555= ((usPixel555 & 0xffc0) >> 1) | (usPixel555 & 0x1f);

      //		usPixel555=	(UINT16)(uiData);

      //	fwrite( &usPixel555, sizeof(UINT16), 1, disk);
      //		fwrite(	(void *)(((UINT8 *)SurfaceDescription.lpSurface) + ( iCountY * 640 *
      // 2) + ( iCountX * 2 ) ), 2 * sizeof( BYTE ), 1, disk );

      *(pDest + (iCountY * 640) + (iCountX)) = *(pVideo + (iCountY * 640) + (iCountX));
    }
  }

  giNumFrames++;

  if (giNumFrames == MAX_NUM_FRAMES) {
    RefreshMovieCache();
  }

  ZEROMEM(SurfaceDescription);
  SurfaceDescription.dwSize = sizeof(DDSURFACEDESC);
  ReturnCode = IDirectDrawSurface2_Unlock(gpPrimarySurface, &SurfaceDescription);
  if ((ReturnCode != DD_OK) && (ReturnCode != DDERR_WASSTILLDRAWING)) {
    DirectXAttempt(ReturnCode, __LINE__, __FILE__);
  }

  //	fclose(disk);
}

void VideoCaptureToggle(void) {
#ifdef JA2TESTVERSION
  VideoMovieCapture((BOOLEAN)!gfVideoCapture);
#endif
}

void VideoMovieCapture(BOOLEAN fEnable) {
  INT32 cnt;

  gfVideoCapture = fEnable;
  if (fEnable) {
    for (cnt = 0; cnt < MAX_NUM_FRAMES; cnt++) {
      gpFrameData[cnt] = (UINT16 *)MemAlloc(640 * 480 * 2);
    }

    giNumFrames = 0;

    guiLastFrame = GetTickCount();
  } else {
    RefreshMovieCache();

    for (cnt = 0; cnt < MAX_NUM_FRAMES; cnt++) {
      if (gpFrameData[cnt] != NULL) {
        MemFree(gpFrameData[cnt]);
      }
    }
    giNumFrames = 0;
  }
}

void RefreshMovieCache() {
  TARGA_HEADER Header;
  INT32 iCountX, iCountY;
  FILE *disk;
  CHAR8 cFilename[_MAX_PATH];
  static UINT32 uiPicNum = 0;
  UINT16 *pDest;
  INT32 cnt;
  STRING512 ExecDir;

  PauseTime(TRUE);

  GetExecutableDirectory(ExecDir);
  SetFileManCurrentDirectory(ExecDir);

  for (cnt = 0; cnt < giNumFrames; cnt++) {
    sprintf(cFilename, "JA%5.5d.TGA", uiPicNum++);

    if ((disk = fopen(cFilename, "wb")) == NULL) return;

    memset(&Header, 0, sizeof(TARGA_HEADER));

    Header.ubTargaType = 2;  // Uncompressed 16/24/32 bit
    Header.usImageWidth = 640;
    Header.usImageHeight = 480;
    Header.ubBitsPerPixel = 16;

    fwrite(&Header, sizeof(TARGA_HEADER), 1, disk);

    pDest = gpFrameData[cnt];

    for (iCountY = 480 - 1; iCountY >= 0; iCountY -= 1) {
      for (iCountX = 0; iCountX < 640; iCountX++) {
        fwrite((pDest + (iCountY * 640) + iCountX), sizeof(UINT16), 1, disk);
      }
    }

    fclose(disk);
  }

  PauseTime(FALSE);

  giNumFrames = 0;

  strcat(ExecDir, "\\Data");
  SetFileManCurrentDirectory(ExecDir);
}
