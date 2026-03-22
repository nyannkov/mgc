#include "mgc/mgc.h"
#include "mml.h"
#include "bgm/mml_the_flea_waltz.h"
#include "bgm/mml_the_flea_waltz_end.h"
#include "bgm/mml_bgm_water_fall.h"
#include "bgm/mml_bgm_stage1.h"
#include "bgm/mml_bgm_shop.h"


const mgc_mml_record_t se_records[] = {
    { .id = MML_SE_0_MEOW, .mml = "$P-60$B60L4O5F#" },
    { .id = MML_SE_1_LIFE_RECOVER, .mml = "$P60 L64 O6CEG>C" }, 
    { .id = MML_SE_2_LIFE_UP, .mml = "$P60  L64 [4 O6 CEG>C]" }, 
    { .id = MML_SE_3_DAMAGE, .mml = "$E1$A0$H40$D100$S80$F2000" "L8J31~0" }, 
    { .id = MML_SE_4_ATTACK_SCRATCH, .mml = "$P-60  L64 O7C<GEC" }, 
    { .id = MML_SE_5_QUAKE_1, .mml = "V11[0 J0]" }, 
    { .id = MML_SE_6_QUAKE_2, .mml = "V13[0 J15]" }, 
    { .id = MML_SE_7_QUAKE_3, .mml = "V15[0 J31]" }, 
    { .id = MML_SE_8_OPEN, .mml = "V15$E1$A0$H40$D100$S80$F2000 I0H32I31H4" }, 
    { .id = MML_SE_9_SELECT, .mml = "T140L16V13O6 Q2A" },
    { .id = MML_SE_10_OPEN_2, .mml = "T180 V15$E1$A0$H100$D100$S90$F3000 $P-720 O3 A4" }, 
    { .id = MML_SE_11_SELECT_2, .mml = "T140L16V13O7 Q6A64" },
    { .id = MML_SE_12_SILENT, .mml = "R" },
};
const size_t SE_RECORDS_COUNT = countof(se_records);


const mgc_mml_record_t bgm_records[] = {
    { .id = MML_BGM_0_THE_FLEA_WALTZ, .mml = mml_the_flea_waltz },
    { .id = MML_BGM_1_THE_FLEA_WALTZ_END, .mml = mml_the_flea_waltz_end },
    { .id = MML_BGM_2_WATER_FALL, .mml = mml_bgm_water_fall },
    { .id = MML_BGM_3_STAGE1, .mml = mml_bgm_stage1 },
    { .id = MML_BGM_4_SHOP, .mml = mml_bgm_shop },
};
const size_t BGM_RECORDS_COUNT = countof(bgm_records);

