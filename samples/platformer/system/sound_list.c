#include "sound_list.h"
#include "mgc/sound/psg/sound_psg.h"

static const mgc_mml_record_t bgm_record[NUM_OF_BGM] = {
    {
        .id = BGM_ID_000,
        .mml = "T80I15V6[0 H1]"
    },
};

static const mgc_mml_record_t se_record[NUM_OF_SE] = {
    { .id = SE_ID_000, .mml = "T130V10L16O4$P720Q4A" },
    { .id = SE_ID_001, .mml = "T130V10L32O6Q5C" },
    { .id = SE_ID_002, .mml = "T130V10L32O7Q5C" },
};

void init_sound_lst(void) {
    const mgc_sound_psg_if_t *sound_psg = sound_psg_get_instance();

    sound_psg->set_bgm_list(bgm_record, NUM_OF_BGM);
    sound_psg->set_se_list(se_record, NUM_OF_SE);
}
