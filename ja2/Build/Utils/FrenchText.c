#include "Utils/UtilsAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "LanguageDefines.h"
#ifdef FRENCH
#include "Utils/Text.h"
#include "SGP/FileMan.h"
#include "JA2DemoAds.h"
#endif
#endif

#ifdef FRENCH

/*

******************************************************************************************************
**                                  IMPORTANT TRANSLATION NOTES **
******************************************************************************************************

GENERAL INSTRUCTIONS
- Always be aware that foreign strings should be of equal or shorter length than the English
equivalent. I know that this is difficult to do on many occasions due to the nature of foreign
languages when compared to English.  By doing so, this will greatly reduce the amount of work on
both sides.  In most cases (but not all), JA2 interfaces were designed with just enough space to fit
the English word. The general rule is if the string is very short (less than 10 characters), then
it's short because of interface limitations.  On the other hand, full sentences commonly have little
limitations for length. Strings in between are a little dicey.
- Never translate a string to appear on multiple lines.  All strings L"This is a really long
string...", must fit on a single line non matter how long the string is.  All strings start with L"
and end with ",
- Never remove any extra spaces in strings.  In addition, all strings containing multiple sentences
only have one space after a period, which is different than standard typing convention.  Never
modify sections of strings contain combinations of % characters.  These are special format
characters and are always used in conjunction with other characters.  For example, %s means string,
and is commonly used for names, locations, items, etc.  %d is used for numbers.  %c%d is a character
and a number (such as A9).
        %% is how a single % character is built.  There are countless types, but strings containing
these special characters are usually commented to explain what they mean.  If it isn't commented,
then if you can't figure out the context, then feel free to ask SirTech.
- Comments are always started with // Anything following these two characters on the same line are
        considered to be comments.  Do not translate comments.  Comments are always applied to the
following string(s) on the next line(s), unless the comment is on the same line as a string.
- All new comments made by SirTech will use "//@@@ comment" (without the quotes) notation.  By
searching for @@@ everytime you recieve a new version, it will simplify your task and identify
special instructions. Commonly, these types of comments will be used to ask you to abbreviate a
string.  Please leave the comments intact, and SirTech will remove them once the translation for
that particular area is resolved.
- If you have a problem or question with translating certain strings, please use "//!!! comment"
        (without the quotes).  The syntax is important, and should be identical to the comments used
with @@@ symbols.  SirTech will search for !!! to look for your problems and questions.  This is a
more efficient method than detailing questions in email, so try to do this whenever possible.



FAST HELP TEXT -- Explains how the syntax of fast help text works.
**************

1) BOLDED LETTERS
        The popup help text system supports special characters to specify the hot key(s) for a
button. Anytime you see a '|' symbol within the help text string, that means the following key is
assigned to activate the action which is usually a button.

        EX:  L"|Map Screen"

        This means the 'M' is the hotkey.  In the game, when somebody hits the 'M' key, it activates
that button.  When translating the text to another language, it is best to attempt to choose a word
that uses 'M'.  If you can't always find a match, then the best thing to do is append the 'M' at the
end of the string in this format:

        EX:  L"Ecran De Carte (|M)"  (this is the French translation)

        Other examples are used multiple times, like the Esc key  or "|E|s|c" or Space ->
(|S|p|a|c|e)

2) NEWLINE
  Any place you see a \n within the string, you are looking at another string that is part of the
fast help text system.  \n notation doesn't need to be precisely placed within that string, but
whereever you wish to start a new line.

        EX:  L"Clears all the mercs' positions,\nand allows you to re-enter them manually."

        Would appear as:

                                Clears all the mercs' positions,
                                and allows you to re-enter them manually.

        NOTE:  It is important that you don't pad the characters adjacent to the \n with spaces.  If
we did this in the above example, we would see

        WRONG WAY -- spaces before and after the \n
        EX:  L"Clears all the mercs' positions, \n and allows you to re-enter them manually."

        Would appear as: (the second line is moved in a character)

                                Clears all the mercs' positions,
                                 and allows you to re-enter them manually.


@@@ NOTATION
************

        Throughout the text files, you'll find an assortment of comments.  Comments are used to
describe the text to make translation easier, but comments don't need to be translated.  A good
thing is to search for
        "@@@" after receiving new version of the text file, and address the special notes in this
manner.

!!! NOTATION
************

        As described above, the "!!!" notation should be used by you to ask questions and address
problems as SirTech uses the "@@@" notation.

*/

UINT16 ItemNames[MAXITEMS][80] = {
    L"",
};

UINT16 ShortItemNames[MAXITEMS][80] = {
    L"",
};

// Different weapon calibres
// CAWS is Close Assault Weapon System and should probably be left as it is
// NATO is the North Atlantic Treaty Organization
// WP is Warsaw Pact
// cal is an abbreviation for calibre
UINT16 AmmoCaliber[][20] = {
    L"0",         L"cal .38", L"9mm",    L"cal .45", L"cal .357",
    L"cal 12",    L"CAWS",    L"5.45mm", L"5.56mm",  L"7.62mm OTAN",
    L"7.62mm PV", L"4.7mm",   L"5.7mm",  L"Monster", L"Roquette",
    L"",  // dart
    L"",  // flame
};

// This BobbyRayAmmoCaliber is virtually the same as AmmoCaliber however the bobby version doesnt
// have as much room for the words.
//
// Different weapon calibres
// CAWS is Close Assault Weapon System and should probably be left as it is
// NATO is the North Atlantic Treaty Organization
// WP is Warsaw Pact
// cal is an abbreviation for calibre
UINT16 BobbyRayAmmoCaliber[][20] = {
    L"0",         L"cal .38", L"9mm",    L"cal .45", L"cal .357",
    L"cal 12",    L"CAWS",    L"5.45mm", L"5.56mm",  L"7.62mm O.",
    L"7.62mm PV", L"4.7mm",   L"5.7mm",  L"Monster", L"Roquette",
    L"",  // dart
};

UINT16 WeaponType[][30] = {
    L"Divers",        L"Pistolet",           L"Pistolet-mitrailleur", L"Mitraillette",
    L"Fusil",         L"Fusil de précision", L"Fusil d'assaut",       L"Mitrailleuse légère",
    L"Fusil à pompe",
};

UINT16 TeamTurnString[][STRING_LENGTH] = {
    L"Tour du joueur",  // player's turn
    L"Tour de l'adversaire", L"Tour des créatures", L"Tour de la milice", L"Tour des civils",
    // planning turn
};

UINT16 Message[][STRING_LENGTH] = {
    L"",

    // In the following 8 strings, the %s is the merc's name, and the %d (if any) is a number.

    L"%s est touché à la tête et perd un point de sagesse !",
    L"%s est touché à l'épaule et perd un point de dextérité !",
    L"%s est touché à la poitrine et perd un point de force !",
    L"%s est touché à la jambe et perd un point d'agilité !",
    L"%s est touché à la tête et perd %d points de sagesse !",
    L"%s est touché à l'épaule et perd %d points de dextérité !",
    L"%s est touché à la poitrine et perd %d points de force !",
    L"%s est touché à la jambe et perd %d points d'agilité !",
    L"Interruption !",

    // The first %s is a merc's name, the second is a string from pNoiseVolStr,
    // the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

    L"",  // OBSOLETE
    L"Les renforts sont arrivés !",

    // In the following four lines, all %s's are merc names

    L"%s recharge.",
    L"%s n'a pas assez de Points d'Action !",
    L"%s applique les premiers soins (Appuyez sur une touche pour annuler).",
    L"%s et %s appliquent les premiers soins (Appuyez sur une touche pour annuler).",
    // the following 17 strings are used to create lists of gun advantages and disadvantages
    // (separated by commas)
    L"fiable",
    L"peu fiable",
    L"facile à entretenir",
    L"difficile à entretenir",
    L"puissant",
    L"peu puissant",
    L"cadence de tir élevée",
    L"faible cadence de tir",
    L"longue portée",
    L"courte portée",
    L"léger",
    L"encombrant",
    L"petit",
    L"tir en rafale",
    L"pas de tir en rafale",
    L"grand chargeur",
    L"petit chargeur",

    // In the following two lines, all %s's are merc names

    L"Le camouflage de %s s'est effacé.",
    L"Le camouflage de %s est parti.",

    // The first %s is a merc name and the second %s is an item name

    L"La deuxième arme est vide !",
    L"%s a volé le/la %s.",

    // The %s is a merc name

    L"L'arme de %s ne peut pas tirer en rafale.",

    L"Vous avez déjà ajouté cet accessoire.",
    L"Combiner les objets ?",

    // Both %s's are item names

    L"Vous ne pouvez combiner un(e) %s avec un(e) %s.",

    L"Aucun",
    L"Ejecter chargeur",
    L"Accessoire",

    // You cannot use "item(s)" and your "other item" at the same time.
    // Ex:  You cannot use sun goggles and you gas mask at the same time.
    L"Vous ne pouvez utiliser votre %s et votre %s simultanément.",

    L"Vous pouvez combiner cet accessoire avec certains objets en le mettant dans l'un des quatre "
    L"emplacements disponibles.",
    L"Vous pouvez combiner cet accessoire avec certains objets en le mettant dans l'un des quatre "
    L"emplacements disponibles (Ici, cet accessoire n'est pas compatible avec cet objet).",
    L"Ce secteur n'a pas été sécurisé !",
    L"Vous devez donner %s à %s",  // inverted !! you still need to give the letter to X
    L"%s a été touché à la tête !",
    L"Rompre le combat ?",
    L"Cet accessoire ne pourra plus être enlevé. Désirez-vous toujours le mettre ?",
    L"%s se sent beaucoup mieux !",
    L"%s a glissé sur des billes !",
    L"%s n'est pas parvenu à ramasser le/la %s !",
    L"%s a réparé le %s",
    L"Interruption pour ",
    L"Voulez-vous vous rendre ?",
    L"Cette personne refuse votre aide.",
    L"JE NE CROIS PAS !",
    L"Pour utiliser l'hélicoptère de Skyrider, vous devez ASSIGNER vos mercenaires au VEHICULE.",
    L"%s ne peut recharger qu'UNE arme",
    L"Tour des chats sauvages",
};

// the names of the towns in the game

STR16 pTownNames[] = {
    L"",         L"Omerta", L"Drassen", L"Alma",   L"Grumm",  L"Tixa",     L"Cambria",
    L"San Mona", L"Estoni", L"Orta",    L"Balime", L"Meduna", L"Chitzena",
};

// the types of time compression. For example: is the timer paused? at normal speed, 5 minutes per
// second, etc. min is an abbreviation for minutes

STR16 sTimeStrings[] = {
    L"Pause", L"Normal", L"5 min", L"30 min", L"60 min", L"6 hrs",
};

// Assignment Strings: what assignment does the merc  have right now? For example, are they on a
// squad, training, administering medical aid (doctor) or training a town. All are abbreviated. 8
// letters is the longest it can be.

STR16 pAssignmentStrings[] = {
    L"Esc. 1",     L"Esc. 2",  L"Esc. 3",  L"Esc. 4",  L"Esc. 5",  L"Esc. 6",  L"Esc. 7",
    L"Esc. 8",     L"Esc. 9",  L"Esc. 10", L"Esc. 11", L"Esc. 12", L"Esc. 13", L"Esc. 14",
    L"Esc. 15",    L"Esc. 16", L"Esc. 17", L"Esc. 18", L"Esc. 19", L"Esc. 20",
    L"Service",     // on active duty
    L"Docteur",     // administering medical aid
    L"Patient",     // getting medical aid
    L"Transport",   // in a vehicle
    L"Transit",     // in transit - abbreviated form
    L"Réparation",  // repairing
    L"Formation",   // training themselves
    L"Milice",      // training a town to revolt
    L"Entraîneur",  // training a teammate
    L"Elève",       // being trained by someone else
    L"Mort",        // dead
    L"Incap.",      // abbreviation for incapacitated
    L"Capturé",     // Prisoner of war - captured
    L"Hôpital",     // patient in a hospital
    L"Vide",        // Vehicle is empty
};

STR16 pMilitiaString[] = {
    L"Milice",       // the title of the militia box
    L"Disponibles",  // the number of unassigned militia troops
    L"Vous ne pouvez réorganiser la milice lors d'un combat !",
};

STR16 pMilitiaButtonString[] = {
    L"Auto",  // auto place the militia troops for the player
    L"OK",    // done placing militia troops
};

STR16 pConditionStrings[] = {
    L"Excellent",     // the state of a soldier .. excellent health
    L"Bon",           // good health
    L"Satisfaisant",  // fair health
    L"Blessé",        // wounded health
    L"Fatigué",       // tired
    L"Epuisé",        // bleeding to death
    L"Inconscient",   // knocked out
    L"Mourant",       // near death
    L"Mort",          // dead
};

STR16 pEpcMenuStrings[] = {
    L"Service",    // set merc on active duty
    L"Patient",    // set as a patient to receive medical aid
    L"Transport",  // tell merc to enter vehicle
    L"Laisser",    // let the escorted character go off on their own
    L"Annuler",    // close this menu
};

// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] = {
    L"Esc. 1",     L"Esc. 2",  L"Esc. 3",    L"Esc. 4",  L"Esc. 5",     L"Esc. 6",    L"Esc. 7",
    L"Esc. 8",     L"Esc. 9",  L"Esc. 10",   L"Esc. 11", L"Esc. 12",    L"Esc. 13",   L"Esc. 14",
    L"Esc. 15",    L"Esc. 16", L"Esc. 17",   L"Esc. 18", L"Esc. 19",    L"Esc. 20",   L"Service",
    L"Docteur",    L"Patient", L"Transport", L"Transit", L"Réparation", L"Formation", L"Milice",
    L"Entraîneur", L"Elève",   L"Mort",      L"Incap.",  L"Capturé",    L"Hôpital",
    L"Vide",  // Vehicle is empty
};

// refer to above for comments

STR16 pLongAssignmentStrings[] = {
    L"Esc. 1",     L"Esc. 2",  L"Esc. 3",    L"Esc. 4",  L"Esc. 5",     L"Esc. 6",    L"Esc. 7",
    L"Esc. 8",     L"Esc. 9",  L"Esc. 10",   L"Esc. 11", L"Esc. 12",    L"Esc. 13",   L"Esc. 14",
    L"Esc. 15",    L"Esc. 16", L"Esc. 17",   L"Esc. 18", L"Esc. 19",    L"Esc. 20",   L"Service",
    L"Docteur",    L"Patient", L"Transport", L"Transit", L"Réparation", L"Formation", L"Milice",
    L"Entraîneur", L"Elève",   L"Mort",      L"Incap.",  L"Capturé",
    L"Hôpital",  // patient in a hospital
    L"Vide",     // Vehicle is empty
};

// the contract options

STR16 pContractStrings[] = {
    L"Options du contrat :",
    L"",                      // a blank line, required
    L"Extension 1 jour",      // offer merc a one day contract extension
    L"Extension 1 semaine",   // 1 week
    L"Extension 2 semaines",  // 2 week
    L"Renvoyer",              // end merc's contract
    L"Annuler",               // stop showing this menu
};

STR16 pPOWStrings[] = {
    L"Capturé",  // an acronym for Prisoner of War
    L"??",
};

STR16 pLongAttributeStrings[] = {
    L"FORCE",    L"DEXTERITE", L"AGILITE",      L"SAGESSE",   L"TIR",
    L"MEDECINE", L"TECHNIQUE", L"COMMANDEMENT", L"EXPLOSIFS", L"NIVEAU",
};

STR16 pInvPanelTitleStrings[] = {
    L"Armure",  // the armor rating of the merc
    L"Poids",   // the weight the merc is carrying
    L"Cam.",    // the merc's camouflage rating
};

STR16 pShortAttributeStrings[] = {
    L"Agi",  // the abbreviated version of : agilité
    L"Dex",  // dextérité
    L"For",  // strength
    L"Com",  // leadership
    L"Sag",  // sagesse
    L"Niv",  // experience level
    L"Tir",  // marksmanship skill
    L"Exp",  // explosive skill
    L"Tec",  // mechanical skill
    L"Méd",  // medical skill
};

STR16 pUpperLeftMapScreenStrings[] = {
    L"Affectation",  // the mercs current assignment
    L"Contrat",      // the contract info about the merc
    L"Santé",        // the health level of the current merc
    L"Moral",        // the morale of the current merc
    L"Cond.",        // the condition of the current vehicle
    L"Carb.",        // the fuel level of the current vehicle
};

STR16 pTrainingStrings[] = {
    L"Formation",   // tell merc to train self
    L"Milice",      // tell merc to train town
    L"Entraîneur",  // tell merc to act as trainer
    L"Elève",       // tell merc to be train by other
};

STR16 pGuardMenuStrings[] = {
    L"Cadence de tir :",       // the allowable rate of fire for a merc who is guarding
    L" Feu à volonté",         // the merc can be aggressive in their choice of fire rates
    L" Economiser munitions",  // conserve ammo
    L" Tir restreint",         // fire only when the merc needs to
    L"Autres Options :",       // other options available to merc
    L" Retraite",              // merc can retreat
    L" Abri",                  // merc is allowed to seek cover
    L" Assistance",            // merc can assist teammates
    L"OK",                     // done with this menu
    L"Annuler",                // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by
// the player

STR16 pOtherGuardMenuStrings[] = {
    L"Cadence de tir :", L" *Feu à volonté*", L" *Economiser munitions*",
    L" *Tir restreint*", L"Autres Options :", L" *Retraite*",
    L" *Abri*",          L" *Assistance*",    L"OK",
    L"Annuler",
};

STR16 pAssignMenuStrings[] = {
    L"Service",     // merc is on active duty
    L"Docteur",     // the merc is acting as a doctor
    L"Patient",     // the merc is receiving medical attention
    L"Transport",   // the merc is in a vehicle
    L"Réparation",  // the merc is repairing items
    L"Formation",   // the merc is training
    L"Annuler",     // cancel this menu
};

STR16 pRemoveMercStrings[] = {
    L"Enlever Merc",  // remove dead merc from current team
    L"Annuler",
};

STR16 pAttributeMenuStrings[] = {
    L"Force",    L"Dextérité", L"Agilité",      L"Santé",     L"Tir",
    L"Médecine", L"Technique", L"Commandement", L"Explosifs", L"Annuler",
};

STR16 pTrainingMenuStrings[] = {
    L"Formation",   // train yourself
    L"Milice",      // train the town
    L"Entraîneur",  // train your teammates
    L"Elève",       // be trained by an instructor
    L"Annuler",     // cancel this menu
};

STR16 pSquadMenuStrings[] = {
    L"Esc. 1",  L"Esc. 2",  L"Esc. 3",  L"Esc. 4",  L"Esc. 5",  L"Esc. 6",  L"Esc. 7",
    L"Esc. 8",  L"Esc. 9",  L"Esc. 10", L"Esc. 11", L"Esc. 12", L"Esc. 13", L"Esc. 14",
    L"Esc. 15", L"Esc. 16", L"Esc. 17", L"Esc. 18", L"Esc. 19", L"Esc. 20", L"Annuler",
};

STR16 pPersonnelTitle[] = {
    L"Personnel",  // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] = {
    L"Santé : ",  // health of merc
    L"Agilité : ",
    L"Dextérité : ",
    L"Force : ",
    L"Commandement : ",
    L"Sagesse : ",
    L"Niv. Exp. : ",  // experience level
    L"Tir : ",
    L"Technique : ",
    L"Explosifs : ",
    L"Médecine : ",
    L"Acompte méd. : ",    // amount of medical deposit put down on the merc
    L"Contrat : ",         // cost of current contract
    L"Tués : ",            // number of kills by merc
    L"Participation : ",   // number of assists on kills by merc
    L"Coût/jour :",        // daily cost of merc
    L"Coût total :",       // total cost of merc
    L"Contrat :",          // cost of current contract
    L"Services rendus :",  // total service rendered by merc
    L"Salaires dus :",     // amount left on MERC merc to be paid
    L"Précision :",        // percentage of shots that hit target
    L"Combats :",          // number of battles fought
    L"Blessures :",        // number of times merc has been wounded
    L"Spécialités :",
    L"Aucune spécialité",
};

// These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = {
    L"Aucune spécialité",
    L"Crochetage",
    L"Combat à mains nues",
    L"Electronique",
    L"Opérations de nuit",
    L"Lancer",
    L"Enseigner",
    L"Armes lourdes",
    L"Armes automatiques",
    L"Furtivité",
    L"Ambidextre",
    L"Voleur",
    L"Arts martiaux",
    L"Couteau",
    L"Bonus toucher (sur le toit)",
    L"Camouflage",
    L"(Expert)",
};

// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] = {
    L"|Debout/Marcher",
    L"|Accroupi/Avancer",
    L"Debout/|Courir",
    L"|A terre/Ramper",
    L"|Regarder",
    L"Action",
    L"Parler",
    L"Examiner (|C|t|r|l)",

    // Pop up door menu
    L"Ouvrir à la main",
    L"Examen poussé",
    L"Crocheter",
    L"Enfoncer",
    L"Désamorcer",
    L"Verrouiller",
    L"Déverrouiller",
    L"Utiliser explosif",
    L"Utiliser pied de biche",
    L"Annuler (|E|c|h|a|p)",
    L"Fermer",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] = {
    L"aucun piège",       L"un piège explosif",      L"un piège électrique",
    L"une alarme sonore", L"une alarme silencieuse",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] = {
    L"jour",
    L"semaine",
    L"2 semaines",
};

// On the map screen, there are four columns. This text is popup help text that identifies the
// individual columns.

STR16 pMapScreenMouseRegionHelpText[] = {
    L"Choix personnage", L"Affectation",  L"Destination",
    L"Merc |Contrat",    L"Enlever Merc", L"Repos",
};

// volumes of noises

STR16 pNoiseVolStr[] = {
    L"FAIBLE",
    L"MOYEN",
    L"FORT",
    L"TRES FORT",
};

// types of noises

STR16 pNoiseTypeStr[] =  // OBSOLETE
    {
        L"INCONNU",   L"MOUVEMENT", L"GRINCEMENT", L"CLAPOTEMENT", L"IMPACT", L"COUP DE FEU",
        L"EXPLOSION", L"CRI",       L"IMPACT",     L"IMPACT",      L"BRUIT",  L"COLLISION",
};

// Directions that are used to report noises

STR16 pDirectionStr[] = {
    L"au NORD-EST",  L"à l'EST",   L"au SUD-EST",    L"au SUD",
    L"au SUD-OUEST", L"à l'OUEST", L"au NORD-OUEST", L"au NORD",
};

// These are the different terrain types.

STR16 pLandTypeStrings[] = {
    L"Ville", L"Route", L"Plaine", L"Désert", L"Bois", L"Forêt", L"Marais", L"Eau", L"Collines",
    L"Infranchissable",
    L"Rivière",  // river from north to south
    L"Rivière",  // river from east to west
    L"Pays étranger",
    // NONE of the following are used for directional travel, just for the sector description.
    L"Tropical", L"Cultures", L"Plaines, route", L"Bois, route", L"Ferme, route",
    L"Tropical, route", L"Forêt, route", L"Route côtière", L"Montagne, route", L"Côte, route",
    L"Désert, route", L"Marais, route", L"Bois, site SAM", L"Désert, site SAM",
    L"Tropical, site SAM", L"Meduna, site SAM",

    // These are descriptions for special sectors
    L"Hôpital de Cambria", L"Aéroport de Drassen", L"Aéroport de Meduna", L"Site SAM",
    L"Base rebelle",          // The rebel base underground in sector A10
    L"Prison de Tixa",        // The basement of the Tixa Prison (J9)
    L"Repaire de créatures",  // Any mine sector with creatures in it
    L"Sous-sols d'Orta",      // The basement of Orta (K4)
    L"Tunnel",                // The tunnel access from the maze garden in Meduna
                              // leading to the secret shelter underneath the palace
    L"Abri",                  // The shelter underneath the queen's palace
    L"",                      // Unused
};

STR16 gpStrategicString[] = {
    L"",                                                                     // Unused
    L"%s détecté dans le secteur %c%d et une autre escouade est en route.",  // STR_DETECTED_SINGULAR
    L"%s détecté dans le secteur %c%d et d'autres escouades sont en route.",  // STR_DETECTED_PLURAL
    L"Voulez-vous coordonner vos mouvements de troupe ?",                     // STR_COORDINATE

    // Dialog strings for enemies.

    L"L'ennemi vous propose de vous rendre.",             // STR_ENEMY_SURRENDER_OFFER
    L"L'ennemi a capturé vos mercenaires inconscients.",  // STR_ENEMY_CAPTURED

    // The text that goes on the autoresolve buttons

    L"Retraite",  // The retreat button				//STR_AR_RETREAT_BUTTON
    L"OK",        // The done button				//STR_AR_DONE_BUTTON

    // The headers are for the autoresolve type (MUST BE UPPERCASE)

    L"DEFENSEURS",  // STR_AR_DEFEND_HEADER
    L"ATTAQUANTS",  // STR_AR_ATTACK_HEADER
    L"RENCONTRE",   // STR_AR_ENCOUNTER_HEADER
    L"Secteur",     // The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

    // The battle ending conditions

    L"VICTOIRE !",   // STR_AR_OVER_VICTORY
    L"DEFAITE !",    // STR_AR_OVER_DEFEAT
    L"REDDITION !",  // STR_AR_OVER_SURRENDERED
    L"CAPTURE !",    // STR_AR_OVER_CAPTURED
    L"RETRAITE !",   // STR_AR_OVER_RETREATED

    // These are the labels for the different types of enemies we fight in autoresolve.

    L"Mort",      // STR_AR_MILITIA_NAME,
    L"Mort",      // STR_AR_ELITE_NAME,
    L"Mort",      // STR_AR_TROOP_NAME,
    L"Admin",     // STR_AR_ADMINISTRATOR_NAME,
    L"Créature",  // STR_AR_CREATURE_NAME,

    // Label for the length of time the battle took

    L"Temps écoulé",  // STR_AR_TIME_ELAPSED,

    // Labels for status of merc if retreating.  (UPPERCASE)

    L"SE RETIRE",    // STR_AR_MERC_RETREATED,
    L"EN RETRAITE",  // STR_AR_MERC_RETREATING,
    L"RETRAITE",     // STR_AR_MERC_RETREAT,

    // PRE BATTLE INTERFACE STRINGS
    // Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
    // a system that automatically resolves the combat for the player without having to do anything.
    // These strings must be short (two lines -- 6-8 chars per line)

    L"Auto.",     // STR_PB_AUTORESOLVE_BTN,
    L"Combat",    // STR_PB_GOTOSECTOR_BTN,
    L"Retraite",  // STR_PB_RETREATMERCS_BTN,

    // The different headers(titles) for the prebattle interface.
    L"ENNEMI REPERE",                                   // STR_PB_ENEMYENCOUNTER_HEADER,
    L"ATTAQUE ENNEMIE",                                 // STR_PB_ENEMYINVASION_HEADER, // 30
    L"EMBUSCADE !",                                     // STR_PB_ENEMYAMBUSH_HEADER
    L"VOUS PENETREZ EN SECTEUR ENNEMI",                 // STR_PB_ENTERINGENEMYSECTOR_HEADER
    L"ATTAQUE DE CREATURES",                            // STR_PB_CREATUREATTACK_HEADER
    L"ATTAQUE DE CHATS SAUVAGES",                       // STR_PB_BLOODCATAMBUSH_HEADER
    L"VOUS ENTREZ DANS LE REPAIRE DES CHATS SAUVAGES",  // STR_PB_ENTERINGBLOODCATLAIR_HEADER

    // Various single words for direct translation.  The Civilians represent the civilian
    // militia occupying the sector being attacked.  Limited to 9-10 chars

    L"Lieu",
    L"Ennemis",
    L"Mercs",
    L"Milice",
    L"Créatures",
    L"Chats",
    L"Secteur",
    L"Aucun",  // If there are non uninvolved mercs in this fight.
    L"N/A",    // Acronym of Not Applicable
    L"j",      // One letter abbreviation of day
    L"h",      // One letter abbreviation of hour

    // TACTICAL PLACEMENT USER INTERFACE STRINGS
    // The four buttons

    L"Annuler",
    L"Dispersé",
    L"Groupé",
    L"OK",

    // The help text for the four buttons.  Use \n to denote new line (just like enter).

    L"|Annule le déploiement des mercenaires \net vous permet de les déployer vous-même.",
    L"Disperse |aléatoirement vos mercenaires \nà chaque fois.",
    L"Vous permet de placer votre groupe |de mercenaires.",
    L"Cliquez sur ce bouton lorsque vous avez déployé \nvos mercenaires. (|E|n|t|r|é|e)",
    L"Vous devez déployer vos mercenaires \navant d'engager le combat.",

    // Various strings (translate word for word)

    L"Secteur",
    L"Définissez les points d'entrée",

    // Strings used for various popup message boxes.  Can be as long as desired.

    L"Il semblerait que l'endroit soit inaccessible...",
    L"Déployez vos mercenaires dans la zone en surbrillance.",

    // This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
    // Don't uppercase first character, or add spaces on either end.

    L"est arrivé dans le secteur",

    // These entries are for button popup help text for the prebattle interface.  All popup help
    // text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
    L"|Résolution automatique du combat\nsans charger la carte.",
    L"|Résolution automatique impossible lorsque\nvous attaquez.",
    L"|Pénétrez dans le secteur pour engager le combat.",
    L"|Faire retraite vers le secteur précédent.",     // singular version
    L"|Faire retraite vers les secteurs précédents.",  // multiple groups with same previous sector

    // various popup messages for battle conditions.

    //%c%d is the sector -- ex:  A9
    L"L'ennemi attaque votre milice dans le secteur %c%d.",
    //%c%d is the sector -- ex:  A9
    L"Les créatures attaquent votre milice dans le secteur %c%d.",
    // 1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
    // Note:  the minimum number of civilians eaten will be two.
    L"Les créatures ont tué %d civils dans le secteur %s.",
    //%s is the sector location -- ex:  A9: Omerta
    L"L'ennemi attaque vos mercenaires dans le secteur %s. Aucun de vos hommes ne peut combattre !",
    //%s is the sector location -- ex:  A9: Omerta
    L"Les créatures attaquent vos mercenaires dans le secteur %s. Aucun de vos hommes ne peut "
    L"combattre !",

};

STR16 gpGameClockString[] = {
    // This is the day represented in the game clock.  Must be very short, 4 characters max.
    L"Jour",
};

// When the merc finds a key, they can get a description of it which
// tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] = {
    L"Secteur :",
    L"Jour :",
};

// The headers used to describe various weapon statistics.

INT16 gWeaponStatsDesc[][14] = {
    L"Poids (%s):",
    L"Etat :",
    L"Munitions :",  // Number of bullets left in a magazine
    L"Por. :",       // Range
    L"Dég. :",       // Damage
    L"PA :",         // abbreviation for Action Points
    L"",
    L"=",
    L"=",
};

// The headers used for the merc's money.

INT16 gMoneyStatsDesc[][13] = {
    L"Montant",
    L"Restant :",  // this is the overall balance
    L"Montant",
    L"Partager :",  // the amount he wants to separate from the overall balance to get two piles of
                    // money

    L"Actuel",
    L"Solde",
    L"Montant à",
    L"Retirer",
};

// The health of various creatures, enemies, characters in the game. The numbers following each are
// for comment only, but represent the precentage of points remaining.

UINT16 zHealthStr[][13] = {
    L"MOURANT",       //	>= 0
    L"CRITIQUE",      //	>= 15
    L"FAIBLE",        //	>= 30
    L"BLESSE",        //	>= 45
    L"SATISFAISANT",  //	>= 60
    L"BON",           // 	>= 75
    L"EXCELLENT",     // 	>= 90
};

STR16 gzMoneyAmounts[6] = {
    L"1000", L"100", L"10", L"OK", L"Partager", L"Retirer",
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons."
INT16 gzProsLabel[10] = {
    L"Plus :",
};

INT16 gzConsLabel[10] = {
    L"Moins :",
};

// Conversation options a player has when encountering an NPC
UINT16 zTalkMenuStrings[6][SMALL_STRING_LENGTH] = {
    L"Pardon ?",  // meaning "Repeat yourself"
    L"Amical",    // approach in a friendly
    L"Direct",    // approach directly - let's get down to business
    L"Menaçant",  // approach threateningly - talk now, or I'll blow your face off
    L"Donner",   L"Recruter",
};

// Some NPCs buy, sell or repair items. These different options are available for those NPCs as
// well.
UINT16 zDealerStrings[4][SMALL_STRING_LENGTH] = {
    L"Acheter/Vendre",
    L"Acheter",
    L"Vendre",
    L"Réparer",
};

UINT16 zDialogActions[1][SMALL_STRING_LENGTH] = {
    L"OK",
};

// These are vehicles in the game.

STR16 pVehicleStrings[] = {
    L"Eldorado",
    L"Hummer",  // a hummer jeep/truck -- military vehicle
    L"Camion de glaces", L"Jeep", L"Char", L"Hélicoptère",
};

STR16 pShortVehicleStrings[] = {
    L"Eldor.",
    L"Hummer",  // the HMVV
    L"Camion", L"Jeep", L"Char",
    L"Hélico",  // the helicopter
};

STR16 zVehicleName[] = {
    L"Eldorado",
    L"Hummer",  // a military jeep. This is a brand name.
    L"Camion",  // Ice cream truck
    L"Jeep",     L"Char",
    L"Hélico",  // an abbreviation for Helicopter
};

// These are messages Used in the Tactical Screen

UINT16 TacticalStr[][MED_STRING_LENGTH] = {
    L"Raid aérien", L"Appliquer les premiers soins ?",

    // CAMFIELD NUKE THIS and add quote #66.

    L"%s a remarqué qu'il manque des objets dans cet envoi.",

    // The %s is a string from pDoorTrapStrings

    L"La serrure est piégée par %s.", L"Pas de serrure.", L"Réussite !", L"Echec.", L"Réussite !",
    L"Echec.", L"La serrure n'est pas piégée.", L"Réussite !",
    // The %s is a merc name
    L"%s ne possède pas la bonne clé.", L"Le piège est désamorcé.", L"La serrure n'est pas piégée.",
    L"Verrouillée.", L"PORTE", L"PIEGEE", L"VERROUILLEE", L"OUVERTE", L"ENFONCEE",
    L"Un interrupteur. Voulez-vous l'actionner ?", L"Désamorcer le piège ?", L"Préc...", L"Suiv...",
    L"Plus...",

    // In the next 2 strings, %s is an item name

    L"%s posé(e) à terre.", L"%s donné(e) à %s.",

    // In the next 2 strings, %s is a name

    L"%s a été payé.", L"%d dus à %s.",
    L"Choisissez la fréquence :",          // in this case, frequency refers to a radio signal
    L"Nombre de tours avant explosion :",  // how much time, in turns, until the bomb blows
    L"Définir fréquence :",                // in this case, frequency refers to a radio signal
    L"Désamorcer le piège ?", L"Enlever le drapeau bleu ?", L"Poser un drapeau bleu ?",
    L"Fin du tour",

    // In the next string, %s is a name. Stance refers to way they are standing.

    L"Voulez-vous vraiment attaquer %s ?", L"Les véhicules ne peuvent changer de position.",
    L"Le robot ne peut changer de position.",

    // In the next 3 strings, %s is a name

    L"%s ne peut adopter cette position ici.", L"%s ne peut recevoir de premiers soins ici.",
    L"%s n'a pas besoin de premiers soins.", L"Impossible d'aller ici.",
    L"Votre escouade est au complet. Vous ne pouvez pas ajouter quelqu'un.",  // there's non room
                                                                              // for a recruit on
                                                                              // the player's team

    // In the next string, %s is a name

    L"%s a été recruté(e).",

    // Here %s is a name and %d is a number

    L"Vous devez %d $ à %s.",

    // In the next string, %s is a name

    L"Escorter %s?",

    // In the next string, the first %s is a name and the second %s is an amount of money (including
    // $ sign)

    L"Engager %s à %s la journée ?",

    // This line is used repeatedly to ask player if they wish to participate in a boxing match.

    L"Voulez-vous engager le combat ?",

    // In the next string, the first %s is an item name and the
    // second %s is an amount of money (including $ sign)

    L"Acheter %s pour %s ?",

    // In the next string, %s is a name

    L"%s est escorté(e) par l'escouade %d.",

    // These messages are displayed during play to alert the player to a particular situation

    L"ENRAYE",                                      // weapon is jammed.
    L"Le robot a besoin de munitions calibre %s.",  // Robot is out of ammo
    L"Lancer ici ? Aucune chance.",  // Merc can't throw to the destination he selected

    // These are different buttons that the player can turn on and off.

    L"Furtivité (|Z)", L"|Carte", L"|OK (Fin du tour)", L"Parler à", L"Muet",
    L"Position haute (|P|g|U|p)", L"Niveau du curseur (|T|a|b)", L"Escalader / Sauter",
    L"Position basse (|P|g|D|n)", L"Examiner (|C|t|r|l)", L"Mercenaire précédent",
    L"Mercenaire suivant (E|s|p|a|c|e)", L"|Options", L"|Rafale", L"|Regarder/Pivoter",
    L"Santé : %d/%d\nEnergie : %d/%d\nMoral : %s",
    L"Pardon ?",  // this means "what?"
    L"Suite",     // an abbrieviation for "Continued"
    L"Sourdine désactivée pour %s.", L"Sourdine activée pour %s.",
    L"Etat : %d/%d\nCarburant : %d/%d", L"Sortir du véhicule",
    L"Changer d'escouade ( |M|a|j| |E|s|p|a|c|e )", L"Conduire",
    L"N/A",  // this is an acronym for "Not Applicable."
    L"Utiliser (Mains nues)", L"Utiliser (Arme à feu)", L"Utiliser (Couteau)",
    L"Utiliser (Explosifs)", L"Utiliser (Trousse de soins)", L"(Prendre)", L"(Recharger)",
    L"(Donner)", L"%s part.", L"%s arrive.", L"%s n'a plus de Points d'Action.",
    L"%s n'est pas disponible.", L"%s est couvert de bandages.", L"%s n'a plus de bandages.",
    L"Ennemi dans le secteur !", L"Pas d'ennemi en vue.", L"Pas assez de Points d'Action.",
    L"Télécommande inutilisée.", L"La rafale a vidé le chargeur !", L"SOLDAT", L"CREPITUS",
    L"Milice", L"CIVIL", L"Quitter Secteur", L"OK", L"Annuler", L"Mercenaire", L"Tous", L"GO",
    L"Carte", L"Vous ne pouvez pas quitter ce secteur par ce côté.", L"%s est trop loin.",
    L"Effacer cime des arbres", L"Afficher cime des arbres",
    L"CORBEAU",  // Crow, as in the large black bird
    L"COU", L"TETE", L"TORSE", L"JAMBES", L"Donner informations à la Reine ?",
    L"Acquisition de l'ID digitale", L"ID digitale refusée. Arme désactivée.", L"Cible acquise",
    L"Chemin bloqué",
    L"Dépôt/Retrait",  // Help text over the $ button on the Single Merc Panel
    L"Personne n'a besoin de premiers soins.",
    L"Enr.",                     // Short form of JAMMED, for small inv slots
    L"Impossible d'aller ici.",  // used ( now ) for when we click on a cliff
    L"Chemin bloqué. Voulez-vous changer de place avec cette personne ?",
    L"La personne refuse de bouger.",
    // In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
    L"Etes-vous d'accord pour payer %s ?", L"Acceptez-vous le traitement médical gratuit ?",
    L"Voulez-vous épouser Daryl ?", L"Trousseau de Clés",
    L"Vous ne pouvez pas faire ça avec ce personnage.", L"Epargner Krott ?", L"Hors de portée",
    L"Mineur", L"Un véhicule ne peut rouler qu'entre des secteurs",
    L"Impossible d'apposer des bandages maintenant", L"Chemin bloqué pour %s",
    L"Vos mercenaires capturés par l'armée de Deidranna sont emprisonnés ici !", L"Verrou touché",
    L"Verrou détruit", L"Quelqu'un d'autre veut essayer sur cette porte.",
    L"Etat : %d/%d\nCarburant : %d/%d",
    L"%s ne peut pas voir %s.",  // Cannot see person trying to talk to
};

// Varying helptext explains (for the "Go to Sector/Map" checkbox) what will happen given different
// circumstances in the "exiting sector" interface.
STR16 pExitingSectorHelpText[] = {
    // Helptext for the "Go to Sector" checkbox button, that explains what will happen when the box
    // is checked.
    L"Si vous cochez ce bouton, le secteur adjacent sera immédiatement chargé.",
    L"Si vous cochez ce bouton, vous arriverez directement dans l'écran de carte\nle temps que vos "
    L"mercenaires arrivent.",

    // If you attempt to leave a sector when you have multiple squads in a hostile sector.
    L"Vous ne pouvez laisser vos mercenaires ici.\nVous devez d'abord nettoyer ce secteur.",

    // Because you only have one squad in the sector, and the "move all" option is checked, the "go
    // to sector" option is locked to on. The helptext explains why it is locked.
    L"Faites sortir vos derniers mercenaires du secteur\npour charger le secteur adjacent.",
    L"Faites sortir vos derniers mercenaires du secteur\npour aller dans l'écran de carte le temps "
    L"que vos mercenaires fassent le voyage.",

    // If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being
    // escorted.  The "single" button is disabled.
    L"%s doit être escorté(e) par vos mercenaires et ne peut quitter ce secteur tout seul.",

    // If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc
    // will be prohibited from leaving alone. There are several strings depending on the gender of
    // the merc and how many EPCs are in the squad. DO NOT USE THE NEWLINE HERE AS IT IS USED FOR
    // BOTH HELPTEXT AND SCREEN MESSAGES!
    L"%s escorte %s, il ne peut quitter ce secteur seul.",                        // male singular
    L"%s escorte %s, elle ne peut quitter ce secteur seule.",                     // female singular
    L"%s escorte plusieurs personnages, il ne peut quitter ce secteur seul.",     // male plural
    L"%s escorte plusieurs personnages, elle ne peut quitter ce secteur seule.",  // female plural

    // If one or more of your mercs in the selected squad aren't in range of the traversal area,
    // then the  "move all" option is disabled, and this helptext explains why.
    L"Tous vos mercenaires doivent être dans les environs\npour que l'escouade avance.",

    L"",  // UNUSED

    // Standard helptext for single movement.  Explains what will happen (splitting the squad)
    L"Si vous cochez ce bouton, %s voyagera seul et sera\nautomatiquement assigné à une nouvelle "
    L"escouade.",

    // Standard helptext for all movement.  Explains what will happen (moving the squad)
    L"Si vous cochez ce bouton, votre escouade\nquittera le secteur.",

    // This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC
    // selected and you attempt to tactically traverse the EPC while the escorting mercs aren't near
    // enough (or dead, dying, or unconscious), this message will appear and the "exiting sector"
    // interface will not appear.  This is just like the situation where This string is special, as
    // it is not used as helptext.  Do not use the special newline character (\n) for this string.
    L"%s est escorté par vos mercenaires et ne peut quitter ce secteur seul. Vos mercenaires "
    L"doivent être à proximité.",
};

STR16 pRepairStrings[] = {
    L"Objets",    // tell merc to repair items in inventory
    L"Site SAM",  // tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
    L"Annuler",   // cancel this menu
    L"Robot",     // repair the robot
};

// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill."

STR16 sPreStatBuildString[] = {
    L"perd",        // the merc has lost a statistic
    L"gagne",       // the merc has gained a statistic
    L"point de",    // singular
    L"points de",   // plural
    L"niveau d'",   // singular
    L"niveaux d'",  // plural
};

STR16 sStatGainStrings[] = {
    L"santé.",
    L"agilité.",
    L"dextérité.",
    L"sagesse.",
    L"compétence médicale.",
    L"compétence en explosifs.",
    L"compétence technique.",
    L"tir",
    L"expérience.",
    L"force.",
    L"commandement.",
};

STR16 pHelicopterEtaStrings[] = {
    L"Distance totale :  ",  // total distance for helicopter to travel
    L" Aller :  ",           // distance to travel to destination
    L" Retour : ",           // distance to return from destination to airport
    L"Coût : ",              // total cost of trip by helicopter
    L"AHP :  ",              // ETA is an acronym for "estimated time of arrival"
    L"L'hélicoptère n'a plus de carburant et doit se poser en terrain ennemi !",  // warning that
                                                                                  // the sector the
                                                                                  // helicopter is
                                                                                  // going to use
                                                                                  // for refueling
                                                                                  // is under enemy
                                                                                  // control ->
    L"Passagers : ",
    L"Sélectionner Skyrider ou l'aire d'atterrissage ?",
    L"Skyrider",
    L"Arrivée",
};

STR16 sMapLevelString[] = {
    L"Niveau souterrain :",  // what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] = {
    L"Loyauté",  // the loyalty rating of a town ie : Loyal 53%
};

// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] = {
    L"Impossible de donner des ordres.",
};

STR16 gsTimeStrings[] = {
    L"h",  // hours abbreviation
    L"m",  // minutes abbreviation
    L"s",  // seconds abbreviation
    L"j",  // days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] = {
    L"Aucun",        L"Hôpital", L"Industrie", L"Prison", L"Militaire", L"Aéroport",
    L"Champ de tir",  // a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] = {
    L"Inventaire",
    L"Quitter",
};

// town strings

STR16 pwTownInfoStrings[] = {
    L"Taille",             // 0 // size of the town in sectors
    L"",                   // blank line, required
    L"Contrôle",           // how much of town is controlled
    L"Aucune",             // none of this town
    L"Mine associée",      // mine associated with this town
    L"Loyauté",            // 5 // the loyalty level of this town
    L"Forces entraînées",  // the forces in the town trained by the player
    L"",
    L"Principales installations",  // main facilities in this town
    L"Niveau",                     // the training level of civilians in this town
    L"Formation",                  // 10 // state of civilian training in town
    L"Milice",                     // the state of the trained civilians in the town
};

// Mine strings

STR16 pwMineStrings[] = {
    L"Mine",  // 0
    L"Argent",
    L"Or",
    L"Production quotidienne",
    L"Production estimée",
    L"Abandonnée",  // 5
    L"Fermée",
    L"Epuisée",
    L"Production",
    L"Etat",
    L"Productivité",
    L"Type de minerai",  // 10
    L"Contrôle de la ville",
    L"Loyauté de la ville",
    //	L"Mineurs au travail",
};

// blank sector strings

STR16 pwMiscSectorStrings[] = {
    L"Forces ennemies", L"Secteur", L"# d'objets", L"Inconnu", L"Contrôlé", L"Oui", L"Non",
};

// error strings for inventory

STR16 pMapInventoryErrorString[] = {
    L"%s n'est pas assez près.",  // Merc is in sector with item but not close enough
    L"Sélection impossible.",     // MARK CARTER
    L"%s n'est pas dans le bon secteur.",
    L"En combat, vous devez prendre les objets vous-même.",
    L"En combat, vous devez abandonner les objets vous-même.",
    L"%s n'est pas dans le bon secteur.",
};

STR16 pMapInventoryStrings[] = {
    L"Lieu",    // sector these items are in
    L"Objets",  // total number of items in sector
};

// help text for the user

STR16 pMapScreenFastHelpTextList[] = {
    L"Cliquez sur la colonne Affectation pour assigner un mercenaire à une nouvelle tâche",
    L"Cliquez sur la colonne Destination pour ordonner à un mercenaire de se rendre dans un "
    L"secteur",
    L"Utilisez la compression du temps pour que le voyage du mercenaire vous paraisse moins long.",
    L"Cliquez sur un secteur pour le sélectionner. Cliquez à nouveau pour donner un ordre de "
    L"mouvement à un mercenaire ou effectuez un clic droit pour obtenir des informations sur le "
    L"secteur.",
    L"Appuyez sur 'H' pour afficher l'aide en ligne.",
    L"Test Text",
    L"Test Text",
    L"Test Text",
    L"Test Text",
    L"Cet écran ne vous est d'aucune utilité tant que vous n'êtes pas arrivé à Arulco. Une fois "
    L"votre équipe constituée, cliquez sur le bouton de compression du temps en bas à droite. Le "
    L"temps vous paraîtra moins long...",
};

// movement menu text

STR16 pMovementMenuStrings[] = {
    L"Déplacement",  // title for movement box
    L"Route",        // done with movement menu, start plotting movement
    L"Annuler",      // cancel this menu
    L"Autre",        // title for group of mercs not on squads nor in vehicles
};

STR16 pUpdateMercStrings[] = {
    L"Oups :",                     // an error has occured
    L"Expiration du contrat :",    // this pop up came up due to a merc contract ending
    L"Tâches accomplies :",        // this pop up....due to more than one merc finishing assignments
    L"Mercenaires disponibles :",  // this pop up ....due to more than one merc waking up and
                                   // returing to work
    L"Mercenaires au repos :",  // this pop up ....due to more than one merc being tired and going
                                // to sleep
    L"Contrats arrivant à échéance :",  // this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] = {
    L"Afficher |Villes",        L"Afficher |Mines",  L"Afficher |Escouades & Ennemis",
    L"Afficher |Espace aérien", L"Afficher |Objets", L"Afficher Milice & Ennemis (|Z)",
};

STR16 pMapScreenBottomFastHelp[] = {
    L"|Poste de Travail",
    L"Tactique (|E|c|h|a|p)",
    L"|Options",
    L"Compression du temps (|+)",                             // time compress more
    L"Compression du temps (|-)",                             // time compress less
    L"Message précédent (|U|p)\nPage précédente (|P|g|U|p)",  // previous message in scrollable list
    L"Message suivant (|D|o|w|n)\nPage suivante (|P|g|D|n)",  // next message in the scrollable list
    L"Interrompre/Reprendre (|S|p|a|c|e)",                    // start/stop time compression
};

STR16 pMapScreenBottomText[] = {
    L"Solde actuel",  // current balance in player bank account
};

STR16 pMercDeadString[] = {
    L"%s est mort.",
};

STR16 pDayStrings[] = {
    L"Jour",
};

// the list of email sender names

STR16 pSenderNameList[] = {
    L"Enrico",
    L"Psych Pro Inc",
    L"Help Desk",
    L"Psych Pro Inc",
    L"Speck",
    L"R.I.S.",  // 5
    L"Barry",
    L"Blood",
    L"Lynx",
    L"Grizzly",
    L"Vicki",  // 10
    L"Trevor",
    L"Grunty",
    L"Ivan",
    L"Steroid",
    L"Igor",  // 15
    L"Shadow",
    L"Red",
    L"Reaper",
    L"Fidel",
    L"Fox",  // 20
    L"Sidney",
    L"Gus",
    L"Buns",
    L"Ice",
    L"Spider",  // 25
    L"Cliff",
    L"Bull",
    L"Hitman",
    L"Buzz",
    L"Raider",  // 30
    L"Raven",
    L"Static",
    L"Len",
    L"Danny",
    L"Magic",
    L"Stephan",
    L"Scully",
    L"Malice",
    L"Dr.Q",
    L"Nails",
    L"Thor",
    L"Scope",
    L"Wolf",
    L"MD",
    L"Meltdown",
    //----------
    L"M.I.S. Assurance",
    L"Bobby Ray",
    L"Kingpin",
    L"John Kulba",
    L"A.I.M.",
};

// next/prev strings

STR16 pTraverseStrings[] = {
    L"Précédent",
    L"Suivant",
};

// new mail notify string

STR16 pNewMailStrings[] = {
    L"Vous avez des messages...",
};

// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] = {
    L"Effacer message ?",
    L"Effacer message NON CONSULTE ?",
};

// the sort header strings

STR16 pEmailHeaders[] = {
    L"De :",
    L"Sujet :",
    L"Date :",
};

// email titlebar text

STR16 pEmailTitleText[] = {
    L"Boîte aux lettres",
};

// the financial screen strings
STR16 pFinanceTitle[] = {
    L"Bookkeeper Plus",  // the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] = {
    L"Crédit :",  // credit (subtract from) to player's account
    L"Débit :",   // debit (add to) to player's account
    L"Revenus (hier) :",
    L"Dépôts (hier) :",
    L"Dépenses (hier) :",
    L"Solde (fin de journée) :",
    L"Revenus (aujourd'hui) :",
    L"Dépôts (aujourd'hui) :",
    L"Dépenses (aujourd'hui) :",
    L"Solde actuel :",
    L"Revenus (prévision) :",
    L"Solde (prévision) :",  // projected balance for player for tommorow
};

// headers to each list in financial screen

STR16 pFinanceHeaders[] = {
    L"Jour",         // the day column
    L"Crédit",       // the credits column (to ADD money to your account)
    L"Débit",        // the debits column (to SUBTRACT money from your account)
    L"Transaction",  // transaction type - see TransactionText below
    L"Solde",        // balance at this point in time
    L"Page",         // page number
    L"Jour(s)",      // the day(s) of transactions this page displays
};

STR16 pTransactionText[] = {
    L"Intérêts cumulés",  // interest the player has accumulated so far
    L"Dépôt anonyme",
    L"Commission",
    L"Engagé",            // Merc was hired
    L"Achats Bobby Ray",  // Bobby Ray is the name of an arms dealer
    L"Règlement M.E.R.C.",
    L"Acompte médical pour %s",  // medical deposit for merc
    L"Analyse IMP",              // IMP is the acronym for International Mercenary Profiling
    L"Assurance pour %s",
    L"Réduction d'assurance pour %s",
    L"Extension d'assurance pour %s",  // johnny contract extended
    L"Annulation d'assurance pour %s",
    L"Indemnisation pour %s",  // insurance claim for merc
    L"1 jour",                 // merc's contract extended for a day
    L"1 semaine",              // merc's contract extended for a week
    L"2 semaines",             // ... for 2 weeks
    L"Revenus des mines",
    L"",  // String nuked
    L"Achat de fleurs",
    L"Remboursement médical pour %s",
    L"Remb. médical partiel pour %s",
    L"Pas de remb. médical pour %s",
    L"Paiement à %s",               // %s is the name of the npc being paid
    L"Transfert de fonds pour %s",  // transfer funds to a merc
    L"Transfert de fonds de %s",    // transfer funds from a merc
    L"Coût milice de %s",           // initial cost to equip a town's militia
    L"Achats à %s.",  // is used for the Shop keeper interface.  The dealers name will be appended
                      // to the en d of the string.
    L"Montant déposé par %s.",
};

STR16 pTransactionAlternateText[] = {
    L"Assurance pour",                // insurance for a merc
    L"Ext. contrat de %s (1 jour).",  // entend mercs contract by a day
    L"Ext. contrat de %s (1 semaine).",
    L"Ext. contrat de %s (2 semaines).",
};

// helicopter pilot payment

STR16 pSkyriderText[] = {
    L"Skyrider a reçu %d $",               // skyrider was paid an amount of money
    L"Skyrider attend toujours ses %d $",  // skyrider is still owed an amount of money
    L"Skyrider a fait le plein",           // skyrider has finished refueling
    L"",                                   // unused
    L"",                                   // unused
    L"Skyrider est prêt à redécoller.",    // Skyrider was grounded but has been freed
    L"Skyrider n'a pas de passagers. Si vous voulez envoyer des mercenaires dans ce secteur, "
    L"n'oubliez pas de les assigner à l'hélicoptère.",
};

// strings for different levels of merc morale

STR16 pMoralStrings[] = {
    L"Superbe", L"Bon", L"Stable", L"Bas", L"Paniqué", L"Mauvais",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] = {
    L"L'équipement de %s est maintenant disponible à Omerta (A9).",
    L"L'équipement de %s est maintenant disponible à Drassen (B13).",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] = {
    L"Santé",     L"Energie", L"Moral",
    L"Etat",       // the condition of the current vehicle (its "Santé")
    L"Carburant",  // the fuel level of the current vehicle (its "energy")
};

STR16 pMapScreenPrevNextCharButtonHelpText[] = {
    L"Mercenaire précédent (|G|a|u|c|h|e)",  // previous merc in the list
    L"Mercenaire suivant (|D|r|o|i|t|e)",    // next merc in the list
};

STR16 pEtaString[] = {
    L"HPA :",  // eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] = {
    L"Vous ne le reverrez jamais. Vous êtes sûr de vous ?",  // do you want to continue and lose the
                                                             // item forever
    L"Cet objet a l'air VRAIMENT important. Vous êtes bien sûr (mais alors BIEN SUR) de vouloir "
    L"l'abandonner ?",  // does the user REALLY want to trash this item
};

STR16 pMapErrorString[] = {
    L"L'escouade ne peut se déplacer si l'un de ses membres se repose.",

    // 1-5
    L"Déplacez d'abord votre escouade.",
    L"Des ordres de mouvement ? C'est un secteur hostile !",
    L"Les mercenaires doivent d'abord être assignés à un véhicule.",
    L"Vous n'avez plus aucun membre dans votre escouade.",  // you have non members, can't do
                                                            // anything
    L"Le mercenaire ne peut obéir.",                        // merc can't comply with your order
                                                            // 6-10
    L"doit être escorté. Mettez-le dans une escouade.",  // merc can't move unescorted .. for a male
    L"doit être escortée. Mettez-la dans une escouade.",  // for a female
    L"Ce mercenaire n'est pas encore arrivé !",
    L"Il faudrait d'abord revoir les termes du contrat...",
    L"",
    // 11-15
    L"Des ordres de mouvement ? Vous êtes en plein combat !",
    L"Vous êtes tombé dans une embuscade de chats sauvages dans le secteur %s !",
    L"Vous venez d'entrer dans le repaire des chats sauvages (secteur I16) !",
    L"",
    L"Le site SAM en %s est sous contrôle ennemi.",
    // 16-20
    L"La mine en %s est sous contrôle ennemi. Votre revenu journalier est réduit de %s.",
    L"L'ennemi vient de prendre le contrôle du secteur %s.",
    L"L'un au moins de vos mercenaires ne peut effectuer cette tâche.",
    L"%s ne peut rejoindre %s (plein).",
    L"%s ne peut rejoindre %s (éloignement).",
    // 21-25
    L"La mine en %s a été reprise par les forces de Deidranna !",
    L"Les forces de Deidranna viennent d'envahir le site SAM en %s",
    L"Les forces de Deidranna viennent d'envahir %s",
    L"Les forces de Deidranna ont été aperçues en %s.",
    L"Les forces de Deidranna viennent de prendre %s.",
    // 26-30
    L"L'un au moins de vos mercenaires ne peut se reposer.",
    L"L'un au moins de vos mercenaires ne peut être réveillé.",
    L"La milice n'apparaît sur l'écran qu'une fois son entraînement achevé.",
    L"%s ne peut recevoir d'ordre de mouvement pour le moment.",
    L"Les miliciens qui ne se trouvent pas dans les limites d'une ville ne peuvent être déplacés.",
    // 31-35
    L"Vous ne pouvez pas entraîner de milice en %s.",
    L"Un véhicule ne peut se déplacer s'il est vide !",
    L"L'état de santé de %s ne lui permet pas de voyager !",
    L"Vous devez d'abord quitter le musée !",
    L"%s est mort !",
    // 36-40
    L"%s ne peut passer à %s (en mouvement)",
    L"%s ne peut pas pénétrer dans le véhicule de cette façon",
    L"%s ne peut rejoindre %s",
    L"Vous devez d'abord engager des mercenaires !",
    L"Ce véhicule ne peut circuler que sur les routes !",
    // 41-45
    L"Vous ne pouvez réaffecter des mercenaires qui sont en déplacement",
    L"Plus d'essence !",
    L"%s est trop fatigué(e) pour entreprendre ce voyage.",
    L"Personne n'est capable de conduire ce véhicule.",
    L"L'un au moins des membres de cette escouade ne peut se déplacer.",
    // 46-50
    L"L'un au moins des AUTRES mercenaires ne peut se déplacer.",
    L"Le véhicule est trop endommagé !",
    L"Deux mercenaires au plus peuvent être assignés à l'entraînement de la milice dans chaque "
    L"secteur.",
    L"Le robot ne peut se déplacer sans son contrôleur. Mettez-les ensemble dans la même escouade.",
};

// help text used during strategic route plotting
STR16 pMapPlotStrings[] = {
    L"Cliquez à nouveau sur votre destination pour la confirmer ou cliquez sur d'autres secteurs "
    L"pour définir de nouvelles étapes.",
    L"Route confirmée.",
    L"Destination inchangée.",
    L"Route annulée.",
    L"Route raccourcie.",
};

// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] = {
    L"Cliquez sur la nouvelle destination de vos mercenaires.",
    L"OK. Les mercenaires arriveront en %s",
    L"Les mercenaires ne peuvent être déployés ici, l'espace aérien n'est pas sécurisé !",
    L"Annulé. Secteur d'arrivée inchangé.",
    L"L'espace aérien en %s n'est plus sûr ! Le secteur d'arrivée est maintenant %s.",
};

// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] = {
    L"Inventaire (|E|n|t|r|é|e)",
    L"Lancer objet",
    L"Quitter Inventaire (|E|n|t|r|é|e)",
};

// male version of where equipment is left
STR16 pMercHeLeaveString[] = {
    L"%s doit-il abandonner son équipement sur place (%s) ou à Drassen (B13) avant de quitter "
    L"Arulco ?",
    L"%s doit-il abandonner son équipement sur place (%s) ou à Omerta (A9) avant de quitter Arulco "
    L"?",
    L"est sur le point de partir et laissera son équipement à Omerta (A9).",
    L"est sur le point de partir et laissera son équipement à Drassen (B13).",
    L"%s est sur le point de partir et laissera son équipement en %s.",
};

// female version
STR16 pMercSheLeaveString[] = {
    L"%s doit-elle abandonner son équipement sur place (%s) ou à Drassen (B13) avant de quitter "
    L"Arulco ?",
    L"%s doit-elle abandonner son équipement sur place (%s) ou à Omerta (A9) avant de quitter "
    L"Arulco ?",
    L"est sur le point de partir et laissera son équipement à Omerta (A9).",
    L"est sur le point de partir et laissera son équipement à Drassen (B13).",
    L"%s est sur le point de partir et laissera son équipement en %s.",
};

STR16 pMercContractOverStrings[] = {
    L"a rempli son contrat, il est rentré chez lui.",  // merc's contract is over and has departed
    L"a rempli son contrat, elle est rentrée chez elle.",  // merc's contract is over and has
                                                           // departed
    L"est parti, son contrat ayant été annulé.",           // merc's contract has been terminated
    L"est partie, son contrat ayant été annulé.",          // merc's contract has been terminated
    L"Vous devez trop d'argent au M.E.R.C., %s quitte Arulco.",  // Your M.E.R.C. account is invalid
                                                                 // so merc left
};

// Text used on IMP Web Pages

STR16 pImpPopUpStrings[] = {
    L"Code Incorrect",
    L"Vous allez établir un nouveau profil. Etes-vous sûr de vouloir recommencer ?",
    L"Veuillez entrer votre nom et votre sexe.",
    L"Vous n'avez pas les moyens de vous offrir une analyse de profil.",
    L"Option inaccessible pour le moment.",
    L"Pour que cette analyse soit efficace, il doit vous rester au moins une place dans votre "
    L"escouade.",
    L"Profil déjà établi.",
};

// button labels used on the IMP site

STR16 pImpButtonText[] = {
    L"Nous",              // about the IMP site
    L"COMMENCER",         // begin profiling
    L"Personnalité",      // personality section
    L"Caractéristiques",  // personal stats/attributes section
    L"Portrait",          // the personal portrait selection
    L"Voix %d",           // the voice selection
    L"OK",                // done profiling
    L"Recommencer",       // start over profiling
    L"Oui, la réponse en surbrillance me convient.",
    L"Oui",
    L"Non",
    L"Terminé",                    // finished answering questions
    L"Préc.",                      // previous question..abbreviated form
    L"Suiv.",                      // next question
    L"OUI, JE SUIS SUR.",          // oui, I am certain
    L"NON, JE VEUX RECOMMENCER.",  // non, I want to start over the profiling process
    L"OUI",
    L"NON",
    L"Retour",   // back one page
    L"Annuler",  // cancel selection
    L"Oui, je suis sûr.",
    L"Non, je ne suis pas sûr.",
    L"Registre",  // the IMP site registry..when name and gender is selected
    L"Analyse",   // analyzing your profile results
    L"OK",
    L"Voix",
};

STR16 pExtraIMPStrings[] = {
    L"Pour lancer l'analyse, cliquez sur Personnalité.",
    L"Cliquez maintenant sur Caractéristiques.",
    L"Passons maintenant à la galerie de portraits.",
    L"Pour que l'analyse soit complète, choisissez une voix.",
};

STR16 pFilesTitle[] = {
    L"Fichiers",
};

STR16 pFilesSenderList[] = {
    L"Rapport Arulco",  // the recon report sent to the player. Recon is an abbreviation for
                        // reconissance
    L"Intercept #1",  // first intercept file .. Intercept is the title of the organization sending
                      // the file...similar in function to INTERPOL/CIA/KGB..refer to fist record in
                      // files.txt for the translated title
    L"Intercept #2",  // second intercept file
    L"Intercept #3",  // third intercept file
    L"Intercept #4",  // fourth intercept file
    L"Intercept #5",  // fifth intercept file
    L"Intercept #6",  // sixth intercept file
};

// Text having to do with the History Log

STR16 pHistoryTitle[] = {
    L"Historique",
};

STR16 pHistoryHeaders[] = {
    L"Jour",       // the day the history event occurred
    L"Page",       // the current page in the history report we are in
    L"Jour",       // the days the history report occurs over
    L"Lieu",       // location (in sector) the event occurred
    L"Evénement",  // the event label
};

// various history events
// THESE STRINGS ARE "HISTORY LOG" STRINGS AND THEIR LENGTH IS VERY LIMITED.
// PLEASE BE MINDFUL OF THE LENGTH OF THESE STRINGS. ONE WAY TO "TEST" THIS
// IS TO TURN "CHEAT MODE" ON AND USE CONTROL-R IN THE TACTICAL SCREEN, THEN
// GO INTO THE LAPTOP/HISTORY LOG AND CHECK OUT THE STRINGS. CONTROL-R INSERTS
// MANY (NOT ALL) OF THE STRINGS IN THE FOLLOWING LIST INTO THE GAME.
STR16 pHistoryStrings[] = {
    L"",  // leave this line blank
    // 1-5
    L"%s engagé(e) sur le site A.I.M.",    // merc was hired from the aim site
    L"%s engagé(e) sur le site M.E.R.C.",  // merc was hired from the aim site
    L"%s meurt.",                          // merc was killed
    L"Versements M.E.R.C.",                // paid outstanding bills at MERC
    L"Ordre de mission d'Enrico Chivaldori accepté",
    // 6-10
    L"Profil IMP",
    L"Souscription d'un contrat d'assurance pour %s.",  // insurance contract purchased
    L"Annulation du contrat d'assurance de %s.",        // insurance contract canceled
    L"Indemnité pour %s.",                              // insurance claim payout for merc
    L"Extension du contrat de %s (1 jour).",            // Extented "mercs name"'s for a day
    // 11-15
    L"Extension du contrat de %s (1 semaine).",   // Extented "mercs name"'s for a week
    L"Extension du contrat de %s (2 semaines).",  // Extented "mercs name"'s 2 weeks
    L"%s a été renvoyé(e).",                      // "merc's name" was dismissed.
    L"%s a démissionné.",                         // "merc's name" quit.
    L"quête commencée.",                          // a particular quest started
    // 16-20
    L"quête achevée.",
    L"Entretien avec le chef des mineurs de %s",  // talked to head miner of town
    L"Libération de %s",
    L"Activation du mode triche",
    L"Le ravitaillement devrait arriver demain à Omerta",
    // 21-25
    L"%s a quitté l'escouade pour épouser Daryl Hick",
    L"Expiration du contrat de %s.",
    L"Recrutement de %s.",
    L"Plainte d'Enrico pour manque de résultats",
    L"Victoire",
    // 26-30
    L"La mine de %s commence à s'épuiser",
    L"La mine de %s est épuisée",
    L"La mine de %s a été fermée",
    L"La mine de %s a été réouverte",
    L"Une prison du nom de Tixa a été découverte.",
    // 31-35
    L"Rumeurs sur une usine d'armes secrètes : Orta.",
    L"Les chercheurs d'Orta vous donnent des fusils à roquettes.",
    L"Deidranna fait des expériences sur les cadavres.",
    L"Frank parle de combats organisés à San Mona.",
    L"Un témoin pense avoir aperçu quelque chose dans les mines.",
    // 36-40
    L"Rencontre avec Devin - vend des explosifs.",
    L"Rencontre avec Mike, le fameux ex-mercenaire de l'AIM !",
    L"Rencontre avec Tony - vend des armes.",
    L"Fusil à roquettes récupéré auprès du Sergent Krott.",
    L"Acte de propriété du magasin d'Angel donné à Kyle.",
    // 41-45
    L"Madlab propose de construire un robot.",
    L"Gabby fait des décoctions rendant invisible aux créatures.",
    L"Keith est hors-jeu.",
    L"Howard fournit du cyanure à la Reine Deidranna.",
    L"Rencontre avec Keith - vendeur à Cambria.",
    // 46-50
    L"Rencontre avec Howard - pharmacien à Balime",
    L"Rencontre avec Perko - réparateur en tous genres.",
    L"Rencontre avec Sam de Balime - vendeur de matériel.",
    L"Franz vend du matériel électronique.",
    L"Arnold tient un magasin de réparations à Grumm.",
    // 51-55
    L"Fredo répare le matériel électronique à Grumm.",
    L"Don provenant d'un homme influent de Balime.",
    L"Rencontre avec Jake, vendeur de pièces détachées.",
    L"Clé électronique reçue.",
    L"Corruption de Walter pour ouvrir l'accès aux sous-sols.",
    // 56-60
    L"Dave refait gratuitement le plein s'il a du carburant.",
    L"Pot-de-vin donné à Pablo.",
    L"Kingpin cache un trésor dans la mine de San Mona.",
    L"Victoire de %s dans l'Extreme Fighting",
    L"Défaite de %s dans l'Extreme Fighting",
    // 61-65
    L"Disqualification de %s dans l'Extreme Fighting",
    L"Importante somme découverte dans la mine abandonnée.",
    L"Rencontre avec un tueur engagé par Kingpin.",
    L"Perte du secteur",  // ENEMY_INVASION_CODE
    L"Secteur défendu",
    // 66-70
    L"Défaite",    // ENEMY_ENCOUNTER_CODE
    L"Embuscade",  // ENEMY_AMBUSH_CODE
    L"Embuscade ennemie déjouée",
    L"Echec de l'attaque",  // ENTERING_ENEMY_SECTOR_CODE
    L"Réussite de l'attaque !",
    // 71-75
    L"Attaque de créatures",       // CREATURE_ATTACK_CODE
    L"Attaque de chats sauvages",  // BLOODCAT_AMBUSH_CODE
    L"Elimination des chats sauvages",
    L"%s a été tué(e)",
    L"Tête de terroriste donnée à Carmen",
    L"Reste Slay",
    L"%s a été tué(e)",
};

STR16 pHistoryLocations[] = {
    L"N/A",  // N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] = {
    L"E-mail",      L"Internet", L"Finances", L"Personnel", L"Historique", L"Fichiers", L"Eteindre",
    L"sir-FER 4.0",  // our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] = {
    L"A.I.M.", L"Bobby Ray", L"I.M.P",     L"M.E.R.C.",
    L"Morgue", L"Fleuriste", L"Assurance", L"Annuler",
};

STR16 pBookmarkTitle[] = {
    L"Favoris",
    L"Faites un clic droit pour accéder plus tard à ce menu.",
};

// When loading or download a web page

STR16 pDownloadString[] = {
    L"Téléchargement",
    L"Chargement",
};

// This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] = {
    L"OK",
    L"Prendre",  // take money from merc
    L"Donner",   // give money to merc
    L"Annuler",  // cancel transaction
    L"Effacer",  // clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] = {
    L"Transférer $",  // transfer money to merc -- short form
    L"Stats",         // view stats of the merc
    L"Inventaire",    // view the inventory of the merc
    L"Tâche",
};

STR16 sATMText[] = {
    L"Transférer les fonds ?",                    // transfer funds to merc?
    L"Ok ?",                                      // are we certain?
    L"Entrer montant",                            // enter the amount you want to transfer to merc
    L"Choix du type",                             // select the type of transfer to merc
    L"Fonds insuffisants",                        // not enough money to transfer to merc
    L"Le montant doit être un multiple de 10 $",  // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages.
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] = {
    L"Erreur",
    L"Le serveur ne trouve pas l'entrée DNS.",
    L"Vérifiez l'adresse URL et essayez à nouveau.",
    L"OK",
    L"Connexion à l'hôte.",
};

STR16 pPersonnelString[] = {
    L"Mercs :",  // mercs we have
};

STR16 pWebTitle[] = {
    L"sir-FER 4.0",  // our name for the version of the browser, play on company name
};

// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] = {
    L"A.I.M.",
    L"Membres A.I.M.",
    L"Galerie A.I.M.",  // a mug shot is another name for a portrait
    L"Tri A.I.M.",
    L"A.I.M.",
    L"Anciens A.I.M.",
    L"Règlement A.I.M.",
    L"Historique A.I.M.",
    L"Liens A.I.M.",
    L"M.E.R.C.",
    L"Comptes M.E.R.C.",
    L"Enregistrement M.E.R.C.",
    L"Index M.E.R.C.",
    L"Bobby Ray",
    L"Bobby Ray - Armes",
    L"Bobby Ray - Munitions",
    L"Bobby Ray - Armures",
    L"Bobby Ray - Divers",  // misc is an abbreviation for miscellaneous
    L"Bobby Ray - Occasions",
    L"Bobby Ray - Commande",
    L"I.M.P.",
    L"I.M.P.",
    L"Service des Fleuristes Associés",
    L"Service des Fleuristes Associés - Exposition",
    L"Service des Fleuristes Associés - Bon de commande",
    L"Service des Fleuristes Associés - Cartes",
    L"Malleus, Incus & Stapes Courtiers",
    L"Information",
    L"Contrat",
    L"Commentaires",
    L"Morgue McGillicutty",
    L"",
    L"URL introuvable.",
    L"Bobby Ray - Dernières commandes",
    L"",
    L"",
};

STR16 pShowBookmarkString[] = {
    L"Sir-Help",
    L"Cliquez à nouveau pour accéder aux Favoris.",
};

STR16 pLaptopTitles[] = {
    L"Boîte aux lettres", L"Fichiers", L"Personnel", L"Bookkeeper Plus", L"Historique",
};

STR16 pPersonnelDepartedStateStrings[] = {
    // reasons why a merc has left.
    L"Mort en mission", L"Parti(e)", L"Autre", L"Mariage", L"Contrat terminé", L"Quitter",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] = {
    L"Equipe actuelle",    L"Départs",  L"Coût quotidien :", L"Coût maximum :", L"Coût minimum :",
    L"Morts en mission :", L"Partis :", L"Autres :",
};

STR16 pPersonnelCurrentTeamStatsStrings[] = {
    L"Minimum",
    L"Moyenne",
    L"Maximum",
};

STR16 pPersonnelTeamStatsStrings[] = {
    L"SAN", L"AGI", L"DEX", L"FOR", L"COM", L"SAG", L"NIV", L"TIR", L"TECH", L"EXPL", L"MED",
};

// horizontal and vertical indices on the map screen

STR16 pMapVertIndex[] = {
    L"X", L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H",
    L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P",
};

STR16 pMapHortIndex[] = {
    L"X", L"1",  L"2",  L"3",  L"4",  L"5",  L"6",  L"7",  L"8",
    L"9", L"10", L"11", L"12", L"13", L"14", L"15", L"16",
};

STR16 pMapDepthIndex[] = {
    L"",
    L"-1",
    L"-2",
    L"-3",
};

// text that appears on the contract button

STR16 pContractButtonString[] = {
    L"Contrat",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] = {
    L"Continuer",
    L"Stop",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

UINT16 LargeTacticalStr[][LARGE_STRING_LENGTH] = {
    L"Vous avez été vaincu dans ce secteur !",
    L"L'ennemi, sans aucune compassion, ne fait pas de quartier !",
    L"Vos mercenaires inconscients ont été capturés !",
    L"Vos mercenaires ont été faits prisonniers.",
};

// Insurance Contract.c
// The text on the buttons at the bottom of the screen.

STR16 InsContractText[] = {
    L"Précédent",
    L"Suivant",
    L"Accepter",
    L"Annuler",
};

// Insurance Info
// Text on the buttons on the bottom of the screen

STR16 InsInfoText[] = {
    L"Précédent",
    L"Suivant",
};

// For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16 MercAccountText[] = {
    // Text on the buttons on the bottom of the screen
    L"Autoriser",
    L"Home",
    L"Compte # :",
    L"Merc",
    L"Jours",
    L"Taux",  // 5
    L"Montant",
    L"Total :",
    L"Désirez-vous autoriser le versement de %s ?",  // the %s is a string that contains the dollar
                                                     // amount ( ex. "$150" )
};

// For use at the M.E.R.C. web site. Text relating a MERC mercenary

STR16 MercInfo[] = {
    L"Santé",
    L"Agilité",
    L"Dextérité",
    L"Force",
    L"Commandement",
    L"Sagesse",
    L"Niveau",
    L"Tir",
    L"Technique",
    L"Explosifs",
    L"Médecine",

    L"Précédent",
    L"Engager",
    L"Suivant",
    L"Infos complémentaires",
    L"Home",
    L"Engagé",
    L"Salaire :",
    L"Par jour",
    L"Décédé(e)",

    L"Vous ne pouvez engager plus de 18 mercenaires.",
    L"Indisponible",
};

// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16 MercNoAccountText[] = {
    // Text on the buttons at the bottom of the screen
    L"Ouvrir compte",
    L"Annuler",
    L"Vous ne possédez pas de compte. Désirez-vous en ouvrir un ?",
};

// For use at the M.E.R.C. web site. MERC Homepage

STR16 MercHomePageText[] = {
    // Description of various parts on the MERC page
    L"Speck T. Kline, fondateur",
    L"Cliquez ici pour ouvrir un compte",
    L"Cliquez ici pour voir votre compte",
    L"Cliquez ici pour consulter les fichiers",
    // The version number on the video conferencing system that pops up when Speck is talking
    L"Speck Com v3.2",
};

// For use at MiGillicutty's Web Page.

STR16 sFuneralString[] = {
    L"Morgue McGillicutty : A votre écoute depuis 1983.",
    L"Murray \"Pops\" McGillicutty, notre directeur bien aimé, est un ancien mercenaire de l'AIM. "
    L"Sa spécialité : la mort des autres.",
    L"Pops l'a côtoyée pendant si longtemps qu'il est un expert de la mort, à tous points de vue.",
    L"La morgue McGillicutty vous offre un large éventail de services funéraires, depuis une "
    L"écoute compréhensive jusqu'à la reconstitution des corps... dispersés.",
    L"Laissez donc la morgue McGillicutty vous aider, pour que votre compagnon repose enfin en "
    L"paix.",

    // Text for the various links available at the bottom of the page
    L"ENVOYER FLEURS",
    L"CERCUEILS & URNES",
    L"CREMATION",
    L"SERVICES FUNERAIRES",
    L"NOTRE ETIQUETTE",

    // The text that comes up when you click on any of the links ( except for send flowers ).
    L"Le concepteur de ce site s'est malheureusement absenté pour cause de décès familial. Il "
    L"reviendra dès que possible pour rendre ce service encore plus efficace.",
    L"Veuillez croire en nos sentiments les plus respectueux dans cette période qui doit vous être "
    L"douloureuse.",
};

// Text for the florist Home page

STR16 sFloristText[] = {
    // Text on the button on the bottom of the page

    L"Vitrine",

    // Address of United Florist

    L"\"Nous livrons partout dans le monde\"",
    L"0-800-SENTMOI",
    L"333 NoseGay Dr, Seedy City, CA USA 90210",
    L"http://www.sentmoi.com",

    // detail of the florist page

    L"Rapides et efficaces !",
    L"Livraison en 24 heures partout dans le monde (ou presque).",
    L"Les prix les plus bas (ou presque) !",
    L"Si vous trouvez moins cher, nous vous livrons gratuitement une douzaine de roses !",
    L"Flore, Faune & Fleurs depuis 1981.",
    L"Nos bombardiers (recyclés) vous livrent votre bouquet dans un rayon de 20 km (ou presque). "
    L"N'importe quand - N'importe où !",
    L"Nous répondons à tous vos besoins (ou presque) !",
    L"Bruce, notre expert fleuriste-conseil, trouvera pour vous les plus belles fleurs et vous "
    L"composera le plus beau bouquet que vous ayez vu !",
    L"Et n'oubliez pas que si nous ne l'avons pas, nous pouvons le faire pousser - et vite !",
};

// Florist OrderForm

STR16 sOrderFormText[] = {
    // Text on the buttons

    L"Retour",
    L"Envoi",
    L"Annuler",
    L"Galerie",

    L"Nom du bouquet :",
    L"Prix :",  // 5
    L"Référence :",
    L"Date de livraison",
    L"jour suivant",
    L"dès que possible",
    L"Lieu de livraison",  // 10
    L"Autres services",
    L"Pot Pourri (10$)",
    L"Roses Noires (20$)",
    L"Nature Morte (10$)",
    L"Gâteau (si dispo)(10$)",  // 15
    L"Carte personnelle :",
    L"Veuillez écrire votre message en 75 caractères maximum...",
    L"...ou utiliser l'une de nos",

    L"CARTES STANDARDS",
    L"Informations",  // 20

    // The text that goes beside the area where the user can enter their name

    L"Nom :",
};

// Florist Gallery.c

STR16 sFloristGalleryText[] = {
    // text on the buttons

    L"Préc.",  // abbreviation for previous
    L"Suiv.",  // abbreviation for next

    L"Cliquez sur le bouquet que vous désirez commander.",
    L"Note : les bouquets \"pot pourri\" et \"nature morte\" vous seront facturés 10$ "
    L"supplémentaires.",

    // text on the button

    L"Home",
};

// Florist Cards

STR16 sFloristCards[] = {
    L"Faites votre choix",
    L"Retour",
};

// Text for Bobby Ray's Mail Order Site

STR16 BobbyROrderFormText[] = {
    L"Commande",       // Title of the page
    L"Qté",            // The number of items ordered
    L"Poids (%s)",     // The weight of the item
    L"Description",    // The name of the item
    L"Prix unitaire",  // the item's weight
    L"Total",          // 5	// The total price of all of items of the same type
    L"Sous-total",     // The sub total of all the item totals added
    L"Transport",      // S&H is an acronym for Shipping and Handling
    L"Total",          // The grand total of all item totals + the shipping and handling
    L"Lieu de livraison",
    L"Type d'envoi",                 // 10	// See below
    L"Coût (par %s.)",               // The cost to ship the items
    L"Du jour au lendemain",         // Gets deliverd the next day
    L"2 c'est mieux qu'un",          // Gets delivered in 2 days
    L"Jamais 2 sans 3",              // Gets delivered in 3 days
    L"Effacer commande",             // 15			// Clears the order page
    L"Confirmer commande",           // Accept the order
    L"Retour",                       // text on the button that returns to the previous page
    L"Home",                         // Text on the button that returns to the home page
    L"* Matériel d'occasion",        // Disclaimer stating that the item is used
    L"Vous n'avez pas les moyens.",  // 20	// A popup message that to warn of not enough money
    L"<AUCUNE>",                     // Gets displayed when there is non valid city selected
    L"Etes-vous sûr de vouloir envoyer cette commande à %s ?",  // A popup that asks if the city
                                                                // selected is the correct one
    L"Poids total **",  // Displays the weight of the package
    L"** Pds Min.",     // Disclaimer states that there is a minimum weight for the package
    L"Envois",
};

// This text is used when on the various Bobby Ray Web site pages that sell items

STR16 BobbyRText[] = {
    L"Pour commander",  // Title
    // instructions on how to order
    L"Cliquez sur les objets désirés. Cliquez à nouveau pour sélectionner plusieurs exemplaires "
    L"d'un même objet. Effectuez un clic droit pour désélectionner un objet. Il ne vous reste plus "
    L"qu'à passer commande.",

    // Text on the buttons to go the various links

    L"Objets précédents",  //
    L"Armes",              // 3
    L"Munitions",          // 4
    L"Armures",            // 5
    L"Divers",             // 6	//misc is an abbreviation for miscellaneous
    L"Occasion",           // 7
    L"Autres objets",
    L"BON DE COMMANDE",
    L"Home",  // 10

    // The following 2 lines are used on the Ammunition page.
    // They are used for help text to display how many items the player's merc has
    // that can use this type of ammo

    L"Votre équipe possède",                          // 11
    L"arme(s) qui utilise(nt) ce type de munitions",  // 12

    // The following lines provide information on the items

    L"Poids :",          // Weight of all the items of the same type
    L"Cal :",            // the caliber of the gun
    L"Chrg :",           // number of rounds of ammo the Magazine can hold
    L"Por :",            // The range of the gun
    L"Dgt :",            // Damage of the weapon
    L"CDT :",            // Weapon's Rate Of Fire, acronym ROF
    L"Prix :",           // Cost of the item
    L"En réserve :",     // The number of items still in the store's inventory
    L"Qté commandée :",  // The number of items on order
    L"Endommagé",        // If the item is damaged
    L"Poids :",          // the Weight of the item
    L"Sous-total :",     // The total cost of all items on order
    L"* %% efficacité",  // if the item is damaged, displays the percent function of the item

    // Popup that tells the player that they can only order 10 items at a time

    L"Pas de chance ! Vous ne pouvez commander que 10 objets à la fois. Si vous désirez passer une "
    L"commande plus importante, il vous faudra remplir un nouveau bon de commande.",

    // A popup that tells the user that they are trying to order more items then the store has in
    // stock

    L"Nous sommes navrés, mais nos stocks sont vides. N'hésitez pas à revenir plus tard !",

    // A popup that tells the user that the store is temporarily sold out

    L"Nous sommes navrés, mais nous n'en avons plus en rayon.",

};

// Text for Bobby Ray's Home Page

STR16 BobbyRaysFrontText[] = {
    // Details on the web site

    L"Vous cherchez des armes et du matériel militaire ? Vous avez frappé à la bonne porte",
    L"Un seul credo : force de frappe !",
    L"Occasions et secondes mains",

    // Text for the various links to the sub pages

    L"Divers",
    L"ARMES",
    L"MUNITIONS",  // 5
    L"ARMURES",

    // Details on the web site

    L"Si nous n'en vendons pas, c'est que ça n'existe pas !",
    L"En Construction",
};

// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug
// shot page

STR16 AimSortText[] = {
    L"Membres A.I.M.",  // Title
    // Title for the way to sort
    L"Tri par :",

    // sort by...

    L"Prix",
    L"Expérience",
    L"Tir",
    L"Médecine",
    L"Explosifs",
    L"Technique",

    // Text of the links to other AIM pages

    L"Afficher l'index de la galerie de portraits",
    L"Consulter le fichier individuel",
    L"Consulter la galerie des anciens de l'A.I.M.",

    // text to display how the entries will be sorted

    L"Ascendant",
    L"Descendant",
};

// Aim Policies.c
// The page in which the AIM policies and regulations are displayed

STR16 AimPolicyText[] = {
    // The text on the buttons at the bottom of the page

    L"Précédent", L"Home AIM", L"Index", L"Suivant", L"Je refuse", L"J'accepte",
};

// Aim Member.c
// The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot
// index

STR16 AimMemberText[] = {
    L"Cliquez pour",
    L"contacter le mercenaire.",
    L"Clic droit pour",
    L"afficher l'index.",
};

// Aim Member.c
// The page in which the players hires AIM mercenaries

STR16 CharacterInfo[] = {
    // The various attributes of the merc

    L"Santé", L"Agilité", L"Dextérité", L"Force", L"Commandement", L"Sagesse", L"Niveau", L"Tir",
    L"Technique", L"Explosifs",
    L"Médecine",  // 10

    // the contract expenses' area

    L"Paie", L"Contrat", L"1 jour", L"1 semaine", L"2 semaines",

    // text for the buttons that either go to the previous merc,
    // start talking to the merc, or go to the next merc

    L"Précédent", L"Contacter", L"Suivant",

    L"Info. complémentaires",  // Title for the additional info for the merc's bio
    L"Membres actifs",         // 20		// Title of the page
    L"Matériel optionnel :",   // Displays the optional gear cost
    L"Dépôt Médical",          // If the merc required a medical deposit, this is displayed
};

// Aim Member.c
// The page in which the player's hires AIM mercenaries

// The following text is used with the video conference popup

STR16 VideoConfercingText[] = {
    L"Contrat :",  // Title beside the cost of hiring the merc

    // Text on the buttons to select the length of time the merc can be hired

    L"1 jour", L"1 semaine", L"2 semaines",

    // Text on the buttons to determine if you want the merc to come with the equipment

    L"Pas d'équipement", L"Acheter équipement",

    // Text on the Buttons

    L"TRANSFERT",   // to actually hire the merc
    L"Annuler",     // go back to the previous menu
    L"ENGAGER",     // go to menu in which you can hire the merc
    L"RACCROCHER",  // stops talking with the merc
    L"OK",
    L"MESSAGE",  // if the merc is not there, you can leave a message

    // Text on the top of the video conference popup

    L"Conférence vidéo avec", L"Connexion. . .",

    L"dépôt compris"  // Displays if you are hiring the merc with the medical deposit
};

// Aim Member.c
// The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16 AimPopUpText[] = {
    L"TRANSFERT ACCEPTE",   // You hired the merc
    L"TRANSFERT REFUSE",    // Player doesn't have enough money, message 1
    L"FONDS INSUFFISANTS",  // Player doesn't have enough money, message 2

    // if the merc is not available, one of the following is displayed over the merc's face

    L"En mission",
    L"Veuillez laisser un message",
    L"Décédé",

    // If you try to hire more mercs than game can support

    L"Votre équipe contient déjà 18 mercenaires.",

    L"Message pré-enregistré",
    L"Message enregistré",
};

// AIM Link.c

STR16 AimLinkText[] = {
    L"Liens A.I.M.",  // The title of the AIM links page
};

// Aim History

// This page displays the history of AIM

STR16 AimHistoryText[] = {
    L"Historique A.I.M.",  // Title

    // Text on the buttons at the bottom of the page

    L"Précédent",
    L"Home",
    L"Anciens",
    L"Suivant",
};

// Aim Mug Shot Index

// The page in which all the AIM members' portraits are displayed in the order selected by the AIM
// sort page.

STR16 AimFiText[] = {
    // displays the way in which the mercs were sorted

    L"Prix",
    L"Expérience",
    L"Tir",
    L"Médecine",
    L"Explosifs",
    L"Technique",

    // The title of the page, the above text gets added at the end of this text

    L"Tri ascendant des membres de l'A.I.M. par %s",
    L"Tri descendant des membres de l'A.I.M. par %s",

    // Instructions to the players on what to do

    L"Cliquez pour",
    L"sélectionner le mercenaire",  // 10
    L"Clic droit pour",
    L"les options de tri",

    // Gets displayed on top of the merc's portrait if they are...

    L"Absent",
    L"Décédé",  // 14
    L"En mission",
};

// AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are non longer
// with AIM

STR16 AimAlumniText[] = {
    // Text of the buttons

    L"PAGE 1", L"PAGE 2", L"PAGE 3",

    L"Anciens",  // Title of the page

    L"OK"  // Stops displaying information on selected merc
};

// AIM Home Page

STR16 AimScreenText[] = {
    // AIM disclaimers

    L"A.I.M. et le logo A.I.M. sont des marques déposées dans la plupart des pays.",
    L"N'espérez même pas nous copier !",
    L"Copyright 1998-1999 A.I.M., Ltd. Tous droits réservés.",

    // Text for an advertisement that gets displayed on the AIM page

    L"Service des Fleuristes Associés",
    L"\"Nous livrons partout dans le monde\"",  // 10
    L"Faites-le dans les règles de l'art",
    L"... la première fois",
    L"Si nous ne l'avons pas, c'est que vous n'en avez pas besoin.",
};

// Aim Home Page

STR16 AimBottomMenuText[] = {
    // Text for the links at the bottom of all AIM pages
    L"Home", L"Membres", L"Anciens", L"Règlement", L"Historique", L"Liens",
};

// ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with
// the various store clerks scattered through out the game.

STR16 SKI_Text[] = {
    L"MARCHANDISE EN STOCK",  // Header for the merchandise available
    L"PAGE",                  // The current store inventory page being displayed
    L"COUT TOTAL",            // The total cost of the the items in the Dealer inventory area
    L"VALEUR TOTALE",         // The total value of items player wishes to sell
    L"EVALUATION",            // Button text for dealer to evaluate items the player wants to sell
    L"TRANSACTION",           // Button text which completes the deal. Makes the transaction.
    L"OK",                    // Text for the button which will leave the shopkeeper interface.
    L"COUT REPARATION",       // The amount the dealer will charge to repair the merc's goods
    L"1 HEURE",    // SINGULAR! The text underneath the inventory slot when an item is given to the
                   // dealer to be repaired
    L"%d HEURES",  // PLURAL!   The text underneath the inventory slot when an item is given to the
                   // dealer to be repaired
    L"REPARE",  // Text appearing over an item that has just been repaired by a NPC repairman dealer
    L"Plus d'emplacements libres.",  // Message box that tells the user there is non more room to
                                     // put there stuff
    L"%d MINUTES",  // The text underneath the inventory slot when an item is given to the dealer to
                    // be repaired
    L"Objet lâché à terre.",
};

// ShopKeeper Interface
// for the bank machine panels. Referenced here is the acronym ATM, which means Automatic Teller
// Machine

STR16 SkiAtmText[] = {
    // Text on buttons on the banking machine, displayed at the bottom of the page
    L"0",       L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9",
    L"OK",       // Transfer the money
    L"Prendre",  // Take money from the player
    L"Donner",   // Give money to the player
    L"Annuler",  // Cancel the transfer
    L"Effacer",  // Clear the money display
};

// Shopkeeper Interface
STR16 gzSkiAtmText[] = {

    // Text on the bank machine panel that....
    L"Choix",    // tells the user to select either to give or take from the merc
    L"Montant",  // Enter the amount to transfer
    L"Transfert de fonds au mercenaire",  // Giving money to the merc
    L"Transfert de fonds du mercenaire",  // Taking money from the merc
    L"Fonds insuffisants",                // Not enough money to transfer
    L"Solde",                             // Display the amount of money the player currently has
};

STR16 SkiMessageBoxText[] = {
    L"Voulez-vous déduire %s de votre compte pour combler la différence ?",
    L"Pas assez d'argent. Il vous manque %s",
    L"Voulez-vous déduire %s de votre compte pour couvrir le coût ?",
    L"Demander au vendeur de lancer la transaction",
    L"Demander au vendeur de réparer les objets sélectionnés",
    L"Terminer l'entretien",
    L"Solde actuel",
};

// OptionScreen.c

STR16 zOptionsText[] = {
    // button Text
    L"Enregistrer",
    L"Charger partie",
    L"Quitter",
    L"OK",

    // Text above the slider bars
    L"Effets",
    L"Dialogue",
    L"Musique",

    // Confirmation pop when the user selects..
    L"Quitter la partie et revenir au menu principal ?",

    L"Activez le mode Dialogue ou Sous-titre.",
};

// SaveLoadScreen
STR16 zSaveLoadText[] = {
    L"Enregistrer",
    L"Charger partie",
    L"Annuler",
    L"Enregistrement",
    L"Chargement",

    L"Enregistrement réussi",
    L"ERREUR lors de la sauvegarde !",
    L"Chargement réussi",
    L"ERREUR lors du chargement !",

    L"La version de la sauvegarde est différente de celle du jeu. Désirez-vous continuer ?",
    L"Les fichiers de sauvegarde sont peut-être altérés. Voulez-vous les effacer ?",

// Translators, the next two strings are for the same thing.  The first one is for beta version
// releases and the second one is used for the final version.  Please don't modify the "#ifdef
// JA2BETAVERSION" or the "#else" or the "#endif" as they are used by the compiler and will cause
// program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
    L"La version de la sauvegarde a changé. Désirez-vous continuer ?",
#else
    L"Tentative de chargement d'une sauvegarde de version précédente. Voulez-vous effectuer une "
    L"mise à jour ?",
#endif

// Translators, the next two strings are for the same thing.  The first one is for beta version
// releases and the second one is used for the final version.  Please don't modify the "#ifdef
// JA2BETAVERSION" or the "#else" or the "#endif" as they are used by the compiler and will cause
// program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
    L"La version de la sauvegarde a changé. Désirez-vous continuer?",
#else
    L"Tentative de chargement d'une sauvegarde de version précédente. Voulez-vous effectuer une "
    L"mise à jour ?",
#endif

    L"Etes-vous sûr de vouloir écraser la sauvegarde #%d ?",
    L"Voulez-vous charger la sauvegarde #%d ?",

    // The first %d is a number that contains the amount of free space on the users hard drive,
    // the second is the recommended amount of free space.
    L"Votre risquez de manquer d'espace disque. Il ne vous reste que %d Mo de libre alors que le "
    L"jeu nécessite %d Mo d'espace libre.",

    L"Enregistrement...",  // When saving a game, a message box with this string appears on the
                           // screen

    L"Peu d'armes",
    L"Beaucoup d'armes",
    L"Style réaliste",
    L"Style SF",

    L"Difficulté",
};

// MapScreen
STR16 zMarksMapScreenText[] = {
    L"Niveau carte",
    L"Vous n'avez pas de milice : vous devez entraîner les habitants de la ville.",
    L"Revenu quotidien",
    L"Assurance vie",
    L"%s n'est pas fatigué.",
    L"%s est en mouvement et ne peut dormir.",
    L"%s est trop fatigué pour obéir.",
    L"%s conduit.",
    L"L'escouade ne peut progresser si l'un de ses membres se repose.",

    // stuff for contracts
    L"Vous pouvez payer les honoraires de ce mercenaire, mais vous ne pouvez pas vous offrir son "
    L"assurance.",
    L"La prime d'assurance de %s coûte %s pour %d jour(s) supplémentaire(s). Voulez-vous les payer "
    L"?",
    L"Inventaire du Secteur",
    L"Le mercenaire a un dépôt médical.",

    // other items
    L"Medics",    // people acting a field medics and bandaging wounded mercs
    L"Patients",  // people who are being bandaged by a medic
    L"OK",        // Continue on with the game after autobandage is complete
    L"Stop",      // Stop autobandaging of patients by medics now
    L"Désolé. Cette option n'est pas disponible.",  // informs player this option/button has been
                                                    // disabled in the demo
    L"%s n'a pas de trousse à outil.",
    L"%s n'a pas de trousse de soins.",
    L"Il y a trop peu de volontaires pour l'entraînement.",
    L"%s ne peut pas former plus de miliciens.",
    L"Le mercenaire a un contrat déterminé.",
    L"Ce mercenaire n'est pas assuré.",
};

STR16 pLandMarkInSectorString[] = {
    L"L'escouade %d a remarqué quelque chose dans le secteur %s",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] = {
    L"L'entraînement de la milice vous coûtera $",  // telling player how much it will cost
    L"Etes-vous d'accord ?",            // asking player if they wish to pay the amount requested
    L"Vous n'en avez pas les moyens.",  // telling the player they can't afford to train this town
    L"Voulez-vous poursuivre l'entraînement de la milice à %s (%s %d) ?",  // continue training this
                                                                           // town?
    L"Coût $",  // the cost in dollars to train militia
    L"(O/N)",   // abbreviated oui/non
    L"",        // unused
    L"L'entraînement des milices dans %d secteurs vous coûtera %d $. %s",  // cost to train sveral
                                                                           // sectors at once
    L"Vous ne pouvez pas payer les %d $ nécessaires à l'entraînement.",
    L"Vous ne pouvez poursuivre l'entraînement de la milice à %s que si cette ville est à niveau "
    L"de loyauté de %d pour-cent.",
    L"Vous ne pouvez plus entraîner de milice à %s.",
};

#ifdef JA2DEMOADS
STR16 gpDemoString[] = {
    // 0-9
    L"LE MEILLEUR DE LA STRATEGIE.",
    L"LE MEILLEUR DU JEU DE ROLES.",
    L"Une intrigue évolutive",
    L"Un scénario complexe",
    L"Des combats dantesques",
    L"Eclairage dynamique",
    L"Interaction avec les autres personnages",
    L"Des tactiques évoluées (attaque de nuit, furtivité)",
    L"Créez votre propre mercenaire",
    L"Des quêtes passionnantes",
    L"Des véhicules à conduire",
    // 10-19
    L"Plus de 150 personnages",
    L"Des centaines d'objets",
    L"Plus de 250 secteurs différents",
    L"Des explosions ébouriffantes",
    L"De nouvelles manœuvres tactiques",
    L"Des tonnes d'animation",
    L"Plus de 9000 lignes de dialogue",
    L"Une superbe bande originale de Kevin Manthei",
    L"Une stratégie globale",
    L"Des raids aériens",
    // 20-29
    L"Entraînez votre propre milice",
    L"Plusieurs types de jeu disponibles",
    L"Des champs de vision réalistes",
    L"Des graphismes de haut niveau",
    L"Achetez, vendez et échangez",
    L"Une IA sans pitié",
    L"Le meilleur du combat tactique",
    L"Option SF",
    L"Une horloge de 24 heures",
    L"Bien plus de 60 heures de jeu",
    // 30-34
    L"Plusieurs niveaux de difficulté",
    L"Des modèles balistiques étudiés",
    L"Un moteur puissant",
    L"Un environnement interactif",
    L"Et tellement plus...",
    // 35 on are BOLD
    L"",
    L"",
    L"\"L'un des jeux les plus prenants de cette fin de siècle\"",
    L"PC Gamer",
    L"Commandez-le dès maintenant sur WWW.JAGGEDALLIANCE2.COM !",
};

STR16 gpDemoIntroString[] = {
    L"Vous êtes sur le point de goûter au meilleur de la stratégie, du jeu de rôles et du combat "
    L"tactique :",
    // Point 1 (uses one string)
    L"Contrôlez une équipe de mercenaires possédant tous leur personnalité (ici, pas d'unités "
    L"identiques à l'infini).",
    // Point 2 (uses one string)
    L"Utilisez toutes les ficelles du combat tactique, faites-les courir, ramper, grimper et "
    L"sauter, se battre au corps-à-corps et bien plus encore.",
    // Point 3 (uses one string)
    L"Jouez avec le feu ! Des gadgets cools, des armes dévastatrices, des explosions "
    L"ébouriffantes.",
    // Additional comment
    L"(et ce n'est qu'un début)",
    // Introduction/instructions
    L"Bienvenue à Demoville... (appuyez sur une touche)",
};
#endif

// Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom
// of the single merc panel
STR16 gzMoneyWithdrawMessageText[] = {
    L"Vous ne pouvez retirer que 20 000 $ à la fois.",
    L"Etes-vous sûr de vouloir déposer %s sur votre compte ?",
};

STR16 gzCopyrightText[] = {
    L"Copyright (C) 1999 Sir-tech Canada Ltd. Tous droits réservés.",
};

// option Text
STR16 zOptionsToggleText[] = {
    L"Dialogue",
    L"Confirmations muettes",
    L"Sous-titres",
    L"Pause des dialogues",
    L"Animation fumée",
    L"Du sang et des tripes",
    L"Ne pas toucher à ma souris !",
    L"Ancienne méthode de sélection",
    L"Afficher chemin",
    L"Afficher tirs manqués",
    L"Confirmation temps réel",
    L"Afficher notifications sommeil/réveil",
    L"Système métrique",
    L"Mercenaire éclairé lors des mouvements",
    L"Figer curseur sur les mercenaires",
    L"Figer curseur sur les portes",
    L"Objets en surbrillance",
    L"Afficher cimes",
    L"Affichage fil de fer",
    L"Curseur 3D",
};

// This is the help text associated with the above toggles.
STR16 zOptionsScreenHelpText[] = {
    // speech
    L"Activez cette option pour entendre vos mercenaires lorsqu'ils parlent.",

    // Mute Confirmation
    L"Active/désactive les confirmations des mercenaires.",

    // Subtitles
    L"Affichage des sous-titres à l'écran.",

    // Key to advance speech
    L"Si les sous-titres s'affichent à l'écran, cette option vous permet de prendre le temps de "
    L"les lire.",

    // Toggle smoke animation
    L"Désactivez cette option si votre machine n'est pas suffisamment puissante.",

    // Blood n Gore
    L"Désactivez cette option si le jeu vous paraît trop violent.",

    // Never move my mouse
    L"Activez cette option pour que le curseur ne se place pas automatiquement sur les boutons qui "
    L"s'affichent.",

    // Old selection method
    L"Activez cette option pour retrouver vos automatismes de la version précédente.",

    // Show movement path
    L"Activez cette option pour afficher le chemin suivi par les mercenaires. Vous pouvez la "
    L"désactiver et utiliser la touche MAJ en cours de jeu.",

    // show misses
    L"Activez cette option pour voir où atterrissent tous vos tirs.",

    // Real Time Confirmation
    L"Activez cette option pour afficher une confirmation de mouvement en temps réel.",

    // Sleep/Wake notification
    L"Activez cette option pour être mis au courant de l'état de veille de vos mercenaires.",

    // Use the metric system
    L"Activez cette option pour que le jeu utilise le système métrique.",

    // Merc Lighted movement
    L"Activez cette option pour éclairer les environs des mercenaires. Désactivez-le si votre "
    L"machine n'est pas suffisamment puissante.",

    // Smart cursor
    L"Activez cette option pour que le curseur se positionne directement sur un mercenaire quand "
    L"il est à proximité.",

    // snap cursor to the door
    L"Activez cette option pour que le curseur se positionne directement sur une porte quand il "
    L"est à proximité.",

    // glow items
    L"Activez cette option pour mettre les objets en évidence",

    // toggle tree tops
    L"Activez cette option pour afficher le cime des arbres.",

    // toggle wireframe
    L"Activez cette option pour afficher les murs en fil de fer.",

    L"Activez cette option pour afficher le curseur en 3D. ( |Home )",

};

STR16 gzGIOScreenText[] = {
    L"CONFIGURATION DU JEU",
    L"Style de jeu",
    L"Réaliste",
    L"SF",
    L"Armes",
    L"Beaucoup",
    L"Peu",
    L"Difficulté",
    L"Novice",
    L"Expérimenté",
    L"Expert",
    L"Ok",
    L"Annuler",
    L"En combat",
    L"Temps illimité",
    L"Temps limité",
    L"Désactivé pour la démo",
};

STR16 pDeliveryLocationStrings[] = {
    L"Austin",       // Austin, Texas, USA
    L"Bagdad",       // Baghdad, Iraq (Suddam Hussein's home)
    L"Drassen",      // The main place in JA2 that you can receive items.  The other towns are dummy
                     // names...
    L"Hong Kong",    // Hong Kong, Hong Kong
    L"Beyrouth",     // Beirut, Lebanon	(Middle East)
    L"Londres",      // London, England
    L"Los Angeles",  // Los Angeles, California, USA (SW corner of USA)
    L"Meduna",       // Meduna -- the other airport in JA2 that you can receive items.
    L"Metavira",     // The island of Metavira was the fictional location used by JA1
    L"Miami",        // Miami, Florida, USA (SE corner of USA)
    L"Moscou",       // Moscow, USSR
    L"New-York",     // New York, New York, USA
    L"Ottawa",       // Ottawa, Ontario, Canada -- where JA2 was made!
    L"Paris",        // Paris, France
    L"Tripoli",      // Tripoli, Libya (eastern Mediterranean)
    L"Tokyo",        // Tokyo, Japan
    L"Vancouver",    // Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] = {
    // This string is used in the IMP character generation.  It is possible to select 0 ability
    // in a skill meaning you can't use it.  This text is confirmation to the player.
    L"Etes-vous sûr de vous ? Une valeur de ZERO signifie que vous serez INCAPABLE d'utiliser "
    L"cette compétence.",
};

STR16 pIMPBeginScreenStrings[] = {
    L"( 8 Caractères Max )",
};

STR16 pIMPFinishButtonText[1] = {
    L"Analyse",
};

STR16 pIMPFinishStrings[] = {
    L"Nous vous remercions, %s",  //%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] = {
    L"Voix",
};

STR16 pDepartedMercPortraitStrings[] = {
    L"Mort",
    L"Renvoyé",
    L"Autre",
};

// title for program
STR16 pPersTitleText[] = {
    L"Personnel",
};

// paused game strings
STR16 pPausedGameText[] = {
    L"Pause",
    L"Reprendre (|P|a|u|s|e)",
    L"Pause (|P|a|u|s|e)",
};

STR16 pMessageStrings[] = {
    L"Quitter la partie ?",
    L"OK",
    L"OUI",
    L"NON",
    L"Annuler",
    L"CONTRAT",
    L"MENT",
    L"Sans description",  // Save slots that don't have a description.
    L"Partie sauvegardée.",
    L"Partie sauvegardée.",
    L"Sauvegarde rapide",  // The name of the quicksave file (filename, text reference)
    L"Partie",  // The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
    L"sav",     // The 3 character dos extension (represents sav)
    L"..\\SavedGames",  // The name of the directory where games are saved.
    L"Jour",
    L"Mercs",
    L"Vide",     // An empty save game slot
    L"Démo",     // Demo of JA2
    L"Debug",    // State of development of a project (JA2) that is a debug build
    L"Version",  // Release build for JA2
    L"bpm",  // Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
    L"min",  // Abbreviation for minute.
    L"m",    // One character abbreviation for meter (metric distance measurement unit).
    L"balles",       // Abbreviation for rounds (# of bullets)
    L"kg",           // Abbreviation for kilogram (metric weight measurement unit)
    L"lb",           // Abbreviation for pounds (Imperial weight measurement unit)
    L"Home",         // Home as in homepage on the internet.
    L"USD",          // Abbreviation to US dollars
    L"n/a",          // Lowercase acronym for not applicable.
    L"Entre-temps",  // Meanwhile
    L"%s est arrivé dans le secteur %s%s",  // Name/Squad has arrived in sector A9.  Order must not
                                            // change without notifying SirTech
    L"Version",
    L"Emplacement de sauvegarde rapide vide",
    L"Cet emplacement est réservé aux sauvegardes rapides effectuées depuis l'écran tactique "
    L"(ALT+S).",
    L"Ouverte",
    L"Fermée",
#ifdef JA2DEMO
    L"Il n'y a plus rien à faire ici. Allez donc voir Gabby.",
    L"Dommage de l'avoir tué...",
#endif
    L"Espace disque insuffisant. Il ne vous reste que %s Mo de libre et Jagged Alliance 2 "
    L"nécessite %s Mo.",
    L"%s embauché(e) sur le site AIM",
    L"%s prend %s.",  //'Merc name' has caught 'item' -- let SirTech know if name comes after item.
    L"%s a pris la drogue.",                //'Merc name' has taken the drug
    L"%s n'a aucune compétence médicale.",  //'Merc name' has non medical skill.

    // CDRom errors (such as ejecting CD while attempting to read the CD)
    L"L'intégrité du jeu n'est plus assurée.",
    L"ERREUR : CD-ROM manquant",

    // When firing heavier weapons in close quarters, you may not have enough room to do so.
    L"Pas assez de place !",

    // Can't change stance due to objects in the way...
    L"Impossible de changer de position ici.",

    // Simple text indications that appear in the game, when the merc can do one of these things.
    L"Lâcher",
    L"Lancer",
    L"Donner",

    L"%s donné à %s.",  //"Item" passed to "merc".  Please try to keep the item %s before the merc
                        //%s, otherwise, must notify SirTech.
    L"Impossible de donner %s à %s.",  // pass "item" to "merc".  Same instructions as above.

    // A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
    L" combiné )",

    // Cheat modes
    L"Triche niveau 1",
    L"Triche niveau 2",

    // Toggling various stealth modes
    L"Escouade en mode furtif.",
    L"Escouade en mode normal.",
    L"%s en mode furtif.",
    L"%s en mode normal.",

    // Wireframes are shown through buildings to reveal doors and windows that can't otherwise be
    // seen in an isometric engine.  You can toggle this mode freely in the game.
    L"Fil de fer activé",
    L"Fil de fer désactivé",

    // These are used in the cheat modes for changing levels in the game.  Going from a basement
    // level to an upper level, etc.
    L"Impossible de remonter...",
    L"Pas de niveau inférieur...",
    L"Entrée dans le sous-sol %d...",
    L"Sortie du sous-sol...",

#ifdef JA2DEMO

    // For the demo, the sector exit interface, you'll be able to split your teams up, but the demo
    // has this feature disabled.  This string is fast help text that appears over "single" button.
    L"Impossible de séparer l'équipe dans la démo.",

    // The overhead map is a map of the entire sector, which you can go into anytime, except in the
    // demo.
    L"Carte désactivée dans la démo.",

#endif

    L"'s",  // used in the shop keeper inteface to mark the ownership of the item eg Red's gun
    L"Mode poursuite désactivé.",
    L"Mode poursuite activé.",
    L"Curseur 3D désactivé.",
    L"Curseur 3D activé.",
    L"Escouade %d active.",
    L"Vous ne pouvez pas payer le salaire de %s qui se monte à %s",  // first %s is the mercs name,
                                                                     // the seconds is a string
                                                                     // containing the salary
    L"Passer",
    L"%s ne peut sortir seul.",
    L"Une sauvegarde a été crée (Partie99.sav). Renommez-la (Partie01 - Partie10) pour pouvoir la "
    L"charger ultérieurement.",
    L"%s a bu %s",
    L"Un colis vient d'arriver à Drassen.",
    L"%s devrait arriver au point d'entrée (secteur %s) en jour %d vers %s.",  // first %s is mercs
                                                                               // name, next is the
                                                                               // sector location
                                                                               // and name where
                                                                               // they will be
                                                                               // arriving in,
                                                                               // lastely is the day
                                                                               // an the time of
                                                                               // arrival
    L"Historique mis à jour.",
#ifdef JA2BETAVERSION
    L"Partie enregistrée dans l'emplacement de sauvegarde automatique.",
#endif
};

UINT16 ItemPickupHelpPopup[][40] = {
    L"OK", L"Défilement haut", L"Tout sélectionner", L"Défilement bas", L"Annuler",
};

STR16 pDoctorWarningString[] = {
    L"%s est trop loin pour être soigné.",
    L"Impossible de soigner tout le monde.",
};

STR16 pMilitiaButtonsHelpText[] = {
    L"Prendre (Clic droit)/poser (Clic gauche) Miliciens",  // button help text informing player
                                                            // they can pick up or drop militia with
                                                            // this button
    L"Prendre (Clic droit)/poser (Clic gauche) Soldats",
    L"Prendre (Clic droit)/poser (Clic gauche) Vétérans",
    L"Répartition automatique",
};

STR16 pMapScreenJustStartedHelpText[] = {
    L"Allez sur le site de l'AIM et engagez des mercenaires ( *Truc* allez voir dans le Poste de "
    L"travail)",  // to inform the player to hired some mercs to get things going
    L"Cliquez sur le bouton de Compression du temps pour faire avancer votre équipe sur le "
    L"terrain.",  // to inform the player to hit time compression to get the game underway
};

STR16 pAntiHackerString[] = {
    L"Erreur. Fichier manquant ou corrompu. L'application va s'arrêter.",
};

STR16 gzLaptopHelpText[] = {
    // Buttons:
    L"Voir messages",
    L"Consulter les sites Internet",
    L"Consulter les documents attachés",
    L"Lire le compte-rendu",
    L"Afficher les infos de l'équipe",
    L"Afficher les états financiers",
    L"Fermer le Poste de travail",

    // Bottom task bar icons (if they exist):
    L"Vous avez de nouveaux messages",
    L"Vous avez reçu de nouveaux fichiers",

    // Bookmarks:
    L"Association Internationale des Mercenaires",
    L"Bobby Ray : Petits et Gros Calibres",
    L"Institut des Mercenaires Professionnels",
    L"Mouvement pour l'Entraînement et le Recrutement des Commandos",
    L"Morgue McGillicutty",
    L"Service des Fleuristes Associés",
    L"Courtiers d'Assurance des Mercenaires de l'A.I.M.",
};

STR16 gzHelpScreenText[] = {
    L"Quitter l'écran d'aide",
};

STR16 gzNonPersistantPBIText[] = {
    L"Vous êtes en plein combat. Vous pouvez donner l'ordre de retraite depuis l'écran tactique.",
    L"|Pénétrez dans le secteur pour reprendre le cours du combat.",
    L"|Résolution automatique du combat.",
    L"Résolution automatique impossible lorsque vous êtes l'attaquant.",
    L"Résolution automatique impossible lorsque vous êtes pris en embuscade.",
    L"Résolution automatique impossible lorsque vous combattez des créatures dans les mines.",
    L"Résolution automatique impossible en présence de civils hostiles.",
    L"Résolution automatique impossible en présence de chats sauvages.",
    L"COMBAT EN COURS",
    L"Retraite impossible.",
};

STR16 gzMiscString[] = {
    L"Votre milice continue le combat sans vos mercenaires...",
    L"Ce véhicule n'a plus besoin de carburant pour le moment.",
    L"Le réservoir est plein à %d%%.",
    L"L'armée de Deidranna a repris le contrôle de %s.",
    L"Vous avez perdu un site de ravitaillement.",
};

STR16 gzIntroScreen[] = {
    L"Vidéo d'introduction introuvable",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] = {
    L"%s entend un bruit de %s %s.",   L"%s entend un bruit %s de MOUVEMENT %s.",
    L"%s entend un GRINCEMENT %s %s.", L"%s entend un CLAPOTIS %s %s.",
    L"%s entend un IMPACT %s %s.",     L"%s entend une EXPLOSION %s %s.",
    L"%s entend un CRI %s %s.",        L"%s entend un IMPACT %s %s.",
    L"%s entend un IMPACT %s %s.",     L"%s entend un BRUIT %s %s.",
    L"%s entend un BRUIT %s %s.",
};

STR16 wMapScreenSortButtonHelpText[] = {
    L"Tri par nom (|F|1)",  L"Tri par affectation (|F|2)", L"Tri par état de veille (|F|3)",
    L"Tri par lieu (|F|4)", L"Tri par destination (|F|5)", L"Tri par date de départ (|F|6)",
};

STR16 BrokenLinkText[] = {
    L"Erreur 404",
    L"Site introuvable.",
};

STR16 gzBobbyRShipmentText[] = {
    L"Derniers envois",
    L"Commande #",
    L"Quantité d'objets",
    L"Commandé",
};

STR16 gzCreditNames[] = {
    L"Chris Camfield",
    L"Shaun Lyng",
    L"Kris Märnes",
    L"Ian Currie",
    L"Linda Currie",
    L"Eric \"WTF\" Cheng",
    L"Lynn Holowka",
    L"Norman \"NRG\" Olsen",
    L"George Brooks",
    L"Andrew Stacey",
    L"Scot Loving",
    L"Andrew \"Big Cheese\" Emmons",
    L"Dave \"The Feral\" French",
    L"Alex Meduna",
    L"Joey \"Joeker\" Whelan",
};

STR16 gzCreditNameTitle[] = {
    L"Programmeur",                          // Chris Camfield
    L"Co-designer/Ecrivain",                 // Shaun Lyng
    L"Systèmes stratégiques & Programmeur",  // Kris Marnes
    L"Producteur/Co-designer",               // Ian Currie
    L"Co-designer/Conception des cartes",    // Linda Currie
    L"Artiste",                              // Eric \"WTF\" Cheng
    L"Coordination, Assistance",             // Lynn Holowka
    L"Artiste Extraordinaire",               // Norman \"NRG\" Olsen
    L"Gourou du son",                        // George Brooks
    L"Conception écrans/Artiste",            // Andrew Stacey
    L"Artiste en chef/Animateur",            // Scot Loving
    L"Programmeur en chef",                  // Andrew \"Big Cheese Doddle\" Emmons
    L"Programmeur",                          // Dave French
    L"Systèmes stratégiques & Programmeur",  // Alex Meduna
    L"Portraits",                            // Joey \"Joeker\" Whelan",
};

STR16 gzCreditNameFunny[] = {
    L"",                                                    // Chris Camfield
    L"(ah, la ponctuation...)",                             // Shaun Lyng
    L"(\"C'est bon, trois fois rien\")",                    // Kris \"The Cow Rape Man\" Marnes
    L"(j'ai passé l'âge)",                                  // Ian Currie
    L"(et en plus je bosse sur Wizardry 8)",                // Linda Currie
    L"(on m'a forcé !)",                                    // Eric \"WTF\" Cheng
    L"(partie en cours de route...)",                       // Lynn Holowka
    L"",                                                    // Norman \"NRG\" Olsen
    L"",                                                    // George Brooks
    L"(Tête de mort et fou de jazz)",                       // Andrew Stacey
    L"(en fait il s'appelle Robert)",                       // Scot Loving
    L"(la seule personne un peu responsable de l'équipe)",  // Andrew \"Big Cheese Doddle\" Emmons
    L"(bon, je vais pouvoir réparer ma moto)",              // Dave French
    L"(piqué à l'équipe de Wizardry 8)",                    // Alex Meduna
    L"(conception des objets et des écrans de chargement !)",  // Joey \"Joeker\" Whelan",
};

STR16 sRepairsDoneString[] = {
    L"%s a terminé la réparation de ses objets",
    L"%s a terminé la réparation des armes & armures",
    L"%s a terminé la réparation des objets portés",
    L"%s a terminé la réparation des objets transportés",
};

STR16 zGioDifConfirmText[] = {
    L"Vous avez choisi le mode de difficulté NOVICE. Ce mode de jeu est conseillé pour les joueurs "
    L"qui découvrent Jagged Alliance, qui n'ont pas l'habitude de jouer à des jeux de stratégie ou "
    L"qui souhaitent que les combats ne durent pas trop longtemps. Ce choix influe sur de nombreux "
    L"paramètres du jeu. Etes-vous certain de vouloir jouer en mode Novice ?",
    L"Vous avez choisi le mode de difficulté EXPERIMENTE. Ce mode de jeu est conseillé pour les "
    L"joueurs qui ont déjà joué à Jagged Alliance ou des jeux de stratégie. Ce choix influe sur de "
    L"nombreux paramètres du jeu. Etes-vous certain de vouloir jouer en mode Expérimenté ?",
    L"Vous avez choisi le mode de difficulté EXPERT. Vous aurez été prévenu. Ne venez pas vous "
    L"plaindre si vos mercenaires quittent Arulco dans un cerceuil. Ce choix influe sur de "
    L"nombreux paramètres du jeu. Etes-vous certain de vouloir jouer en mode Expert ?",
};

STR16 gzLateLocalizedString[] = {
    L"Données de l'écran de chargement de %S introuvables...",

    // 1-5
    L"Le robot ne peut quitter ce secteur par lui-même.",

    // This message comes up if you have pending bombs waiting to explode in tactical.
    L"Compression du temps impossible. C'est bientôt le feu d'artifice !",

    //'Name' refuses to move.
    L"%s refuse d'avancer.",

    //%s a merc name
    L"%s n'a pas assez d'énergie pour changer de position.",

    // A message that pops up when a vehicle runs out of gas.
    L"Le %s n'a plus de carburant ; le véhicule est bloqué à %c%d.",

    // 6-10

    // the following two strings are combined with the pNewNoise[] strings above to report noises
    // heard above or below the merc
    L"au-dessus",
    L"en-dessous",

    // The following strings are used in autoresolve for autobandaging related feedback.
    L"Aucun de vos mercenaires n'a de compétence médicale.",
    L"Plus de bandages !",
    L"Pas assez de bandages pour soigner tout le monde.",
    L"Aucun de vos mercenaires n'a besoin de soins.",
    L"Soins automatiques.",
    L"Tous vos mercenaires ont été soignés.",

    // 14
    L"Arulco",

    L"(roof)",

    L"Santé : %d/%d",

    // In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
    //"vs." is the abbreviation of versus.
    L"%d contre %d",

    L"Plus de place dans le %s !",  //(ex "The ice cream truck is full")

    L"%s requiert des soins bien plus importants et/ou du repos.",

    // 20
    // Happens when you get shot in the legs, and you fall down.
    L"%s a été touché aux jambes ! Il ne peut plus tenir debout !",
    // Name can't speak right now.
    L"%s ne peut pas parler pour le moment.",

    // 22-24 plural versions
    L"%d miliciens ont été promus vétérans.",
    L"%d miliciens ont été promus soldats.",
    L"%d soldats ont été promus vétérans.",

    // 25
    L"Echanger",

    // 26
    // Name has gone psycho -- when the game forces the player into burstmode (certain unstable
    // characters)
    L"%s est devenu fou !",

    // 27-28
    // Messages why a player can't time compress.
    L"Nous vous déconseillons d'utiliser la Compression du temps ; vous avez des mercenaires dans "
    L"le secteur %s.",
    L"Nous vous déconseillons d'utiliser la Compression du temps lorsque vos mercenaires se "
    L"trouvent dans des mines infestées de créatures.",

    // 29-31 singular versions
    L"1 milicien a été promu vétéran.",
    L"1 milicien a été promu soldat.",
    L"1 soldat a été promu vétéran.",

    // 32-34
    L"%s ne dit rien.",
    L"Revenir à la surface ?",
    L"(Escouade %d)",

    // 35
    // Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope,
    // Scope before MP5K)
    L"%s a réparé pour %s : %s",  // inverted order !!! Red has repaired the MP5 of Scope

    // 36
    L"Chat Sauvage",

    // 37-38 "Name trips and falls"
    L"%s trébuche et tombe",
    L"Cet objet ne peut être pris d'ici.",

    // 39
    L"Il ne vous reste aucun mercenaire en état de se battre. La milice combattra les créatures "
    L"seule.",

    // 40-43
    //%s is the name of merc.
    L"%s n'a plus de trousse de soins !",
    L"%s n'a aucune compétence médicale !",
    L"%s n'a plus de trousse à outils !",
    L"%s n'a aucune compétence technique !",

    // 44-45
    L"Temps de réparation",
    L"%s ne peut pas voir cette personne.",

    // 46-48
    L"Le prolongateur de %s est tombé !",
    L"Seuls %d instructeurs de milice peuvent travailler par secteur.",
    L"Etes-vous sûr ?",

    // 49-50
    L"Compression du temps",
    L"Le réservoir est plein.",

    // 51-52 Fast help text in mapscreen.
    L"Compression du temps (|E|s|p|a|c|e)",
    L"Arrêt de la Compression du temps (|E|c|h|a|p)",

    // 53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11"
    L"%s a désenrayé le %s",
    L"%s a désenrayé le %s de %s",  // inverted !!! magic has unjammed the g11 of raven

    // 55
    L"Compression du temps impossible dans l'écran d'inventaire.",

    L"Le CD Play de Jagged Alliance 2 est introuvable. L'application va se terminer.",

    L"Objets associés.",

    // 58
    // Displayed with the version information when cheats are enabled.
    L"Actuel/Maximum : %d%%/%d%%",

    // 59
    L"Escorter John et Mary ?",

    L"Interrupteur activé.",
};

#endif  // FRENCH
