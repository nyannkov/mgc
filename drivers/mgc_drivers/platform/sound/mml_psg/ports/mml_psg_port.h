/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2025 nyannkov
 */
#ifndef MGC_DRIVERS_MML_PSG_PORT_H
#define MGC_DRIVERS_MML_PSG_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

void mml_psg_port__init(float psg_emu_rate, float mml_proc_rate);
void mml_psg_port__deinit(void);
void mml_psg_port__psg_guard_enter(void);
void mml_psg_port__psg_guard_exit(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif/*MGC_DRIVERS_MML_PSG_PORT_H*/
