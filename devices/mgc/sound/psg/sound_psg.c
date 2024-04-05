#include "sound_psg.h"

#if MGC_SOUND_PSG_TYPE==1
#include "mgc/sound/psg/ymz294/sound_psg_ymz294.h"

const mgc_sound_if_t * ( * const sound_psg_get_instance )(void)
    = sound_psg_ymz294_get_instance;

void ( * const sound_psg_set_bgm_list )(const mgc_mml_record_t *records, size_t count)
    = sound_psg_ymz294_set_bgm_list;

void ( * const sound_psg_set_se_list )(const mgc_mml_record_t *records, size_t count)
    = sound_psg_ymz294_set_se_list;


#elif MGC_SOUND_PSG_TYPE==10
#include "mgc/sound/psg/emulator/sound_psg_emulator.h"

const mgc_sound_if_t * ( * const sound_psg_get_instance )(void)
    = sound_psg_emulator_get_instance;

void ( * const sound_psg_set_bgm_list )(const mgc_mml_record_t *records, size_t count)
    = sound_psg_emulator_set_bgm_list;

void ( * const sound_psg_set_se_list )(const mgc_mml_record_t *records, size_t count)
    = sound_psg_emulator_set_se_list;

#else
#error "No pgs-sound selected."

#endif

