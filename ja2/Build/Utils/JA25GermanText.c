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

#ifdef GERMAN

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

STR16 zNewTacticalMessages[] = {
    L"Entfernung zum Ziel: %d Felder",
    L"Verbinden Sie den Transmitter mit Ihrem Laptop-Computer.",
    L"Sie haben nicht genug Geld, um %s anzuheuern",
    L"Das obenstehende Honorar deckt für einen begrenzten Zeitraum die Kosten der Gesamtmission, "
    L"und schließt untenstehendes Equipment mit ein.",
    L"Engagieren Sie %s jetzt und nutzen Sie den Vorteil unseres beispiellosen 'Ein Betrag für "
    L"alles'-Honorars. Das persönliche Equipment des Söldners ist gratis in diesem Preis mit "
    L"inbegriffen.",
    L"Honorar",
    L"Da ist noch jemand im Sektor...",
    L"Waffen-Rchwt.: %d Felder, Entf. zum Ziel: %d Felder",
    L"Deckung anzeigen",
    L"Sichtfeld",
    L"Neue Rekruten können dort nicht hinkommen.",
    L"Da Ihr Laptop keinen Transmitter besitzt, können Sie keine neuen Teammitglieder anheuern. "
    L"Vielleicht ist dies eine guter Zeitpunkt, ein gespeichertes  Spiel zu laden oder ein neues "
    L"zu starten!",
    L"%s hört das Geräusch knirschenden Metalls unter Jerry hervordringen. Es klingt grässlich - "
    L"die Antenne ihres Laptop-Computers ist  zerstört.",  // the %s is the name of a merc.  @@@
                                                           // Modified
    L"Nach Ansehen des Hinweises, den Commander Morris hinterließ, erkennt %s eine einmalige "
    L"Gelegenheit. Der Hinweis enthält Koordinaten für den Start von Raketen gegen verschiedene "
    L"Städte in Arulco. Aber er enthält auch die Koordinaten des Startpunktes - der Raketenanlage.",
    L"Das Kontroll-Board studierend, entdeckt %s, dass die Zahlen umgedreht werden könnten, so "
    L"dass die Raketen diese Anlage selbst zerstören. %s muss nun einen Fluchtweg finden. Der "
    L"Aufzug scheint die schnellstmögliche Route zu bieten...",  //!!! The original reads:
                                                                 //! L"Noticing the control panel
                                                                 //! %s, figures the numbers can be
                                                                 //! reversed..." That sounds odd
                                                                 //! for me, but I think the comma
                                                                 //! is placed one word too late...
                                                                 //!(correct?)
    L"Dies ist ein IRON MAN-Spiel, und es kann nicht gespeichert werden, wenn sich Gegner in der "
    L"Nähe befinden.",
    L"(Kann während Kampf nicht speichern)",
    L"Der Name der aktuellen Kampagne enthält mehr als 30 Buchstaben.",
    L"Die aktuelle Kampagne kann nicht gefunden werden.",
    L"Kampagne: Standard ( %S )",
    L"Kampagne: %S",
    L"Sie haben die Kampagne %S gewählt. Diese ist eine vom Spieler modifizierte Version der "
    L"Originalkampagne von JA2UB. Möchten Sie die Kampagne %S spielen?",
    L"Um den Editor zu benutzen, müssen Sie eine andere als die Standardkampgane auswählen.",
};

//@@@:  New string as of March 3, 2000.
STR16 gzIronManModeWarningText[] = {
    L"You have chosen IRON MAN mode. This setting makes the game considerably more challenging as "
    L"you will not be able to save your game when in a sector occupied by enemies. This setting "
    L"will affect the entire course of the game.  Are you sure want to play in IRON MAN mode?",
};

#endif
