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
    MML_SE_COUNT
};

enum {
    MML_BGM_0_THE_FLEA_WALTZ = 0,
    MML_BGM_1_THE_FLEA_WALTZ_END,
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
