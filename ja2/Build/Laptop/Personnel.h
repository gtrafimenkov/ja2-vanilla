#ifndef __PERSONNEL_H
#define __PERSONNEL_H

#include "SGP/Types.h"
#include "Tactical/SoldierControl.h"

// delay for change in ATM mode
#define DELAY_PER_MODE_CHANGE_IN_ATM 2000

void GameInitPersonnel();
void EnterPersonnel();
void ExitPersonnel();
void HandlePersonnel();
void RenderPersonnel();

// add character to:

// leaving for odd reasons
void AddCharacterToOtherList(SOLDIERCLASS *pSoldier);

// killed and removed
void AddCharacterToDeadList(SOLDIERCLASS *pSoldier);

// simply fired...but alive
void AddCharacterToFiredList(SOLDIERCLASS *pSoldier);

// get the total amt of money on this guy
INT32 GetFundsOnMerc(SOLDIERCLASS *pSoldier);

BOOLEAN TransferFundsFromMercToBank(SOLDIERCLASS *pSoldier, INT32 iCurrentBalance);
BOOLEAN TransferFundsFromBankToMerc(SOLDIERCLASS *pSoldier, INT32 iCurrentBalance);

BOOLEAN RemoveNewlyHiredMercFromPersonnelDepartedList(UINT8 ubProfile);

#endif
