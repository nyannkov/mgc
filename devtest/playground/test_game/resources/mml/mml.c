#include "mgc/mgc.h"
#include "mml.h"
#include "bgm/mml_the_flea_waltz.h"
#include "bgm/mml_the_flea_waltz_end.h"


const mgc_mml_record_t se_records[] = {
    { .id = MML_SE_0_MEOW, .mml = "$P-60$B60L4O5F#" },
    { .id = MML_SE_1_LIFE_RECOVER, .mml = "$P60 L64 O6CEG>C" }, 
    { .id = MML_SE_2_LIFE_UP, .mml = "$P60  L64 [4 O6 CEG>C]" }, 
    { .id = MML_SE_3_DAMAGE, .mml = "$E1$A0$H40$D100$S80$F2000" "L8J31~0" }, 
    { .id = MML_SE_4_ATTACK_SCRATCH, .mml = "$P-60  L64 O7C<GEC" }, 
};
const size_t SE_RECORDS_COUNT = countof(se_records);


const mgc_mml_record_t bgm_records[] = {
    { .id = MML_BGM_0_THE_FLEA_WALTZ, .mml = mml_the_flea_waltz },
    { .id = MML_BGM_1_THE_FLEA_WALTZ_END, .mml = mml_the_flea_waltz_end }
};
const size_t BGM_RECORDS_COUNT = countof(bgm_records);

