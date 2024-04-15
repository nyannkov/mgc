#include "sound_psg.h"

#if MGC_SOUND_PSG_TYPE==1
#include "mgc/sound/psg/ymz294/sound_psg_ymz294.h"
#define GET_INSTANCE    sound_psg_ymz294_get_instance

#elif MGC_SOUND_PSG_TYPE==10
#include "mgc/sound/psg/emulator/sound_psg_emulator.h"
#define GET_INSTANCE    sound_psg_emulator_get_instance

#else
#error "No pgs-sound selected."

#endif

const mgc_sound_psg_if_t *sound_psg_get_instance(void) {
    return GET_INSTANCE();
}

