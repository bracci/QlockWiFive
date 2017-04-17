/******************************************************************************
   Languages.h
******************************************************************************/

#ifndef LANGUAGES_H
#define LANGUAGES_H

const String sLanguage[] = {
  "DE",   // 00
  "DESW", // 01
  "DEBA", // 02
  "DESA", // 03
  "MKDE", // 04
  "MKSW", // 05
  "MKBA", // 06
  "MKSA", // 07
  "D3",   // 08
  "CH",   // 09
  "CHGS", // 10
  "EN",   // 11
  "ES",   // 12
  "FR",   // 13
  "IT",   // 14
  "NL"    // 15
};

enum eLanguage : uint8_t {
  LANGUAGE_DE_DE,
  LANGUAGE_DE_SW,
  LANGUAGE_DE_BA,
  LANGUAGE_DE_SA,
  LANGUAGE_DE_MKF_DE,
  LANGUAGE_DE_MKF_SW,
  LANGUAGE_DE_MKF_BA,
  LANGUAGE_DE_MKF_SA,
  LANGUAGE_D3,
  LANGUAGE_CH,
  LANGUAGE_CH_GS,
  LANGUAGE_EN,
  LANGUAGE_ES,
  LANGUAGE_FR,
  LANGUAGE_IT,
  LANGUAGE_NL,
  LANGUAGE_COUNT
};

#endif
