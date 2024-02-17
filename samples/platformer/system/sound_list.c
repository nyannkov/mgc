#include "sound_list.h"

const mml_sound_record_t bgm_record[NUM_OF_BGM] = {
    {
        .id = BGM_ID_000,
        .mml = "T80I15V6[0 H1]"
    },
};

const mml_sound_record_t se_record[NUM_OF_SE] = {
    { .id = SE_ID_000, .mml = "T130V10L16O4$P720Q4A" },
    { .id = SE_ID_001, .mml = "T130V10L32O6Q5C" },
    { .id = SE_ID_002, .mml = "T130V10L32O7Q5C" },
};

void init_sound_lst(void) {
    sound_driver_ymz294_rp2040_set_bgm_table(bgm_record, NUM_OF_BGM);
    sound_driver_ymz294_rp2040_set_se_table(se_record, NUM_OF_SE);
}
