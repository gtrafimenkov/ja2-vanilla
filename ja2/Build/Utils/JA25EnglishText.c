#include "Utils/UtilsAll.h"
#include "Utils/Ja25EnglishText.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "LanguageDefines.h"
#ifdef ENGLISH
#include "Utils/Text.h"
#include "SGP/FileMan.h"
#include "JA2DemoAds.h"
#endif
#endif

#ifdef ENGLISH

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

CHAR16
zNewTacticalMessages[][LARGE_STRING_LENGTH] = {
    L"Range to target: %d tiles",  /// L"Ðàññòîÿíèå äî öåëè: %d òàéë(îâ)",
    L"Attaching the transmitter to your laptop computer.", L"You cannot afford to hire %s",
    L"For a limited time, the above fee covers the cost of the entire "
    L"mission and includes the equipment listed below.",
    L"Hire %s now and take advantage of our unprecedented 'one fee covers "
    L"all' pricing.  Also included in this unbelievable offer is the "
    L"mercenary's personal equipment at no charge.",
    L"Fee", L"There is someone else in the sector...",
    L"Gun Range: %d tiles, Range to target: %d tiles",  /// L"Ýôôåêòèâíàÿ
                                                        /// äàëüíîñòü îðóæèÿ:
                                                        /// %d òàéë(îâ),
                                                        /// Ðàññòîÿíèå äî
                                                        /// öåëè: %d
                                                        /// òàéë(îâ)",
    L"Display Cover",                                   /// L"Êàðòà óêðûòèé",
    L"Line of Sight",                                   /// L"Ïîëå çðåíèÿ",
    L"New Recruits cannot arrive there.",
    L"Since your laptop has no transmitter, you won't be able to hire new "
    L"team members.  Perhaps this would be a good time to load a saved "
    L"game or start over!",
    L"%s hears the sound of crumpling metal coming from underneath Jerry's "
    L"body.  It sounds disturbingly like your laptop antenna being "
    L"crushed.",  // the %s is the name of a merc.  @@@  Modified
    L"After scanning the note left behind by Deputy Commander Morris, %s "
    L"senses an oppurtinity.  The note contains the coordinates for "
    L"launching missiles against different towns in Arulco.  It also gives "
    L"the coodinates of the origin - the missile facility.",
    L"Noticing the control panel, %s figures the numbers can be reveresed, "
    L"so that the missile might destroy this very facility.  %s needs to "
    L"find an escape route.  The elevator appears to offer the fastest "
    L"solution...",
    L"This is an IRON MAN game and you cannot save when enemies are "
    L"around.",  /// L"Ýòî ðåæèì èãðû 'Æåëåçíàÿ âîëÿ'. Âû íå ìîæåòå
                 /// ñîõðàíÿòüñÿ, ïîêà âîêðóã âðàãè.",	//	@@@  new
                 /// text
    L"(Cannot save during combat)",  /// L"(Íåëüçÿ ñîõðàíÿòüñÿ âî âðåìÿ
                                     /// áîÿ)", //@@@@ new text
    L"The current campaign name is greater than 30 characters.",  // @@@ new
                                                                  // text
    L"The current campaign cannot be found.",                     // @@@ new text
    L"Campaign: Default ( %S )",                                  // @@@ new text
    L"Campaign: %S",                                              // @@@ new text
    L"You have selected the campaign %S. This campaign is a "
    L"player-modified version of the original Unfinished Business "
    L"campaign. Are you sure you wish to play the %S campaign?",  // @@@ new
                                                                  // text
    L"In order to use the editor, please select a campaign other than the "
    L"default.",  ///@@new
};

//@@@:  New string as of March 3, 2000.
CHAR16 gzIronManModeWarningText[][LARGE_STRING_LENGTH] = {
    L"You have chosen IRON MAN mode. This setting makes the game considerably more challenging as "
    L"you will not be able to save your game when in a sector occupied by enemies. This setting "
    L"will affect the entire course of the game.  Are you sure want to play in IRON MAN mode?",
};

#endif
