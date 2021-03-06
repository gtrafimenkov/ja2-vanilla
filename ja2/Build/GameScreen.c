#include "JA2All.h"
#include "HelpScreen.h"
#include "Strategic/PreBattleInterface.h"
#ifdef PRECOMPILEDHEADERS
#else
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "SGP/SGP.h"
#include "GameLoop.h"
#include "SGP/VObject.h"
#include "SGP/WCheck.h"
#include "TileEngine/WorldDef.h"
#include "TileEngine/RenderWorld.h"
#include "SGP/Input.h"
#include "SGP/Font.h"
#include "ScreenIDs.h"
#include "Screens.h"
#include "Tactical/Overhead.h"
#include "TileEngine/IsometricUtils.h"
#include "TileEngine/SysUtil.h"
#include "SGP/Input.h"
#include "Utils/EventPump.h"
#include "Utils/FontControl.h"
#include "Utils/TimerControl.h"
#include "TileEngine/RadarScreen.h"
#include "TileEngine/RenderDirty.h"
#include "Tactical/Interface.h"
#include "Tactical/HandleUI.h"
#include <wchar.h>
#include <tchar.h>
#include "Utils/Cursors.h"
#include "SGP/VObjectBlitters.h"
#include "SGP/ButtonSystem.h"
#include "TileEngine/Lighting.h"
#include "TileEngine/RenderWorld.h"
#include "SysGlobals.h"
#include "TileEngine/Environment.h"
#include "Utils/MusicControl.h"
#include "Tactical/Bullets.h"
#include "Strategic/Assignments.h"
#include "Utils/Message.h"
#include <string.h>
#include "TileEngine/OverheadMap.h"
#include "Tactical/StrategicExitGUI.h"
#include "Strategic/StrategicMovement.h"
#include "TileEngine/TacticalPlacementGUI.h"
#include "Tactical/AirRaid.h"
#include "Strategic/GameClock.h"
#include "Strategic/GameInit.h"

// DEF: Test Code
#ifdef NETWORKED
#include "Networking.h"
#endif
#include "Tactical/InterfaceControl.h"
#include "Strategic/GameClock.h"
#include "TileEngine/Physics.h"
#include "FadeScreen.h"
#include "Tactical/DialogueControl.h"
#include "Tactical/SoldierMacros.h"
#include "Tactical/Faces.h"
#include "Strategic/StrategicMap.h"
#include "GameScreen.h"
#include "Tactical/Interface.h"
#include "SGP/CursorControl.h"
#include "Strategic/StrategicTurns.h"

#include "Tactical/MercEntering.h"
#include "Tactical/SoldierCreate.h"
#include "Tactical/SoldierInitList.h"
#include "Tactical/InterfacePanels.h"
#include "Tactical/MapInformation.h"
#include "TileEngine/Environment.h"
#include "Tactical/Squads.h"
#include "Tactical/interfaceDialogue.h"
#include "Tactical/AutoBandage.h"
#include "Strategic/Meanwhile.h"
#include "Strategic/StrategicAI.h"
#include "HelpScreen.h"
#include "Strategic/PreBattleInterface.h"
#endif

#define ARE_IN_FADE_IN() (gfFadeIn || gfFadeInitialized)

BOOLEAN fDirtyRectangleMode = FALSE;
UINT16 *gpFPSBuffer = NULL;
// MarkNote
// extern ScrollStringStPtr pStringS=NULL;
UINT32 counter = 0;
UINT32 count = 0;
BOOLEAN gfTacticalDoHeliRun = FALSE;
BOOLEAN gfPlayAttnAfterMapLoad = FALSE;

// VIDEO OVERLAYS
INT32 giFPSOverlay = 0;
INT32 giCounterPeriodOverlay = 0;

BOOLEAN gfExitToNewSector = FALSE;
// UINT8		gubNewSectorExitDirection;

BOOLEAN gfGameScreenLocateToSoldier = FALSE;
BOOLEAN gfEnteringMapScreen = FALSE;
UINT32 uiOldMouseCursor;
UINT8 gubPreferredInitialSelectedGuy = NOBODY;

BOOLEAN gfTacticalIsModal = FALSE;
MOUSE_REGION gTacticalDisableRegion;
BOOLEAN gfTacticalDisableRegionActive = FALSE;
INT8 gbTacticalDisableMode = FALSE;
MODAL_HOOK gModalDoneCallback;
BOOLEAN gfBeginEndTurn = FALSE;
extern BOOLEAN gfTopMessageDirty;
extern BOOLEAN gfFailedToSaveGameWhenInsideAMessageBox;
extern BOOLEAN gfFirstHeliRun;
extern BOOLEAN gfRenderFullThisFrame;

// The InitializeGame function is responsible for setting up all data and Gaming Engine
// tasks which will run the game
RENDER_HOOK gRenderOverride = NULL;

#define NOINPUT_DELAY 60000
#define DEMOPLAY_DELAY 40000
#define RESTART_DELAY 6000

void TacticalScreenLocateToSoldier();

UINT32 guiTacticalLeaveScreenID;
BOOLEAN guiTacticalLeaveScreen = FALSE;

void HandleModalTactical();
extern void CheckForDisabledRegionRemove();
extern void InternalLocateGridNo(UINT16 sGridNo, BOOLEAN fForce);

UINT32 MainGameScreenInit(void) {
  VIDEO_OVERLAY_DESC VideoOverlayDesc;

  gpZBuffer = InitZBuffer(1280, 480);
  InitializeBackgroundRects();

  // EnvSetTimeInHours(ENV_TIME_12);

  SetRenderFlags(RENDER_FLAG_FULL);

  // Init Video Overlays
  // FIRST, FRAMERATE
  VideoOverlayDesc.sLeft = 0;
  VideoOverlayDesc.sTop = 0;
  VideoOverlayDesc.uiFontID = SMALLFONT1;
  VideoOverlayDesc.ubFontBack = FONT_MCOLOR_BLACK;
  VideoOverlayDesc.ubFontFore = FONT_MCOLOR_DKGRAY;
  VideoOverlayDesc.sX = VideoOverlayDesc.sLeft;
  VideoOverlayDesc.sY = VideoOverlayDesc.sTop;
  swprintf(VideoOverlayDesc.pzText, L"90");
  VideoOverlayDesc.BltCallback = BlitMFont;
  giFPSOverlay =
      RegisterVideoOverlay((VOVERLAY_STARTDISABLED | VOVERLAY_DIRTYBYTEXT), &VideoOverlayDesc);

  // SECOND, PERIOD COUNTER
  VideoOverlayDesc.sLeft = 30;
  VideoOverlayDesc.sTop = 0;
  VideoOverlayDesc.sX = VideoOverlayDesc.sLeft;
  VideoOverlayDesc.sY = VideoOverlayDesc.sTop;
  swprintf(VideoOverlayDesc.pzText, L"Levelnodes: 100000");
  VideoOverlayDesc.BltCallback = BlitMFont;
  giCounterPeriodOverlay =
      RegisterVideoOverlay((VOVERLAY_STARTDISABLED | VOVERLAY_DIRTYBYTEXT), &VideoOverlayDesc);

  // register debug topics
  RegisterJA2DebugTopic(TOPIC_JA2, "Reg JA2 Debug");
  // MarkNote

  return TRUE;
}

// The ShutdownGame function will free up/undo all things that were started in InitializeGame()
// It will also be responsible to making sure that all Gaming Engine tasks exit properly

UINT32 MainGameScreenShutdown(void) {
  ShutdownZBuffer(gpZBuffer);
  ShutdownBackgroundRects();

  // Remove video Overlays
  RemoveVideoOverlay(giFPSOverlay);

  return TRUE;
}

void FadeInGameScreen() {
  fFirstTimeInGameScreen = TRUE;

  FadeInNextFrame();
}

void FadeOutGameScreen() { FadeOutNextFrame(); }

void EnterTacticalScreen() {
  guiTacticalLeaveScreen = FALSE;

  SetPositionSndsActive();

  // Set pending screen
  SetPendingNewScreen(GAME_SCREEN);

  // Set as active...
  gTacticalStatus.uiFlags |= ACTIVE;

  fInterfacePanelDirty = DIRTYLEVEL2;

  // Disable all faces
  SetAllAutoFacesInactive();

  // CHECK IF OURGUY IS NOW OFF DUTY
  if (gusSelectedSoldier != NOBODY) {
    if (!OK_CONTROLLABLE_MERC(MercPtrs[gusSelectedSoldier])) {
      SelectNextAvailSoldier(MercPtrs[gusSelectedSoldier]);
    }
    // ATE: If the current guy is sleeping, change....
    if (MercPtrs[gusSelectedSoldier]->fMercAsleep) {
      SelectNextAvailSoldier(MercPtrs[gusSelectedSoldier]);
    }
  } else {
    // otherwise, make sure interface is team panel...
    SetCurrentInterfacePanel((UINT8)TEAM_PANEL);
  }

  if (!gfTacticalPlacementGUIActive) {
    MSYS_EnableRegion(&gRadarRegion);
  }
  MSYS_EnableRegion(&gViewportRegion);

  // set default squad on sector entry
  // ATE: moved these 2 call after initalizing the interface!
  // SetDefaultSquadOnSectorEntry( FALSE );
  // ExamineCurrentSquadLights( );

  // UpdateMercsInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

  // Init interface ( ALWAYS TO TEAM PANEL.  DEF changed it to go back to the previous panel )
  if (!gfTacticalPlacementGUIActive) {
    // make sure the gsCurInterfacePanel is valid
    if (gsCurInterfacePanel < 0 || gsCurInterfacePanel >= NUM_UI_PANELS)
      gsCurInterfacePanel = TEAM_PANEL;

    SetCurrentInterfacePanel((UINT8)gsCurInterfacePanel);
  }

  SetTacticalInterfaceFlags(0);

  // set default squad on sector entry
  SetDefaultSquadOnSectorEntry(FALSE);
  ExamineCurrentSquadLights();

  fFirstTimeInGameScreen = FALSE;

  // Make sure it gets re-created....
  DirtyTopMessage();

  // Set compression to normal!
  // SetGameTimeCompressionLevel( TIME_COMPRESS_X1 );

  // Select current guy...
  // gfGameScreenLocateToSoldier = TRUE;

  // Locate if in meanwhile...
  if (AreInMeanwhile()) {
    LocateToMeanwhileCharacter();
  }

  if (gTacticalStatus.uiFlags & IN_DEIDRANNA_ENDGAME) {
    InternalLocateGridNo(4561, TRUE);
  }

  // Clear tactical message q
  ClearTacticalMessageQueue();

  // ATE: Enable messages again...
  EnableScrollMessages();
}

void LeaveTacticalScreen(UINT32 uiNewScreen) {
  guiTacticalLeaveScreenID = uiNewScreen;
  guiTacticalLeaveScreen = TRUE;
}

void InternalLeaveTacticalScreen(UINT32 uiNewScreen) {
  gpCustomizableTimerCallback = NULL;

  // unload the sector they teleported out of
  if (!gfAutomaticallyStartAutoResolve) {
    CheckAndHandleUnloadingOfCurrentWorld();
  }

  SetPositionSndsInActive();

  // Turn off active flag
  gTacticalStatus.uiFlags &= (~ACTIVE);

  fFirstTimeInGameScreen = TRUE;

  SetPendingNewScreen(uiNewScreen);

  // Disable all faces
  SetAllAutoFacesInactive();

  ResetInterfaceAndUI();

  // Remove cursor and reset height....
  gsGlobalCursorYOffset = 0;
  SetCurrentCursorFromDatabase(VIDEO_NO_CURSOR);

  // Shutdown panel
  ShutdownCurrentPanel();

  // disable the radar map
  MSYS_DisableRegion(&gRadarRegion);
  // MSYS_DisableRegion( &gViewportRegion );

  // We are leaving... turn off pedning autobadage...
  SetAutoBandagePending(FALSE);

  // ATE: Disable messages....
  DisableScrollMessages();

  if (uiNewScreen == MAINMENU_SCREEN) {
    // We want to reinitialize the game
    ReStartingGame();
  }

  if (uiNewScreen != MAP_SCREEN) {
    StopAnyCurrentlyTalkingSpeech();
  }

  // If we have some disabled screens up.....remove...
  CheckForDisabledRegionRemove();

  // ATE: Record last time we were in tactical....
  gTacticalStatus.uiTimeSinceLastInTactical = GetWorldTotalMin();

  FinishAnySkullPanelAnimations();
}

extern INT32 iInterfaceDialogueBox;

#ifdef JA2BETAVERSION
extern BOOLEAN ValidateSoldierInitLinks(UINT8 ubCode);
extern BOOLEAN gfDoDialogOnceGameScreenFadesIn;
#endif

UINT32 MainGameScreenHandle(void) {
  UINT32 uiNewScreen = GAME_SCREEN;
  BOOLEAN fEnterDemoMode = FALSE;

  // DO NOT MOVE THIS FUNCTION CALL!!!
  // This determines if the help screen should be active
  //	if( ( !gfTacticalDoHeliRun && !gfFirstHeliRun ) && ShouldTheHelpScreenComeUp(
  // HELP_SCREEN_TACTICAL, FALSE ) )
  if (!gfPreBattleInterfaceActive && ShouldTheHelpScreenComeUp(HELP_SCREEN_TACTICAL, FALSE)) {
    // handle the help screen
    HelpScreenHandler();
    return (GAME_SCREEN);
  }

#ifdef JA2BETAVERSION
  DebugValidateSoldierData();
#endif

  if (HandleAutoBandage()) {
#ifndef VISIBLE_AUTO_BANDAGE
    return (GAME_SCREEN);
#endif
  }

#if 0
	{
		PTR pData, pDestBuf;
		UINT32 uiPitch, uiDestPitchBYTES;

		pData = LockMouseBuffer( &uiPitch );

		pDestBuf = LockVideoSurface(guiRENDERBUFFER, &uiDestPitchBYTES);

		Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES, 
					(UINT16 *)pData, uiPitch,  
					0 , 0, 
					0 , 0, 
					64, 64 );

		UnlockMouseBuffer( );
		UnLockVideoSurface(guiRENDERBUFFER);
		InvalidateRegion( 0, 0, 64, 64 );

		//mprintf( 0, 55, L"W: %dH: %d", gsCurMouseWidth, gsCurMouseHeight );
	}
#endif

  if (gfBeginEndTurn) {
    UIHandleEndTurn(NULL);
    gfBeginEndTurn = FALSE;
  }

#ifdef JA2DEMO
  SetGameTimeCompressionLevel(TIME_COMPRESS_X1);
#endif
  /*
          if ( gfExitToNewSector )
          {
                  // Shade screen one frame
                  if ( gfExitToNewSector == 1 )
                  {
                          // Disable any saved regions!
                          EmptyBackgroundRects( );

                          // Remove cursor
                          uiOldMouseCursor = gViewportRegion.Cursor;
                          SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

                          //Shadow area
                          ShadowVideoSurfaceRect( FRAME_BUFFER, 0, 0, 640, 480 );
                          InvalidateScreen( );

                          // Next frame please
                          gfExitToNewSector++;

                          return( GAME_SCREEN );
                  }
                  else
                  {
                          // Go into sector
                          JumpIntoAdjacentSector( gubNewSectorExitDirection );
                          gfExitToNewSector = FALSE;

                          // Restore mouse
                          SetCurrentCursorFromDatabase( uiOldMouseCursor );
                  }
          }
  */

  // The gfFailedToSaveGameWhenInsideAMessageBox flag will only be set at this point if the game
  // fails to save during a quick save and when the game was already in a message box.
  // If the game failed to save when in a message box, pop up a message box stating an error occured
  if (gfFailedToSaveGameWhenInsideAMessageBox) {
    gfFailedToSaveGameWhenInsideAMessageBox = FALSE;

    DoMessageBox(MSG_BOX_BASIC_STYLE, zSaveLoadText[SLG_SAVE_GAME_ERROR], GAME_SCREEN,
                 MSG_BOX_FLAG_OK, NULL, NULL);

    return (GAME_SCREEN);
  }

  // Check if we are in bar animation...
  if (InTopMessageBarAnimation()) {
    ExecuteBaseDirtyRectQueue();

    EndFrameBufferRender();

    return (GAME_SCREEN);
  }

  if (gfTacticalIsModal) {
    if (gfTacticalIsModal == 1) {
      gfTacticalIsModal++;
    } else {
      HandleModalTactical();

      return (GAME_SCREEN);
    }
  }

  // OK, this is the pause system for when we see a guy...
  if (!ARE_IN_FADE_IN()) {
    if (gTacticalStatus.fEnemySightingOnTheirTurn) {
      if ((GetJA2Clock() - gTacticalStatus.uiTimeSinceDemoOn) > 3000) {
        if (gTacticalStatus.ubCurrentTeam != gbPlayerNum) {
          AdjustNoAPToFinishMove(MercPtrs[gTacticalStatus.ubEnemySightingOnTheirTurnEnemyID],
                                 FALSE);
        }
        MercPtrs[gTacticalStatus.ubEnemySightingOnTheirTurnEnemyID]->fPauseAllAnimation = FALSE;

        gTacticalStatus.fEnemySightingOnTheirTurn = FALSE;
      }
    }
  }

  // see if the helicopter is coming in this time for the initial entrance by the mercs
  InitHelicopterEntranceByMercs();

  // Handle Environment controller here
  EnvironmentController(TRUE);

  if (!ARE_IN_FADE_IN()) {
    HandleWaitTimerForNPCTrigger();

    // Check timer that could have been set to do anything
    CheckCustomizableTimer();

    // HAndle physics engine
    SimulateWorld();

    // Handle strategic engine
    HandleStrategicTurn();
  }

  if (gfTacticalDoHeliRun) {
    gfGameScreenLocateToSoldier = FALSE;
    InternalLocateGridNo(gMapInformation.sNorthGridNo, TRUE);

    // Start heli Run...
    StartHelicopterRun(gMapInformation.sNorthGridNo);

    // Update clock by one so that our DidGameJustStatrt() returns now false for things like LAPTOP,
    // etc...
    SetGameTimeCompressionLevel(TIME_COMPRESS_X1);
    // UpdateClock( 1 );

    gfTacticalDoHeliRun = FALSE;
    // SetMusicMode( MUSIC_TACTICAL_NOTHING );
  }

  if (InOverheadMap()) {
    HandleOverheadMap();
    return (GAME_SCREEN);
  }

  if (!ARE_IN_FADE_IN()) {
    HandleAirRaid();
  }

  if (gfGameScreenLocateToSoldier) {
    TacticalScreenLocateToSoldier();
    gfGameScreenLocateToSoldier = FALSE;
  }

  if (fFirstTimeInGameScreen) {
    EnterTacticalScreen();

    // Select a guy if he hasn;'
    if (!gfTacticalPlacementGUIActive) {
      if (gusSelectedSoldier != NOBODY && OK_INTERRUPT_MERC(MercPtrs[gusSelectedSoldier])) {
        SelectSoldier(gusSelectedSoldier, FALSE, TRUE);
      }
    }
  }

  // ATE: check that this flag is not set.... display message if so
  if (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN) {
    // Unset
    guiTacticalInterfaceFlags &= (~INTERFACE_MAPSCREEN);

#ifdef JA2BETAVERSION
    ScreenMsg(FONT_ORANGE, MSG_BETAVERSION,
              L"MAPSCREEN_INTERFACE flag set: Please remember how you entered Tactical.");
#endif
  }

  if (HandleFadeOutCallback()) {
    return (GAME_SCREEN);
  }

  if (guiCurrentScreen != MSG_BOX_SCREEN) {
    if (HandleBeginFadeOut(GAME_SCREEN)) {
      return (GAME_SCREEN);
    }
  }

#ifdef JA2BETAVERSION
  if (gfDoDialogOnceGameScreenFadesIn) {
    ValidateSoldierInitLinks(4);
  }
#endif

  HandleHeliDrop();

  if (!ARE_IN_FADE_IN()) {
    HandleAutoBandagePending();
  }

  // ATE: CHRIS_C LOOK HERE FOR GETTING AI CONSTANTLY GOING
  // if ( gTacticalStatus.uiFlags & TURNBASED )
  //{
  //	if ( !(gTacticalStatus.uiFlags & ENEMYS_TURN) )
  //	{
  //		EndTurn( );
  //	}
  //}

  //	if ( gfScrollInertia == FALSE )
  {
    if (!ARE_IN_FADE_IN()) {
      UpdateBullets();

      // Execute Tactical Overhead
      ExecuteOverhead();
    }

    // Handle animated cursors
    if (gfWorldLoaded) {
      HandleAnimatedCursors();

      // Handle Interface
      uiNewScreen = HandleTacticalUI();

      // called to handle things like face panels changeing due to team panel, squad changes, etc
      // To be done AFTER HandleUI and before ExecuteOverlays( )
      HandleDialogueUIAdjustments();

      HandleTalkingAutoFaces();
    }
#ifdef JA2EDITOR
    else if (gfIntendOnEnteringEditor) {
      OutputDebugString("Aborting normal game mode and entering editor mode...\n");
      SetPendingNewScreen(0xffff);  // NO_SCREEN
      return EDIT_SCREEN;
    }
#endif
    else if (!gfEnteringMapScreen) {
      gfEnteringMapScreen = TRUE;
    }

    if (uiNewScreen != GAME_SCREEN) {
      return (uiNewScreen);
    }

    // Deque all game events
    if (!ARE_IN_FADE_IN()) {
      DequeAllGameEvents(TRUE);
    }
  }

  /////////////////////////////////////////////////////
  StartFrameBufferRender();

  HandleTopMessages();

  if (gfScrollPending || gfScrollInertia) {
  } else {
    // Handle Interface Stuff
    SetUpInterface();
    HandleTacticalPanelSwitch();
  }

  // Handle Scroll Of World
  ScrollWorld();

  // SetRenderFlags( RENDER_FLAG_FULL );

  RenderWorld();

  if (gRenderOverride != NULL) {
    gRenderOverride();
  }

  if (gfScrollPending || gfScrollInertia) {
    RenderTacticalInterfaceWhileScrolling();
  } else {
    // Handle Interface Stuff
    // RenderTacticalInterface( );
  }

#ifdef NETWORKED
  // DEF:  Test Code
  PrintNetworkInfo();
#endif

  // Render Interface
  RenderTopmostTacticalInterface();

#ifdef JA2TESTVERSION
  if (gTacticalStatus.uiFlags & ENGAGED_IN_CONV) {
    SetFont(MILITARYFONT1);
    SetFontBackground(FONT_MCOLOR_BLACK);
    SetFontForeground(FONT_MCOLOR_LTGREEN);

    mprintf(0, 0, L"IN CONVERSATION %d", giNPCReferenceCount);
    gprintfdirty(0, 0, L"IN CONVERSATION %d", giNPCReferenceCount);
  }

#ifdef JA2BETAVERSION

  if (GamePaused() == TRUE) {
    SetFont(MILITARYFONT1);
    SetFontBackground(FONT_MCOLOR_BLACK);
    SetFontForeground(FONT_MCOLOR_LTGREEN);

    mprintf(0, 10, L"Game Clock Paused");
    gprintfdirty(0, 10, L"Game Clock Paused");
  }

#endif

  if (gTacticalStatus.uiFlags & SHOW_ALL_MERCS) {
    INT32 iSchedules;
    SCHEDULENODE *curr;

    SetFont(MILITARYFONT1);
    SetFontBackground(FONT_MCOLOR_BLACK);
    SetFontForeground(FONT_MCOLOR_LTGREEN);

    mprintf(0, 15, L"Attacker Busy Count: %d", gTacticalStatus.ubAttackBusyCount);
    gprintfdirty(0, 15, L"Attacker Busy Count: %d", gTacticalStatus.ubAttackBusyCount);

    curr = gpScheduleList;
    iSchedules = 0;
    while (curr) {
      iSchedules++;
      curr = curr->next;
    }

    mprintf(0, 25, L"Schedules: %d", iSchedules);
    gprintfdirty(0, 25, L"Schedules: %d", iSchedules);
  }
#endif

  // Render view window
  RenderRadarScreen();

  ResetInterface();

  if (gfScrollPending) {
    AllocateVideoOverlaysArea();
    SaveVideoOverlaysArea(FRAME_BUFFER);
    ExecuteVideoOverlays();
  } else {
    ExecuteVideoOverlays();
  }

  // Adding/deleting of video overlays needs to be done below
  // ExecuteVideoOverlays( )....

  // Handle dialogue queue system
  if (!ARE_IN_FADE_IN()) {
    if (gfPlayAttnAfterMapLoad) {
      gfPlayAttnAfterMapLoad = FALSE;

      if (gusSelectedSoldier != NOBODY) {
        if (!gGameSettings.fOptions[TOPTION_MUTE_CONFIRMATIONS])
          DoMercBattleSound(MercPtrs[gusSelectedSoldier], BATTLE_SOUND_ATTN1);
      }
    }

    HandleDialogue();
  }

  // Don't render if we have a scroll pending!
  if (!gfScrollPending && !gfScrollInertia && !gfRenderFullThisFrame) {
    RenderButtonsFastHelp();
  }

  // Display Framerate
  DisplayFrameRate();

  CheckForMeanwhileOKStart();

  ScrollString();

  ExecuteBaseDirtyRectQueue();

  // KillBackgroundRects( );

  /////////////////////////////////////////////////////
  EndFrameBufferRender();

  if (HandleFadeInCallback()) {
    // Re-render the scene!
    SetRenderFlags(RENDER_FLAG_FULL);
    fInterfacePanelDirty = DIRTYLEVEL2;
  }

  if (HandleBeginFadeIn(GAME_SCREEN)) {
    guiTacticalLeaveScreenID = FADE_SCREEN;
  }

  if (guiTacticalLeaveScreen) {
    guiTacticalLeaveScreen = FALSE;

    InternalLeaveTacticalScreen(guiTacticalLeaveScreenID);
  }

  // Check if we are to enter map screen
  if (gfEnteringMapScreen == 2) {
    gfEnteringMapScreen = FALSE;
    EnterMapScreen();
  }

  // Are we entering map screen? if so, wait a frame!
  if (gfEnteringMapScreen > 0) {
    gfEnteringMapScreen++;
  }

  return (GAME_SCREEN);
}

void SetRenderHook(RENDER_HOOK pRenderOverride) { gRenderOverride = pRenderOverride; }

void DisableFPSOverlay(BOOLEAN fEnable) {
  VIDEO_OVERLAY_DESC VideoOverlayDesc;

  memset(&VideoOverlayDesc, 0, sizeof(VideoOverlayDesc));

  VideoOverlayDesc.fDisabled = fEnable;
  VideoOverlayDesc.uiFlags = VOVERLAY_DESC_DISABLED;

  UpdateVideoOverlay(&VideoOverlayDesc, giFPSOverlay, FALSE);
  UpdateVideoOverlay(&VideoOverlayDesc, giCounterPeriodOverlay, FALSE);
}

void TacticalScreenLocateToSoldier() {
  INT32 cnt;
  SOLDIERTYPE *pSoldier;
  INT16 bLastTeamID;
  BOOLEAN fPreferedGuyUsed = FALSE;

  if (gubPreferredInitialSelectedGuy != NOBODY) {
    // ATE: Put condition here...
    if (OK_CONTROLLABLE_MERC(MercPtrs[gubPreferredInitialSelectedGuy]) &&
        OK_INTERRUPT_MERC(MercPtrs[gubPreferredInitialSelectedGuy])) {
      LocateSoldier(gubPreferredInitialSelectedGuy, 10);
      SelectSoldier(gubPreferredInitialSelectedGuy, FALSE, TRUE);
      fPreferedGuyUsed = TRUE;
    }
    gubPreferredInitialSelectedGuy = NOBODY;
  }

  if (!fPreferedGuyUsed) {
    // Set locator to first merc
    cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;
    bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
    for (pSoldier = MercPtrs[cnt]; cnt <= bLastTeamID; cnt++, pSoldier++) {
      if (OK_CONTROLLABLE_MERC(pSoldier) && OK_INTERRUPT_MERC(pSoldier)) {
        LocateSoldier(pSoldier->ubID, 10);
        SelectSoldier(pSoldier->ubID, FALSE, TRUE);
        break;
      }
    }
  }
}

void EnterMapScreen() {
  // ATE: These flags well get set later on in mapscreen....
  // SetTacticalInterfaceFlags( INTERFACE_MAPSCREEN );
  // fInterfacePanelDirty = DIRTYLEVEL2;
  LeaveTacticalScreen(MAP_SCREEN);
}

void UpdateTeamPanelAssignments() {
  INT32 cnt;
  SOLDIERTYPE *pSoldier;
  INT16 bLastTeamID;

  // Remove all players
  RemoveAllPlayersFromSlot();

  // Set locator to first merc
  cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;
  bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
  for (pSoldier = MercPtrs[cnt]; cnt <= bLastTeamID; cnt++, pSoldier++) {
    // Setup team interface
    CheckForAndAddMercToTeamPanel(pSoldier);
  }
}

void EnterModalTactical(INT8 bMode) {
  gbTacticalDisableMode = bMode;
  gfTacticalIsModal = TRUE;

  if (gbTacticalDisableMode == TACTICAL_MODAL_NOMOUSE) {
    if (!gfTacticalDisableRegionActive) {
      gfTacticalDisableRegionActive = TRUE;

      MSYS_DefineRegion(&gTacticalDisableRegion, 0, 0, 640, 480, MSYS_PRIORITY_HIGH,
                        VIDEO_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK);
      // Add region
      MSYS_AddRegion(&gTacticalDisableRegion);
    }
  }

  UpdateSaveBuffer();
}

void EndModalTactical() {
  if (gfTacticalDisableRegionActive) {
    MSYS_RemoveRegion(&gTacticalDisableRegion);

    gfTacticalDisableRegionActive = FALSE;
  }

  if (gModalDoneCallback != NULL) {
    gModalDoneCallback();

    gModalDoneCallback = NULL;
  }

  gfTacticalIsModal = FALSE;

  SetRenderFlags(RENDER_FLAG_FULL);
}

void HandleModalTactical() {
  StartFrameBufferRender();

  RestoreBackgroundRects();

  RenderWorld();
  RenderRadarScreen();
  ExecuteVideoOverlays();

  // Handle dialogue queue system
  HandleDialogue();

  HandleTalkingAutoFaces();

  // Handle faces
  HandleAutoFaces();

  if (gfInSectorExitMenu) {
    RenderSectorExitMenu();
  }
  RenderButtons();

  SaveBackgroundRects();
  RenderButtonsFastHelp();
  RenderPausedGameBox();

  ExecuteBaseDirtyRectQueue();
  EndFrameBufferRender();
}

void InitHelicopterEntranceByMercs(void) {
  if (DidGameJustStart()) {
    AIR_RAID_DEFINITION AirRaidDef;

    // Update clock ahead from STARTING_TIME to make mercs arrive!
    WarpGameTime(FIRST_ARRIVAL_DELAY, WARPTIME_PROCESS_EVENTS_NORMALLY);

    AirRaidDef.sSectorX = 9;
    AirRaidDef.sSectorY = 1;
    AirRaidDef.sSectorZ = 0;
    AirRaidDef.bIntensity = 2;
    AirRaidDef.uiFlags = AIR_RAID_BEGINNING_GAME;
    AirRaidDef.ubNumMinsFromCurrentTime = 1;

    //	ScheduleAirRaid( &AirRaidDef );

    gfTacticalDoHeliRun = TRUE;
    gfFirstHeliRun = TRUE;

    gTacticalStatus.fDidGameJustStart = FALSE;
  }
}
