/**
 * @file bbb_pwm.h
 * @brief The exported functions and interfaces for the pwm controller and
 * the pwms.
 * @author Travis Lane
 * @version 0.2.0
 * @date 2014-12-25
 */

#ifndef BBB_PWM_H
#define BBB_PWM_H

#include <stdint.h>
#include <stdbool.h>

// Enumeração que simula um tipo booleano para a solicitação de pwm
enum bbb_pwm_state_e
{
    /**
     * @brief The pwm is unclaimed.
     */
    BPS_UNCLAIMED = 0,

    /**
     * @brief The pwm is claimed.
     */
    BPS_CLAIMED = 1
};

// Enumeração de mensagens da aplicação
enum bbb_pwm_return_code_e
{
    BPRC_NOT_IMPLEMENTED = -100,
    BPRC_BAD_WRITE = -59,
    BPRC_NULL_PTR = -58,
    BPRC_BAD_FILE = -57,
    BPRC_RANGE = -56,
    BPRC_NOT_CLAIMED = -55,
    BPRC_BUSY = -54,
    BPRC_PWM_NOT_FOUND = -53,
    BPRC_DUPLICATE = -52,
    BPRC_NO_PWM = -51,
    BPRC_NO_CAPEMGR = -50,
    BPRC_NO_DATA = -4,
    BPRC_NO_MEM = -3,
    BPRC_ERROR = -2,
    BPRC_OK = 0
};

// Structs utilizadas no projeto
struct bbb_pwm_t;
struct bbb_pwm_controller_t;

struct bbb_pwm_controller_t *bbb_pwm_controller_new(); // Função retorna um ponteiro de struct
void bbb_pwm_controller_delete(struct bbb_pwm_controller_t **bpc_ptr);
bool bbb_pwm_controller_has_pwm(struct bbb_pwm_controller_t *bpc, const char *name);
struct bbb_pwm_t* bbb_pwm_controller_get_pwm(struct bbb_pwm_controller_t *bpc, const char *name); // Função retorna um ponteiro de struct

struct bbb_pwm_t* bbb_pwm_controller_get_head_pwm(struct bbb_pwm_controller_t *bpc); // Função retorna um ponteiro de struct
struct bbb_pwm_t* bbb_pwm_get_next_pwm(struct bbb_pwm_t *bp); // Função retorna um ponteiro de struct

int bbb_pwm_claim(struct bbb_pwm_t *bp);
int bbb_pwm_unclaim(struct bbb_pwm_t *bp);

int bbb_pwm_is_unclaimed(struct bbb_pwm_t *bp);
int bbb_pwm_is_claimed(struct bbb_pwm_t *bp);
int bbb_pwm_is_running(struct bbb_pwm_t *bp);

int bbb_pwm_start(struct bbb_pwm_t* bp);
int bbb_pwm_stop(struct bbb_pwm_t* bp);

const char* bbb_pwm_get_name(struct bbb_pwm_t* bp);
const char* bbb_pwm_get_path(struct bbb_pwm_t* bp);

int bbb_pwm_set_duty_cycle(struct bbb_pwm_t *bp, uint32_t duty);
int bbb_pwm_set_period(struct bbb_pwm_t *bp, uint32_t period);
int bbb_pwm_set_polarity(struct bbb_pwm_t *bp, int8_t polarity);
int bbb_pwm_set_duty_percent(struct bbb_pwm_t *bp, float percent);
int bbb_pwm_set_frequency(struct bbb_pwm_t *bp, uint32_t hertz);

int bbb_pwm_get_duty_cycle(struct bbb_pwm_t *bp, uint32_t *out_duty);
int bbb_pwm_get_period(struct bbb_pwm_t *bp, uint32_t *out_period);
int bbb_pwm_get_polarity(struct bbb_pwm_t *bp, int8_t *out_polarity);
int bbb_pwm_get_duty_percent(struct bbb_pwm_t *bp, float *out_percent);
int bbb_pwm_get_frequency(struct bbb_pwm_t *bp, uint32_t *out_hertz);

// Definição de um for específico para a aplicação 
#define foreach_pwm(bp, bpc) \
    for(struct bbb_pwm_t* bp = bbb_pwm_controller_get_head_pwm(bpc); \
            bp != NULL; bp = bbb_pwm_get_next_pwm(bp))

#endif /* BBB_PWM_H */
