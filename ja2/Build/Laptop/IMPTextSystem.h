#ifndef __IMP_TEXT_SYSTEM_H
#define __IMP_TEXT_SYSTEM_H

void LoadAndDisplayIMPText(INT16 sStartX, INT16 sStartY, INT16 sLineLength,
                           INT16 sIMPTextRecordNumber, UINT32 uiFont, UINT8 ubColor,
                           BOOLEAN fShadow, UINT32 uiFlags);
void InitializeImpRecordLengthList(void);
void PrintImpText(void);
void PrintIMPPersonalityQuizQuestionAndAnsers(void);

// buttons text

// extra strings not found in IMP Text Document

enum {
  IMP_HOME_1,
  IMP_HOME_2,
  IMP_HOME_3,
  IMP_HOME_4,
  IMP_HOME_5,
  IMP_HOME_6,
  IMP_HOME_7,
  IMP_HOME_8,
  IMP_HOME_9,
  IMP_HOME_10,
  IMP_ABOUT_US_1,
  IMP_ABOUT_US_2,
  IMP_ABOUT_US_3,
  IMP_ABOUT_US_4,
  IMP_ABOUT_US_5,
  IMP_ABOUT_US_6,
  IMP_ABOUT_US_7,
  IMP_ABOUT_US_8,
  IMP_ABOUT_US_9,
  IMP_ABOUT_US_10,
  IMP_ABOUT_US_11,
  IMP_ABOUT_US_12,
  IMP_MAIN_1,
  IMP_MAIN_2,
  IMP_MAIN_3,
  IMP_MAIN_4,
  IMP_MAIN_5,
  IMP_MAIN_6,
  IMP_MAIN_7,
  IMP_MAIN_8,
  IMP_MAIN_9,
  IMP_BEGIN_1,
  IMP_BEGIN_2,
  IMP_BEGIN_3,
  IMP_BEGIN_4,
  IMP_BEGIN_5,
  IMP_BEGIN_6,
  IMP_BEGIN_7,
  IMP_BEGIN_8,
  IMP_BEGIN_9,
  IMP_BEGIN_10,
  IMP_BEGIN_11,
  IMP_PERS_1,
  IMP_PERS_2,
  IMP_PERS_3,
  IMP_PERS_4,
  IMP_PERS_5,
  IMP_PERS_6,
  IMP_PERS_7,
  IMP_PERS_8,
  IMP_PERS_9,
  IMP_PERS_10,
  IMP_PERS_11,
  IMP_PERS_12,
  IMP_QUESTION_1,
  IMP_PERS_F1 = IMP_QUESTION_1 + 90,
  IMP_PERS_F2,
  IMP_PERS_F3,
  IMP_PERS_F4,
  IMP_PERS_F5,
  IMP_PERS_F6,
  IMP_PERS_F7,
  IMP_PERS_F8,
  IMP_ATTRIB_1,
  IMP_ATTRIB_2,
  IMP_ATTRIB_3,
  IMP_ATTRIB_4,
  IMP_ATTRIB_5,
  IMP_ATTRIB_6,
  IMP_ATTRIB_7,
  IMP_ATTRIB_8,
  IMP_ATTRIB_SA_1,
  IMP_ATTRIB_SA_2,
  IMP_ATTRIB_SA_3,
  IMP_ATTRIB_SA_4,
  IMP_ATTRIB_SA_5,
  IMP_ATTRIB_SA_6,
  IMP_ATTRIB_SA_7,
  IMP_ATTRIB_SA_8,
  IMP_ATTRIB_SA_9,
  IMP_ATTRIB_SA_10,
  IMP_ATTRIB_SA_11,
  IMP_ATTRIB_SA_12,
  IMP_ATTRIB_SA_13,
  IMP_ATTRIB_SA_14,
  IMP_ATTRIB_SA_15,
  IMP_ATTRIB_SA_16,
  IMP_ATTRIB_SA_17,
  IMP_ATTRIB_SA_18,
  IMP_AF_1,
  IMP_AF_2,
  IMP_AF_3,
  IMP_AF_4,
  IMP_AF_5,
  IMP_AF_6,
  IMP_POR_1,
  IMP_POR_2,
  IMP_POR_3,
  IMP_POR_4,
  IMP_POR_5,
  IMP_POR_6,
  IMP_POR_7,
  IMP_VOC_1,
  IMP_VOC_2,
  IMP_VOC_3,
  IMP_VOC_4,
  IMP_VOC_5,
  IMP_VOC_6,
  IMP_VOC_7,
  IMP_FIN_1,
  IMP_FIN_2,
  IMP_FIN_3,
  IMP_FIN_4,
  IMP_FIN_5,
  IMP_FIN_6,
  IMP_FIN_7,
  IMP_FIN_8,
  IMP_FIN_9,
  IMP_FIN_10,
  IMP_CON_1 = IMP_FIN_10 - 1,
  IMP_CON_2,
  IMP_CON_3,

};

#endif
