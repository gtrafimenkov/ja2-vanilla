#include "SGP/SGPAll.h"
#include "JA2Splash.h"
#include "Utils/Utilities.h"
#ifdef PRECOMPILEDHEADERS
#elif defined(WIZ8_PRECOMPILED_HEADERS)
#include "WIZ8 SGP ALL.H"
#else
#include "SGP/Types.h"
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "SGP/SGP.h"
#include "SGP/RegInst.h"
#include "SGP/VObject.h"
#include "SGP/Font.h"
#include "Local.h"
#include "SGP/FileMan.h"
#include "SGP/Input.h"
#include "SGP/Random.h"
#include "GameLoop.h"
#include "SGP/SoundMan.h"
#ifdef JA2
#include "JA2Splash.h"
#include "Utils/TimerControl.h"
#endif
#if !defined(JA2) && !defined(UTIL)
#include "GameData.h"  // for MoveTimer() [Wizardry specific]
#endif
#endif

#include "SGP/Input.h"
#include "zmouse.h"

#include "SGP/ExceptionHandling.h"
#include "dbt.h"

#include "Tactical/InterfacePanels.h"

#ifdef JA2
#include "BuildDefines.h"
#include "Intro.h"
#endif
#include "TileEngine/RenderWorld.h"
#include "TileEngine/RenderDirty.h"
#include "Strategic/MapScreenInterfaceMap.h"
#include "Strategic/MapScreen.h"
#include "Strategic/MapScreenInterface.h"
#include "laptop\laptop.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

extern UINT32 MemDebugCounter;
#ifdef JA2
extern BOOLEAN gfPauseDueToPlayerGamePause;
#endif

extern BOOLEAN CheckIfGameCdromIsInCDromDrive();
extern void QueueEvent(UINT16 ubInputEvent, UINT32 usParam, UINT32 uiParam);

// Prototype Declarations

INT32 FAR PASCAL WindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam);
BOOLEAN InitializeStandardGamingPlatform(HINSTANCE hInstance, int sCommandShow);
void ShutdownStandardGamingPlatform(void);
void GetRuntimeSettings();

int PASCAL HandledWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCommandLine,
                          int sCommandShow);

#if !defined(JA2) && !defined(UTILS)
void ProcessCommandLine(CHAR8 *pCommandLine);
BOOLEAN RunSetup(void);

// Should the game immediately load the quick save at startup?
BOOLEAN gfLoadAtStartup = FALSE;
BOOLEAN gfUsingBoundsChecker = FALSE;
CHAR8 *gzStringDataOverride = NULL;
BOOLEAN gfCapturingVideo = FALSE;

#endif

HINSTANCE ghInstance;

#ifdef JA2
void ProcessJa2CommandLineBeforeInitialization(CHAR8 *pCommandLine);
#endif

// Установка параметров разрешения экрана
void SetVideoParams(INT32 ScrW, INT32 ScrH);

// Global Variable Declarations
RECT rcWindow;
POINT ptWindowSize;
//***22.02.2014***
#ifdef WINDOWED_MODE
BOOLEAN gfWindowedMode = TRUE;
#else
BOOLEAN gfWindowedMode = FALSE;
#endif

//*******************************************************
INT32 giScrW;  // разрешение экрана по горизонтали
INT32 giScrH;  // разрешение экрана по вертикали

INT32 giOffsW;  // смещение рабочей области экрана по горизонтали
INT32 giOffsH;  // смещение рабочей области экрана по вертикали

//***18.11.2008***
INT16 gsRenderOffsetX;  //смещение спрайтов фигурки и курсоров в renderworld.c
INT16 gsRenderOffsetY;  //смещение спрайтов фигурки и курсоров в renderworld.c
//*******************************************************

// moved from header file: 24mar98:HJH
// UINT32		giStartMem;
UINT32 guiMouseWheelMsg;  // For mouse wheel messages

BOOLEAN gfApplicationActive;
BOOLEAN gfProgramIsRunning;
BOOLEAN gfGameInitialized = FALSE;
// UINT32	giStartMem;
BOOLEAN gfDontUseDDBlits = FALSE;

// There were TWO of them??!?! -- DB
// CHAR8		gzCommandLine[ 100 ];
CHAR8 gzCommandLine[100];  // Command line given

CHAR8 gzErrorMsg[2048] = "";
BOOLEAN gfIgnoreMessages = FALSE;

// GLOBAL VARIBLE, SET TO DEFAULT BUT CAN BE CHANGED BY THE GAME IF INIT FILE READ
UINT8 gbPixelDepth = PIXEL_DEPTH;

//<DR>
static DWORD WINAPI EmergencyExitButtonThreadProc(LPVOID lpParameter) {
  for (;;) {
    if (GetAsyncKeyState(VK_CANCEL))  // CTRL + BREAK
      ExitProcess(0);
    Sleep(300);
  }
}

static void EmergencyExitButtonInit() {
  UINT32 uiThreadId;

  SetThreadPriority(CreateThread(0, 0, EmergencyExitButtonThreadProc, 0, 0, (LPDWORD)&uiThreadId),
                    THREAD_PRIORITY_HIGHEST);
}
//</DR>

#define WM_POINTERUPDATE 0x0245
#define WM_POINTERDOWN 0x0246
#define WM_POINTERUP 0x0247

extern void MouseHandlerNew(UINT16 Message, WPARAM wParam, LPARAM lParam);
extern UINT16 gfAltState;
extern void TouchHandler(UINT16 Message, WPARAM wParam, LPARAM lParam);

INT32 FAR PASCAL WindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam) {
  static BOOLEAN fRestore = FALSE;

  if (gfIgnoreMessages) return (DefWindowProc(hWindow, Message, wParam, lParam));

  // ATE: This is for older win95 or NT 3.51 to get MOUSE_WHEEL Messages
  /* 04.02.2011 закомментировано
          if ( Message == guiMouseWheelMsg )
          {
        QueueEvent(MOUSE_WHEEL, wParam, lParam);
                          return( 0L );
          }
  */

  switch (Message) {
/* 04.02.2011 закомментировано
                case WM_MOUSEWHEEL:
                        {
                                QueueEvent(MOUSE_WHEEL, wParam, lParam);
                                break;
                        }
*/
#ifdef JA2
    //***09.02.2016***
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEWHEEL:
      MouseHandlerNew(Message, wParam, lParam);  //***04.02.2011*** обрабатываем все события мыши
                                                 //здесь из-за необходимости ловить дельту скролера
      break;
    //***09.02.2016***
    case WM_POINTERUPDATE:
    case WM_POINTERDOWN:
    case WM_POINTERUP:
    case WM_TIMER:
      TouchHandler(Message, wParam, lParam);
      break;

      /**    case WM_MOVE:

              GetClientRect(hWindow, &rcWindow);
              ClientToScreen(hWindow, (LPPOINT)&rcWindow);
              ClientToScreen(hWindow, (LPPOINT)&rcWindow+1);
              break;
      **/
      //переделано
    case WM_MOVE: {
      if (gfWindowedMode) {
        GetClientRect(hWindow, &rcWindow);
        rcWindow.right = giScrW;
        rcWindow.bottom = giScrH;
        ClientToScreen(hWindow, (LPPOINT)&rcWindow);
        ClientToScreen(hWindow, (LPPOINT)&rcWindow + 1);

        int xPos = (int)(short)LOWORD(lParam);
        int yPos = (int)(short)HIWORD(lParam);
        BOOL fNeedchange = FALSE;

        if (xPos < 0) {
          xPos = 0;
          fNeedchange = TRUE;
        }
        if (yPos < 0) {
          yPos = 0;
          fNeedchange = TRUE;
        }
        if (fNeedchange) {
          SetWindowPos(hWindow, NULL, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
        }

        if (gfWindowedMode == TRUE && FileExistsNoDB(NO_INI_FILE))  //***10.02.2016***
        {
          CHAR8 zStr[12];  //***09.02.2016***

          if (WritePrivateProfileString("Options", WINDOW_POS_X, _itoa(xPos, zStr, 10),
                                        NO_INI_FILE))
            WritePrivateProfileString("Options", WINDOW_POS_Y, _itoa(yPos, zStr, 10), NO_INI_FILE);
        }
      }
      break;
    }

    case WM_GETMINMAXINFO: {
      if (gfWindowedMode) {
        MINMAXINFO *mmi = (MINMAXINFO *)lParam;

        mmi->ptMaxSize = ptWindowSize;
        mmi->ptMaxTrackSize = mmi->ptMaxSize;
        mmi->ptMinTrackSize = mmi->ptMaxSize;
      }
      break;
    }

#else
    case WM_MOUSEMOVE:
      break;

    case WM_SIZING: {
      LPRECT lpWindow;
      INT32 iWidth, iHeight, iX, iY;
      BOOLEAN fWidthByHeight = FALSE, fHoldRight = FALSE;

      lpWindow = (LPRECT)lParam;

      iWidth = lpWindow->right - lpWindow->left;
      iHeight = lpWindow->bottom - lpWindow->top;
      iX = (lpWindow->left + lpWindow->right) / 2;
      iY = (lpWindow->top + lpWindow->bottom) / 2;

      switch (wParam) {
        case WMSZ_BOTTOMLEFT:
          fHoldRight = TRUE;
        case WMSZ_BOTTOM:
        case WMSZ_BOTTOMRIGHT:
          if (iHeight < giScrH) {
            lpWindow->bottom = lpWindow->top + giScrH;
            iHeight = giScrH;
          }
          fWidthByHeight = TRUE;
          break;

        case WMSZ_TOPLEFT:
          fHoldRight = TRUE;
        case WMSZ_TOP:
        case WMSZ_TOPRIGHT:
          if (iHeight < giScrH) {
            lpWindow->top = lpWindow->bottom - giScrH;
            iHeight = giScrH;
          }
          fWidthByHeight = TRUE;
          break;

        case WMSZ_LEFT:
          if (iWidth < giScrW) {
            lpWindow->left = lpWindow->right - giScrW;
            iWidth = giScrW;
          }
          break;

        case WMSZ_RIGHT:
          if (iWidth < giScrW) {
            lpWindow->right = lpWindow->left + giScrW;
            iWidth = giScrW;
          }
      }

      // Calculate width as a factor of height
      if (fWidthByHeight) {
        iWidth = iHeight * giScrW / giScrH;
        //				lpWindow->left = iX - iWidth/2;
        //				lpWindow->right = iX + iWidth / 2;
        if (fHoldRight)
          lpWindow->left = lpWindow->right - iWidth;
        else
          lpWindow->right = lpWindow->left + iWidth;
      } else  // Calculate height as a factor of width
      {
        iHeight = iWidth * giScrH / giScrW;
        //				lpWindow->top = iY - iHeight/2;
        //				lpWindow->bottom = iY + iHeight/2;
        lpWindow->bottom = lpWindow->top + iHeight;
      }

      /*
                              switch(wParam)
                              {
                                      case WMSZ_BOTTOM:
                                      case WMSZ_BOTTOMLEFT:
                                      case WMSZ_BOTTOMRIGHT:
                                              if(iHeight < SCREEN_HEIGHT)
                                                      lpWindow->bottom=lpWindow->top+SCREEN_HEIGHT;
                              }

                              switch(wParam)
                              {
                                      case WMSZ_TOP:
                                      case WMSZ_TOPLEFT:
                                      case WMSZ_TOPRIGHT:
                                              if(iHeight < SCREEN_HEIGHT)
                                                      lpWindow->top=lpWindow->bottom-SCREEN_HEIGHT;
                              }

                              switch(wParam)
                              {
                                      case WMSZ_BOTTOMLEFT:
                                      case WMSZ_LEFT:
                                      case WMSZ_TOPLEFT:
                                              if(iWidth < SCREEN_WIDTH)
                                                      lpWindow->left=lpWindow->right-SCREEN_WIDTH;
                              }

                              switch(wParam)
                              {
                                      case WMSZ_BOTTOMRIGHT:
                                      case WMSZ_RIGHT:
                                      case WMSZ_TOPRIGHT:
                                              if(iWidth < SCREEN_WIDTH)
                                                      lpWindow->right=lpWindow->left+SCREEN_WIDTH;
                              }
      */
    } break;

    case WM_SIZE: {
      UINT16 nWidth = LOWORD(lParam);   // width of client area
      UINT16 nHeight = HIWORD(lParam);  // height of client area

      if (nWidth && nHeight) {
        switch (wParam) {
          case SIZE_MAXIMIZED:
            VideoFullScreen(TRUE);
            break;

          case SIZE_RESTORED:
            VideoResizeWindow();
            break;
        }
      }
    } break;

    case WM_MOVE: {
      INT32 xPos = (INT32)LOWORD(lParam);  // horizontal position
      INT32 yPos = (INT32)HIWORD(lParam);  // vertical position
    } break;
#endif

    case WM_ACTIVATEAPP:
      switch (wParam) {
        case TRUE:  // We are restarting DirectDraw
          if (fRestore == TRUE) {
#ifdef JA2
            RestoreVideoManager();
            RestoreVideoSurfaces();  // Restore any video surfaces

            // unpause the JA2 Global clock
            if (!gfPauseDueToPlayerGamePause) {
              PauseTime(FALSE);
            }
#else
            if (!VideoInspectorIsEnabled()) {
              RestoreVideoManager();
              RestoreVideoSurfaces();  // Restore any video surfaces
            }

            MoveTimer(TIMER_RESUME);
#endif
            gfApplicationActive = TRUE;
          }
          break;
        case FALSE:  // We are suspending direct draw
#ifdef JA2
                     // pause the JA2 Global clock
          PauseTime(TRUE);
          SuspendVideoManager();
#else
#ifndef UTIL
          if (!VideoInspectorIsEnabled()) SuspendVideoManager();
#endif
#endif
          // suspend movement timer, to prevent timer crash if delay becomes long
          // * it doesn't matter whether the 3-D engine is actually running or not, or if it's even
          // been initialized
          // * restore is automatic, no need to do anything on reactivation
#if !defined(JA2) && !defined(UTIL)
          MoveTimer(TIMER_SUSPEND);
#endif

          gfApplicationActive = FALSE;
          fRestore = TRUE;
          break;
      }
      //***19.09.2014*** сброс залипшей Alt
      gfKeyState[ALT] = FALSE;
      gfAltState = FALSE;
      break;

    case WM_CREATE:
      break;

    case WM_DESTROY:
      ShutdownStandardGamingPlatform();
      ShowCursor(TRUE);
      PostQuitMessage(0);
      break;

    case WM_SETFOCUS:
#if !defined(JA2) && !defined(UTIL)
      if (!VideoInspectorIsEnabled()) RestoreVideoManager();
      gfApplicationActive = TRUE;
//			RestrictMouseToXYXY(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
#else
      RestoreCursorClipRect();
#endif

      break;

    case WM_KILLFOCUS:
#if !defined(JA2) && !defined(UTIL)
      if (!VideoInspectorIsEnabled()) SuspendVideoManager();

      gfApplicationActive = FALSE;
      FreeMouseCursor();
#endif
      // Set a flag to restore surfaces once a WM_ACTIVEATEAPP is received
      fRestore = TRUE;
      break;

#if defined(JA2)
#ifndef JA2DEMO
    case WM_DEVICECHANGE: {
      DEV_BROADCAST_HDR *pHeader = (DEV_BROADCAST_HDR *)lParam;

      // if a device has been removed
      if (wParam == DBT_DEVICEREMOVECOMPLETE) {
        // if its  a disk
        if (pHeader->dbch_devicetype == DBT_DEVTYP_VOLUME) {
          // check to see if the play cd is still in the cdrom
          if (!CheckIfGameCdromIsInCDromDrive()) {
          }
        }
      }
    } break;
#endif
#endif

    default:
      return DefWindowProc(hWindow, Message, wParam, lParam);
  }
  return 0L;
}

BOOLEAN InitializeStandardGamingPlatform(HINSTANCE hInstance, int sCommandShow) {
  FontTranslationTable *pFontTable;

  // now required by all (even JA2) in order to call ShutdownSGP
  atexit(SGPExit);

  // First, initialize the registry keys.
  InitializeRegistryKeys("Wizardry8", "Wizardry8key");

  // For rendering DLLs etc.
#ifndef JA2
  AddSubdirectoryToPath("DLL");
#endif

  // Second, read in settings
  GetRuntimeSettings();

  // Initialize the Debug Manager - success doesn't matter
  InitializeDebugManager();

  // Now start up everything else.
  RegisterDebugTopic(TOPIC_SGP, "Standard Gaming Platform");

  // this one needs to go ahead of all others (except Debug), for MemDebugCounter to work right...
  FastDebugMsg("Initializing Memory Manager");
  // Initialize the Memory Manager
  if (InitializeMemoryManager() == FALSE) {  // We were unable to initialize the memory manager
    FastDebugMsg("FAILED : Initializing Memory Manager");
    return FALSE;
  }

#ifdef JA2
  FastDebugMsg("Initializing Mutex Manager");
  // Initialize the Dirty Rectangle Manager
  if (InitializeMutexManager() == FALSE) {  // We were unable to initialize the game
    FastDebugMsg("FAILED : Initializing Mutex Manager");
    return FALSE;
  }
#endif

  FastDebugMsg("Initializing File Manager");
  // Initialize the File Manager
  if (InitializeFileManager(NULL) == FALSE) {  // We were unable to initialize the file manager
    FastDebugMsg("FAILED : Initializing File Manager");
    return FALSE;
  }

  FastDebugMsg("Initializing Containers Manager");
  InitializeContainers();

  FastDebugMsg("Initializing Input Manager");
  // Initialize the Input Manager
  if (InitializeInputManager() == FALSE) {  // We were unable to initialize the input manager
    FastDebugMsg("FAILED : Initializing Input Manager");
    return FALSE;
  }

  FastDebugMsg("Initializing Video Manager");
  // Initialize DirectDraw (DirectX 2)
  if (InitializeVideoManager(hInstance, (UINT16)sCommandShow, (void *)WindowProcedure) ==
      FALSE) {  // We were unable to initialize the video manager
    FastDebugMsg("FAILED : Initializing Video Manager");
    return FALSE;
  }

  // Initialize Video Object Manager
  FastDebugMsg("Initializing Video Object Manager");
  if (!InitializeVideoObjectManager()) {
    FastDebugMsg("FAILED : Initializing Video Object Manager");
    return FALSE;
  }

  // Initialize Video Surface Manager
  FastDebugMsg("Initializing Video Surface Manager");
  if (!InitializeVideoSurfaceManager()) {
    FastDebugMsg("FAILED : Initializing Video Surface Manager");
    return FALSE;
  }

#ifdef JA2
  InitJA2SplashScreen();
#endif

  // Make sure we start up our local clock (in milliseconds)
  // We don't need to check for a return value here since so far its always TRUE
  InitializeClockManager();  // must initialize after VideoManager, 'cause it uses ghWindow

  // Create font translation table (store in temp structure)
  pFontTable = CreateEnglishTransTable();
  if (pFontTable == NULL) {
    return (FALSE);
  }

  // Initialize Font Manager
  FastDebugMsg("Initializing the Font Manager");
  // Init the manager and copy the TransTable stuff into it.
  if (!InitializeFontManager(8, pFontTable)) {
    FastDebugMsg("FAILED : Initializing Font Manager");
    return FALSE;
  }
  // Don't need this thing anymore, so get rid of it (but don't de-alloc the contents)
  MemFree(pFontTable);

  FastDebugMsg("Initializing Sound Manager");
  // Initialize the Sound Manager (DirectSound)
#ifndef UTIL
  if (InitializeSoundManager() == FALSE) {  // We were unable to initialize the sound manager
    FastDebugMsg("FAILED : Initializing Sound Manager");
    return FALSE;
  }
#endif

  FastDebugMsg("Initializing Random");
  // Initialize random number generator
  InitializeRandom();  // no Shutdown

  FastDebugMsg("Initializing Game Manager");
  // Initialize the Game
  if (InitializeGame() == FALSE) {  // We were unable to initialize the game
    FastDebugMsg("FAILED : Initializing Game Manager");
    return FALSE;
  }

  // Register mouse wheel message
  ///	guiMouseWheelMsg = RegisterWindowMessage( MSH_MOUSEWHEEL );

  gfGameInitialized = TRUE;

  return TRUE;
}

void ShutdownStandardGamingPlatform(void) {
#ifndef JA2
  static BOOLEAN Reenter = FALSE;

  //
  // Prevent multiple reentry into this function
  //

  if (Reenter == FALSE) {
    Reenter = TRUE;
  } else {
    return;
  }
#endif

  //
  // Shut down the different components of the SGP
  //

  // TEST
  SoundServiceStreams();

  if (gfGameInitialized) {
    ShutdownGame();
  }

  ShutdownButtonSystem();
  MSYS_Shutdown();

#ifndef UTIL
  ShutdownSoundManager();
#endif

  DestroyEnglishTransTable();  // has to go before ShutdownFontManager()
  ShutdownFontManager();

  ShutdownClockManager();  // must shutdown before VideoManager, 'cause it uses ghWindow

#ifdef SGP_VIDEO_DEBUGGING
  PerformVideoInfoDumpIntoFile("SGPVideoShutdownDump.txt", FALSE);
#endif

  ShutdownVideoSurfaceManager();
  ShutdownVideoObjectManager();
  ShutdownVideoManager();

  ShutdownInputManager();
  ShutdownContainers();
  ShutdownFileManager();
#ifdef JA2
  ShutdownMutexManager();
#endif

#ifdef EXTREME_MEMORY_DEBUGGING
  DumpMemoryInfoIntoFile("ExtremeMemoryDump.txt", FALSE);
#endif

  ShutdownMemoryManager();  // must go last (except for Debug), for MemDebugCounter to work right...

  //
  // Make sure we unregister the last remaining debug topic before shutting
  // down the debugging layer
  UnRegisterDebugTopic(TOPIC_SGP, "Standard Gaming Platform");

  ShutdownDebugManager();
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCommandLine,
                   int sCommandShow) {
// If we are to use exception handling
#ifdef ENABLE_EXCEPTION_HANDLING
  int Result = -1;

  __try {
    Result = HandledWinMain(hInstance, hPrevInstance, pCommandLine, sCommandShow);
  } __except (RecordExceptionInfo(GetExceptionInformation())) {
    // Do nothing here - RecordExceptionInfo() has already done
    // everything that is needed. Actually this code won't even
    // get called unless you return EXCEPTION_EXECUTE_HANDLER from
    // the __except clause.
  }
  return Result;
}

// Do not place code in between WinMain and Handled WinMain

int PASCAL HandledWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCommandLine,
                          int sCommandShow) {
// DO NOT REMOVE, used for exception handing list above in WinMain
#endif
  MSG Message;
  HWND hPrevInstanceWindow;

  // Make sure that only one instance of this application is running at once
  // // Look for prev instance by searching for the window
  hPrevInstanceWindow = FindWindowEx(NULL, NULL, APPLICATION_NAME, APPLICATION_NAME);

  // One is found, bring it up!
  if (hPrevInstanceWindow != NULL) {
    SetForegroundWindow(hPrevInstanceWindow);
    ShowWindow(hPrevInstanceWindow, SW_RESTORE);
    return (0);
  }

  ghInstance = hInstance;

  //***14.07.2013***
  if (GetPrivateProfileInt("Options", "CtrlBrk", 0, NO_INI_FILE_CF)) EmergencyExitButtonInit();

#ifndef WINDOWED_MODE
  //***09.02.2016***
  int iRetVal = GetPrivateProfileInt("Options", "Windowed", -1, NO_INI_FILE_CF);

  if (iRetVal < 0 || iRetVal > 2) {
    gfWindowedMode = FALSE;

    if (IfWin8())
      iRetVal = 2;  //псевдополноэкранный режим
    else
      iRetVal = FALSE;

    WritePrivateProfileString("Options", "Windowed", _itoa(iRetVal, gzCommandLine, 10),
                              NO_INI_FILE_CF);
  } else
    gfWindowedMode = iRetVal;
#endif
  SetTabTipPath();  //путь к экранной клавиатуре

  // Copy commandline!
#ifdef JA2
  strncpy(gzCommandLine, pCommandLine, 100);
  gzCommandLine[99] = '\0';

  // Process the command line BEFORE initialization
  ProcessJa2CommandLineBeforeInitialization(pCommandLine);
#else
  ProcessCommandLine(pCommandLine);
#endif

  // Mem Usage
  ///	giStartMem = MemGetFree(  ) / 1024;

  //***20.01.2010*** определение национальной версии игры
  gbLocale = DetectLocale();

  /*
  #ifdef JA2
          // Handle Check for CD
          if ( !HandleJA2CDCheck( ) )
          {
                  return( 0 );
          }
  #else

          if(!RunSetup())
                  return(0);

  #endif
  */
  ShowCursor(FALSE);

  // Inititialize the SGP
  if (InitializeStandardGamingPlatform(hInstance, sCommandShow) ==
      FALSE) {  // We failed to initialize the SGP
    return 0;
  }

#ifdef JA2
#ifdef ENGLISH
  SetIntroType(INTRO_SPLASH);
#endif
#endif

  gfApplicationActive = TRUE;
  gfProgramIsRunning = TRUE;

  FastDebugMsg("Running Game");

  // At this point the SGP is set up, which means all I/O, Memory, tools, etc... are available. All
  // we need to do is attend to the gaming mechanics themselves
  while (gfProgramIsRunning) {
    if (PeekMessage(&Message, NULL, 0, 0,
                    PM_NOREMOVE)) {  // We have a message on the WIN95 queue, let's get it
      if (!GetMessage(&Message, NULL, 0, 0)) {  // It's quitting time
        return Message.wParam;
      }
      // Ok, now that we have the message, let's handle it
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    } else {  // Windows hasn't processed any messages, therefore we handle the rest
      if (gfApplicationActive ==
          FALSE) {  // Well we got nothing to do but to wait for a message to activate
        WaitMessage();
      } else {  // Well, the game is active, so we handle the game stuff
        GameLoop();

        // After this frame, reset input given flag
        gfSGPInputReceived = FALSE;
      }
    }
  }

  // This is the normal exit point
  FastDebugMsg("Exiting Game");
  PostQuitMessage(0);

  // SGPExit() will be called next through the atexit() mechanism...  This way we correctly process
  // both normal exits and emergency aborts (such as those caused by a failed assertion).

  // return wParam of the last message received
  return Message.wParam;
}

void SGPExit(void) {
  static BOOLEAN fAlreadyExiting = FALSE;
  BOOLEAN fUnloadScreens = TRUE;

  // helps prevent heap crashes when multiple assertions occur and call us
  if (fAlreadyExiting) {
    return;
  }

  fAlreadyExiting = TRUE;
  gfProgramIsRunning = FALSE;

// Wizardry only
#if !defined(JA2) && !defined(UTIL)
  if (gfGameInitialized) {
// ARM: if in DEBUG mode & we've ShutdownWithErrorBox, don't unload screens and release data structs
// to permit easier debugging
#ifdef _DEBUG
    if (gfIgnoreMessages) {
      fUnloadScreens = FALSE;
    }
#endif
    GameloopExit(fUnloadScreens);
  }
#endif

  ShutdownStandardGamingPlatform();
  ShowCursor(TRUE);
  if (strlen(gzErrorMsg)) {
    MessageBox(NULL, gzErrorMsg, "Error", MB_OK | MB_ICONERROR);
  }

#ifndef JA2
  VideoDumpMemoryLeaks();
#endif
}

void GetRuntimeSettings() {
  // Runtime settings - for now use INI file - later use registry
  STRING512 ExeDir;
  STRING512 INIFile;

  // Get Executable Directory
  GetExecutableDirectory(ExeDir);
  // Adjust Current Dir
  sprintf(INIFile, "%s\\sgp.ini", ExeDir);

  gbPixelDepth = GetPrivateProfileInt("SGP", "PIXEL_DEPTH", PIXEL_DEPTH, INIFile);
}

void ShutdownWithErrorBox(CHAR8 *pcMessage) {
  strncpy(gzErrorMsg, pcMessage, 255);
  gzErrorMsg[255] = '\0';
  gfIgnoreMessages = TRUE;

  exit(0);
}

#if !defined(JA2) && !defined(UTILS)

void ProcessCommandLine(CHAR8 *pCommandLine) {
  CHAR8 cSeparators[] = "\t =";
  CHAR8 *pCopy = NULL, *pToken;

  pCopy = (CHAR8 *)MemAlloc(strlen(pCommandLine) + 1);

  Assert(pCopy);
  if (!pCopy) return;

  memcpy(pCopy, pCommandLine, strlen(pCommandLine) + 1);

  pToken = strtok(pCopy, cSeparators);
  while (pToken) {
    if (!_strnicmp(pToken, "/NOSOUND", 8)) {
      SoundEnableSound(FALSE);
    } else if (!_strnicmp(pToken, "/INSPECTOR", 10)) {
      VideoInspectorEnable();
    } else if (!_strnicmp(pToken, "/VIDEOCFG", 9)) {
      pToken = strtok(NULL, cSeparators);
      VideoSetConfigFile(pToken);
    } else if (!_strnicmp(pToken, "/LOAD", 5)) {
      gfLoadAtStartup = TRUE;
    } else if (!_strnicmp(pToken, "/WINDOW", 7)) {
      VideoFullScreen(FALSE);
    } else if (!_strnicmp(pToken, "/BC", 7)) {
      gfUsingBoundsChecker = TRUE;
    } else if (!_strnicmp(pToken, "/CAPTURE", 7)) {
      gfCapturingVideo = TRUE;
    } else if (!_strnicmp(pToken, "/NOOCT", 6)) {
      NoOct();
    } else if (!_strnicmp(pToken, "/STRINGDATA", 11)) {
      pToken = strtok(NULL, cSeparators);
      gzStringDataOverride = (CHAR8 *)MemAlloc(strlen(pToken) + 1);
      strcpy(gzStringDataOverride, pToken);
    }

    pToken = strtok(NULL, cSeparators);
  }

  MemFree(pCopy);
}

BOOLEAN RunSetup(void) {
  if (!FileExists(VideoGetConfigFile()))
    _spawnl(_P_WAIT, "3DSetup.EXE", "3DSetup.EXE", VideoGetConfigFile(), NULL);

  return (FileExists(VideoGetConfigFile()));
}

#endif

//***10.07.2008***
void GetScreenResolutionFromINI(INT32 *pScrW, INT32 *pScrH) {
  CHAR8 zBuf[30], zTmp[8];

  GetPrivateProfileString("Options", "Screen", "", zBuf, 29, NO_INI_FILE_CF);

  if (!_strnicmp(zBuf, "640", 3)) {
    *pScrW = 640;
    *pScrH = 480;
    return;
  }

  if (!_strnicmp(zBuf, "800", 3)) {
    *pScrW = 800;
    *pScrH = 600;
    return;
  }

  if (!_strnicmp(zBuf, "1024", 4)) {
    *pScrW = 1024;
    *pScrH = 768;
    return;
  }

  if (!_strnicmp(zBuf, "1280", 4)) {
    *pScrW = 1280;
    *pScrH = 1024;
    return;
  }

  if (!_strnicmp(zBuf, "WXGA", 4)) {
    *pScrW = 1280;
    *pScrH = 800;
    return;
  }

  if (!_strnicmp(zBuf, "1360", 4)) {
    *pScrW = 1360;
    *pScrH = 768;
    return;
  }

  //***18.11.2008*** обработка произвольного разрешения экрана
  if (!_strnicmp(zBuf, "OTHER", 5)) {
    sscanf(zBuf, "%s%d%d%d%d", zTmp, pScrW, pScrH, &gsRenderOffsetX, &gsRenderOffsetY);
    return;
  }
}

//***22.05.2009***
extern BOOLEAN gfCheats;

void ProcessJa2CommandLineBeforeInitialization(CHAR8 *pCommandLine) {
  CHAR8 cSeparators[] = "\t =";
  CHAR8 *pCopy = NULL, *pToken;
  INT32 ScrW, ScrH;

  DEVMODE dm;
  int index = 0;

  // gfCheats = TRUE;

  //Установка разрешения экрана по умолчанию
  ScrW = 640;
  ScrH = 480;
  // ScrW=800; ScrH=600;
  // ScrW=1024; ScrH=768;
  // ScrW=1280; ScrH=1024;

  gsRenderOffsetX = 0;
  gsRenderOffsetY = 0;

  GetScreenResolutionFromINI(&ScrW, &ScrH);

  pCopy = (CHAR8 *)MemAlloc(strlen(pCommandLine) + 1);

  Assert(pCopy);
  if (!pCopy) return;

  memcpy(pCopy, pCommandLine, strlen(pCommandLine) + 1);

  pToken = strtok(pCopy, cSeparators);
  while (pToken) {
    // if its the NO SOUND option
    if (!_strnicmp(pToken, "-NOSOUND", 8)) {
      // disable the sound
      SoundEnableSound(FALSE);
    }

    if (!_strnicmp(pToken, "-CHEATS", 7)) {
      gfCheats = TRUE;
    }
    /*
                    if(!_strnicmp(pToken, "-CTRLBRK", 8))
                    {
                            //***20.01.2010*** аварийное завершение программы по Ctrl+Break, сделано
       опцией, чтобы не потреблять без надобности ресурсы ЦП EmergencyExitButtonInit(); //DR
                    }
    */
    //***22.02.2014***
    if (!_strnicmp(pToken, "-WINDOWED", 9)) {
      gfWindowedMode = TRUE;
    }

    if (!_strnicmp(pToken, "-800", 4)) {
      ScrW = 800;
      ScrH = 600;
    }

    if (!_strnicmp(pToken, "-1024", 5)) {
      ScrW = 1024;
      ScrH = 768;
    }

    if (!_strnicmp(pToken, "-1280", 5)) {
      ScrW = 1280;
      ScrH = 1024;
    }

    if (!_strnicmp(pToken, "-WXGA", 5)) {
      ScrW = 1280;
      ScrH = 800;
    }

    if (!_strnicmp(pToken, "-1360", 5)) {
      ScrW = 1360;
      ScrH = 768;
    }
    // get the next token
    pToken = strtok(NULL, cSeparators);
  }

  MemFree(pCopy);

  if (gfWindowedMode != TRUE)  //***09.02.2016***
  {
    dm.dmSize = sizeof(DEVMODE);
    while (EnumDisplaySettings(NULL, index, &dm)) {
      if (dm.dmPelsWidth == ScrW && dm.dmPelsHeight == ScrH /*&& dm.dmBitsPerPel == 16*/) {
        index = -1;
        break;
      }
      index++;
    }

    if (index != -1) {
      ScrW = 640;
      ScrH = 480;
    }
  }

  SetVideoParams(ScrW, ScrH);
}

void SetVideoParams(INT32 ScrW, INT32 ScrH) {
  int i;

  giScrW = ScrW;
  giScrH = ScrH;

  // XGA
  if (giScrW == 1024 && giScrH == 768) {
    gsRenderOffsetX = -8;
    gsRenderOffsetY = 5;
  }
  // SXGA
  if (giScrW == 1280 && giScrH == 1024) {
    gsRenderOffsetX = -2;
    gsRenderOffsetY = -8;
  }
  // WXGA
  if (giScrW == 1280 && giScrH == 800) {
    gsRenderOffsetX = -1;
    gsRenderOffsetY = 1;
  }
  // WXGA 2
  if (giScrW == 1360 && giScrH == 768) {
    gsRenderOffsetX = -1;
    gsRenderOffsetY = 4;
  }

  giOffsW = (giScrW - 640) / 2;
  giOffsH = (giScrH - 480) / 2;

  // interface items.c
  gMoneyButtonLoc.y = giScrH - 480 + gMoneyButtonLoc.y;
  gMapMoneyButtonLoc.y = giOffsH + gMapMoneyButtonLoc.y;
  gMapMoneyButtonLoc.x = giOffsW + gMapMoneyButtonLoc.x;

  // font.c
  FontDestPitch = giScrW * 2;
  FontDestRegion.iRight = giScrW;
  FontDestRegion.iBottom = giScrH;
  SaveFontDestPitch = giScrW * 2;
  SaveFontDestRegion.iRight = giScrW;
  SaveFontDestRegion.iBottom = giScrH;

  // button system.c
  ButtonDestPitch = giScrW * 2;

  // vobject_blitters.c
  ClippingRect.iRight = giScrW;
  ClippingRect.iBottom = giScrH;

  // render dirty.c
  gDirtyClipRect.iRight = giScrW;
  gDirtyClipRect.iBottom = giScrH;

  // renderworld.c
  gClippingRect.iRight = giScrW;
  gClippingRect.iBottom = giScrH - 120;
  gsVIEWPORT_END_Y = giScrH - 120;
  gsVIEWPORT_WINDOW_END_Y = giScrH - 120;
  gsVIEWPORT_END_X = giScrW;

  // interface panels.c
  for (i = 0; i < 19; i++) {
    gSMInvPocketXY[i].sY = giScrH - 480 + gSMInvPocketXY[i].sY;
  }
  gSMCamoXY.sY = giScrH - 480 + gSMCamoXY.sY;
  for (i = 1; i < 12; i += 2) {
    sTEAMAPPanelXY[i] = giScrH - 480 + sTEAMAPPanelXY[i];
    sTEAMFacesXY[i] = giScrH - 480 + sTEAMFacesXY[i];
    sTEAMNamesXY[i] = giScrH - 480 + sTEAMNamesXY[i];
    sTEAMFaceHighlXY[i] = giScrH - 480 + sTEAMFaceHighlXY[i];
    sTEAMLifeXY[i] = giScrH - 480 + sTEAMLifeXY[i];
    sTEAMBreathXY[i] = giScrH - 480 + sTEAMBreathXY[i];
    sTEAMMoraleXY[i] = giScrH - 480 + sTEAMMoraleXY[i];
    sTEAMApXY[i] = giScrH - 480 + sTEAMApXY[i];
    sTEAMBarsXY[i] = giScrH - 480 + sTEAMBarsXY[i];
    sTEAMHandInvXY[i] = giScrH - 480 + sTEAMHandInvXY[i];
  }

  // map screen interface map.c
  MapScreenRect.iLeft += giOffsW;
  MapScreenRect.iRight += giOffsW;
  MapScreenRect.iTop += giOffsH;
  MapScreenRect.iBottom += giOffsH;

  // mapscreen.c
  gSCamoXY.sX += giOffsW;
  gSCamoXY.sY += giOffsH;
  for (i = 0; i < 19; i++) {
    gMapScreenInvPocketXY[i].sX += giOffsW;
    gMapScreenInvPocketXY[i].sY += giOffsH;
    //! Car Lion 12.04.2014
    gMapScreenCarInvPocketXY[i].sX += giOffsW;
    gMapScreenCarInvPocketXY[i].sY += giOffsH;
  }

  // Map Screen Interface.c
  MovePosition.iX += giOffsW;
  MovePosition.iY += giOffsH;

  ContractPosition.iX += giOffsW;
  ContractPosition.iY += giOffsH;
  AttributePosition.iX += giOffsW;
  AttributePosition.iY += giOffsH;
  TrainPosition.iX += giOffsW;
  TrainPosition.iY += giOffsH;
  VehiclePosition.iX += giOffsW;
  VehiclePosition.iY += giOffsH;
  RepairPosition.iX += giOffsW;
  RepairPosition.iY += giOffsH;
  AssignmentPosition.iX += giOffsW;
  AssignmentPosition.iY += giOffsH;
  SquadPosition.iX += giOffsW;
  SquadPosition.iY += giOffsH;

  OrigContractPosition.iX += giOffsW;
  OrigContractPosition.iY += giOffsH;
  OrigAttributePosition.iX += giOffsW;
  OrigAttributePosition.iY += giOffsH;
  OrigSquadPosition.iX += giOffsW;
  OrigSquadPosition.iY += giOffsH;
  OrigAssignmentPosition.iX += giOffsW;
  OrigAssignmentPosition.iY += giOffsH;
  OrigTrainPosition.iX += giOffsW;
  OrigTrainPosition.iY += giOffsH;
  OrigVehiclePosition.iX += giOffsW;
  OrigVehiclePosition.iY += giOffsH;

  // laptop.c
  LaptopScreenRect.iLeft += giOffsW;
  LaptopScreenRect.iRight += giOffsW;
  LaptopScreenRect.iTop += giOffsH;
  LaptopScreenRect.iBottom += giOffsH;
}
