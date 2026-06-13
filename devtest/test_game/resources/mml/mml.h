#ifndef MGC_MML_H
#define MGC_MML_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include "mgc_drivers/platform/sound/mml_psg/mml_psg.h"

enum {
    MML_SE_0_MEOW = 0,
    MML_SE_1_LIFE_RECOVER,
    MML_SE_2_LIFE_UP,
    MML_SE_3_DAMAGE,
    MML_SE_4_ATTACK_SCRATCH,
    MML_SE_5_QUAKE_1,
    MML_SE_6_QUAKE_2,
    MML_SE_7_QUAKE_3,
    MML_SE_8_OPEN,
    MML_SE_9_SELECT,
    MML_SE_10_OPEN_2,
    MML_SE_11_SELECT_2,
    MML_SE_12_SILENT,
    MML_SE_13_DISCHARGE,
    MML_SE_14_DISCHARGE_2,
    MML_SE_15_DISCHARGE_3,
    MML_SE_16_SPLAAAAAAASH,
    MML_SE_COUNT
};

enum {
    MML_BGM_0_THE_FLEA_WALTZ = 0,
    MML_BGM_1_THE_FLEA_WALTZ_END,
    MML_BGM_2_WATER_FALL,
    MML_BGM_3_STAGE1,
    MML_BGM_4_SHOP,
    MML_BGM_5_BOSS,
    MML_BGM_6_STAGE2,
    MML_BGM_COUNT
};

extern const mgc_mml_record_t se_records[];
extern const size_t SE_RECORDS_COUNT;

extern const mgc_mml_record_t bgm_records[];
extern const size_t BGM_RECORDS_COUNT;

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*MGC_MML_H*/
