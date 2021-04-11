#ifndef __IMP_CONFIRM_H
#define __IMP_CONFIRM_H

void EnterIMPConfirm(void);
void RenderIMPConfirm(void);
void ExitIMPConfirm(void);
void HandleIMPConfirm(void);

BOOLEAN AddCharacterToPlayersTeam(void);
//***1.11.2007***
// void LoadInCurrentImpCharacter( void );
BOOLEAN LoadInCurrentImpCharacter(STR strFilename);

//***13.01.2009*** загрузка координат глаз и губ IMP-персонажей из файла
void LoadIMPEyesMouthXY(void);

void WriteOutCurrentImpCharacter(INT32 iProfileId);

void ResetIMPCharactersEyesAndMouthOffsets(UINT8 ubMercProfileID);

#endif
