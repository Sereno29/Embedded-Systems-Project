/**
 * @file bbb_pwm_internal.h
 * @brief Unexported innerworkings of the pwm controller and the pwms.
 * Note: This is really meant for internal use only.
 * @author Travis Lane
 * @version 0.2.0
 * @date 2014-12-25
 */

 // Esta biblioteca não é disponibilizada ao usuário já que são funções para uso interno de outras partes do código 

#ifndef BBB_PWM_INTERNAL_H
#define BBB_PWM_INTERNAL_H

#define _GNU_SOURCE
#include <stdio.h> // inclui esta biblioteca, já que utiliza a variável FILE nas funções e structs definidas

#include "bbb_pwm.h" // inclui esta biblioteca, já que utiliza a struct bbb_pwm_t definida lá

struct udev; // declaração de estrutura definida em uma das bibliotecas do ponto c. libudev.h

/**
 * @brief Represents a detected pwm. Tem a estrutura de uma lista para o próximo pwm
 */
struct bbb_pwm_t
{

	/**
	 * @brief The current state of the pwm.
	 */
	enum bbb_pwm_state_e                    bp_state; // enumeração definida em bbb_pwm.h

	/**
	 * @brief The name of this pwm.
	 */
	char                                   *bp_name; // nome do pwm

	/**
	 * @brief The root path of this pwm.
	 */
	char                                   *bp_path; // endereço absoluto do pwm no sistema. Geralmente dispositivos são pastas no Linux 


	/**
	 * @brief The next PWM in the list of PWMs, NULL signifies the end.
	 */
	struct bbb_pwm_t                       *bp_next; // ponteiro para outra struct do mesmo tipo bbb_pwm_t, pode ser usado para fazer uma lista ou pilha



	/**
	 * @brief The cached pwm run state.
	 */
	int8_t                                  bp_running_state; // variável similar a um char com sinal

	/**
	 * @brief The cached pwm duty cycle.
	 */
	uint32_t                                bp_duty_cycle; // variável similar a um unsigned int

	/**
	 * @brief The cached pwm period (in nanoseconds).
	 */
	uint32_t                                bp_period; // variável similar a um unsigned int

	/**
	 * @brief The cached pwm polarity.
	 */
	int8_t                                  bp_polarity; // variável similar a um char com sinal


	// Abaixo são descritos os nomes de certos arquivos. A fim de criar um pwm, se cria um arquivo na pasta que representa o dispositivo?

	/**
	 * @brief The file path of the run file.
	 */
	char                                   *bp_running_state_file_path; // nome do caminho até o run file

	/**
	 * @brief The file path of the duty file.
	 */
	char                                   *bp_duty_file_path; // nome do caminho até o duty file

	/**
	 * @brief The file path of the period file.
	 */
	char                                   *bp_period_file_path; // nome do caminho até o period file

	/**
	 * @brief The file path of the polarity file.
	 */
	char                                   *bp_polarity_file_path;



	// Arquivos que guardam dados do pwm. Se faz isso por obrigação ou para se ter um histórico das ações?
	/**
	 * @brief The file with the pwm's current running state.
	 */
	FILE                                   *bp_running_state_file;

	/**
	 * @brief The file with the pwm's duty data.
	 */
	FILE                                   *bp_duty_file;

	/**
	 * @brief The file with the pwm's period data.
	 */
	FILE                                   *bp_period_file;

	/**
	 * @brief The file with the pwm's polarity data.
	 */
	FILE                                   *bp_polarity_file;
};

/**
 * @brief A controller enables/disables pwm functionality.
 * It also detects pwms and makes them avaliable for use.
 */
struct bbb_pwm_controller_t
{

	/**
	 * @brief The number of pwms detected.
	 */
	int8_t                                  bpc_num_pwms;

	/**
	 * @brief The individual pwms.
	 */
	struct bbb_pwm_t                       *bpc_head_pwm; // ponteiro que aponta para o pwm na cabeça da lista de pwms

	/**
	 * @brief
	 */
	struct udev                            *bpc_udev; // O que é essa estrutura udev?
};


//Funções utilizadas para manipular dispositivos por pwm e seurs controladores de pwm 
int bbb_pwm_controller_init(struct bbb_pwm_controller_t *bpc);
int bbb_pwm_controller_add_pwm(struct bbb_pwm_controller_t *bpc, struct bbb_pwm_t *bp);
int bbb_pwm_controller_remove_pwm(struct bbb_pwm_controller_t *bpc, const char *name);

int bbb_pwm_controller_probe(struct bbb_pwm_controller_t *bpc);

struct bbb_pwm_t *bbb_pwm_new(const char *name, const char *root_path);
void bbb_pwm_delete(struct bbb_pwm_t **bp_ptr);

int bbb_pwm_set_running_state(struct bbb_pwm_t *bp, int8_t running_state);
int bbb_pwm_get_running_state(struct bbb_pwm_t *bp, int8_t *out_running_state);

int file_read_uint32(FILE *file, uint32_t *out_data);
int file_read_int8(FILE *file, int8_t *out_data);

int file_write_uint32(FILE *file, uint32_t data);
int file_write_int8(FILE *file, int8_t data);

int file_can_write(FILE *file);
int file_can_read(FILE *file);

FILE *file_open_and_claim(const char *path, const char *mode);
void file_close_and_unclaim(FILE *file);

#endif /* BBB_PWM_INTERNAL_H */
