#ifndef _CHEATS__H_
#define _CHEATS__H_

#include "LanguageDefines.h"

extern UINT8 gubCheatLevel;

//***22.05.2009***
extern BOOLEAN gfCheats;

// GLOBALS FOR CHEAT MODE......
#ifdef GERMAN
#ifdef JA2TESTVERSION
#define STARTING_CHEAT_LEVEL 7
#elif defined JA2BETAVERSION
#define STARTING_CHEAT_LEVEL 5
#else
#define STARTING_CHEAT_LEVEL 0
#endif
#else
#ifdef JA2TESTVERSION
#define STARTING_CHEAT_LEVEL 6
#elif defined JA2BETAVERSION
#define STARTING_CHEAT_LEVEL 3
#else
#define STARTING_CHEAT_LEVEL 0
#endif
#endif

#ifdef GERMAN

// ATE: remove cheats unless we're doing a debug build
//#ifdef JA2TESTVERSION
#define INFORMATION_CHEAT_LEVEL() (gubCheatLevel >= 5)
#define CHEATER_CHEAT_LEVEL() (gubCheatLevel >= 6)
#define DEBUG_CHEAT_LEVEL() (gubCheatLevel >= 7)
//#else
//	#define						INFORMATION_CHEAT_LEVEL( )
//( FALSE ) 	#define						CHEATER_CHEAT_LEVEL( ) ( FALSE )
//	#define						DEBUG_CHEAT_LEVEL( )
//( FALSE ) #endif

#define RESET_CHEAT_LEVEL() (gubCheatLevel = 0)

#else

// ATE: remove cheats unless we're doing a debug build
//#ifdef JA2TESTVERSION
#define INFORMATION_CHEAT_LEVEL() (gubCheatLevel >= 3 && gfCheats)
#define CHEATER_CHEAT_LEVEL() (gubCheatLevel >= 5 && gfCheats)
#define DEBUG_CHEAT_LEVEL() (gubCheatLevel >= 6 && gfCheats)
//#else
//	#define						INFORMATION_CHEAT_LEVEL( )
//( FALSE ) 	#define						CHEATER_CHEAT_LEVEL( ) ( FALSE )
//	#define						DEBUG_CHEAT_LEVEL( )
//( FALSE ) #endif

#define RESET_CHEAT_LEVEL() (gubCheatLevel = 0)
#endif

#endif
