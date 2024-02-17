#ifndef MGC_SOUND_H
#define MGC_SOUND_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include "mgc/common/common.h"

typedef uint16_t mgc_sound_id_t;

typedef struct mgc_sound_if {
    int  (*init)(void);
    int  (*play_bgm)(mgc_sound_id_t bgm_id);
    void (*stop_bgm)(void);
    int  (*play_se)(mgc_sound_id_t se_id);
    void (*stop_se)(void);
} mgc_sound_if_t;

#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif/*MGC_SOUND_H*/
