#include "BuildDefines.h"
#include "SGP/FileMan.h"

#ifdef JA2EDITOR

#ifndef _JA2_LOAD_SCREEN_
#define _JA2_LOAD_SCREEN_

typedef struct _FDLG_LIST_TAG {
  GETFILESTRUCT FileInfo;
  struct _FDLG_LIST_TAG *pNext;
  struct _FDLG_LIST_TAG *pPrev;
} FDLG_LIST;

extern FDLG_LIST *AddToFDlgList(FDLG_LIST *pList, GETFILESTRUCT *pInfo);

extern void TrashFDlgList(FDLG_LIST *pList);

extern INT32 iFDlgState;
extern INT32 iFDlgSelectedSlot;

extern BOOLEAN gfAskForName;
extern BOOLEAN gfCreatedFDlg;
extern BOOLEAN gfDestroyFDlg;

UINT32 WaitForFileName(void);
void RemoveFileDialog(void);
void CreateFileDialog(CHAR16 *zTitle);

void SelectFileDialogYPos(UINT16 usRelativeYPos);

void BuildFilenameWithCoordinate();
void BuildCoordinateWithFilename();
BOOLEAN ExtractFilenameFromFields();
BOOLEAN ValidCoordinate();
BOOLEAN ValidFilename();

BOOLEAN ExternalLoadMap(STR16 szFilename);
BOOLEAN ExternalSaveMap(STR16 szFilename);

extern BOOLEAN gfErrorCatch;
extern CHAR16 gzErrorCatchString[256];

#endif
#endif
