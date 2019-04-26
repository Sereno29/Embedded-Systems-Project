/**
 * @file bbb_pwm.c
 * @brief This file contains the source behind the pwm controller
 * and the individual pwms, as well as the file IO necessary for each.
 * @author Travis Lane
 * @version 0.2.0
 * @date 2014-12-25
 */


#define _GNU_SOURCE

#include <libudev.h> // Disponibiza API's (Application Programming Interface) para manipular nodulos de dispositivos e 

#include <sys/file.h>

#include <assert.h> // define uma macro que interrompe o programa caso seu argumento seja falso
#include <fcntl.h>
#include <stdio.h> // FILE
#include <stdint.h> // define tipos de variáveis para a aplicação
#include <stdlib.h> // alocação 
#include <string.h> // funções de manipulação de strings
#include <inttypes.h>
#include <unistd.h>

#include <bbb_pwm_internal.h>

/**
 * @brief Create a new controller.
 *
 * @return A new controller, or NULL on failure.
 */
struct bbb_pwm_controller_t *bbb_pwm_controller_new()
{
  struct bbb_pwm_controller_t *bpc = NULL;

  bpc = calloc(sizeof(struct bbb_pwm_controller_t), 1);
  assert(bpc != NULL); // se o conteúdo entre parênteses for falso, então a execução do programa é parada

  if(bbb_pwm_controller_init(bpc) != BPRC_OK) { // caso a inicialização do controlador não der certo
    bbb_pwm_controller_delete(&bpc); // o controlador é deletado (desalocado)
    goto out;
  }

  bbb_pwm_controller_probe(bpc); // examinando o controlador criado

out:
  return bpc; // retorna o controlador deletado ou examinado, dependendo do resultado da inicialização do controlador
}

/**
 * @brief Free a controller.
 *
 * @param bpc_ptr The controller to free.
 */
void
bbb_pwm_controller_delete(struct bbb_pwm_controller_t **bpc_ptr)
{
  struct bbb_pwm_controller_t *bpc;

  // Check the ptr.
  if(bpc_ptr == NULL) {
    return;
  }

  // Check the referenced ptr.
  bpc = (*bpc_ptr);
  if(bpc == NULL) {
    return;
  }

  if(bpc->bpc_udev != NULL) {
    udev_unref(bpc->bpc_udev); // o que esta função faz? Faz parte da biblioteca libudev
  }

  while(bpc->bpc_head_pwm) {
    // Unclaim before we remove it.
    bbb_pwm_unclaim(bpc->bpc_head_pwm);
    bbb_pwm_controller_remove_pwm(bpc, bpc->bpc_head_pwm->bp_name);
  }

  // Free the origional bpc
  free(bpc);
  (*bpc_ptr) = NULL;
}

/**
 * @brief Initializes a bbb pwm controller.
 *
 * @param bpc The bbb pwm controller to initialize.
 *
 * @return A status code.
 */
int
bbb_pwm_controller_init(struct bbb_pwm_controller_t *bpc)
{
  bpc->bpc_udev = udev_new();
  // Probing can fail and it isn't the end of the world.
  // Maybe emit a warning?
  bbb_pwm_controller_probe(bpc);

  return BPRC_OK;
}

/**
 * @brief Probe the filesystem for obvious PWM devices.
 * NOTE: This hasn't been implemented yet. Sorry!
 * I'll add it in the next month or so. :)
 *
 * @param bpc The controller to probe on.
 *
 * @return BPRC_NOT_IMPLEMENTED
 */
int
bbb_pwm_controller_probe(struct bbb_pwm_controller_t *bpc)
{
  struct udev_list_entry *dev_entry = NULL;

  struct udev_enumerate *enumer = NULL;
  struct udev_list_entry *devs = NULL;

  assert(bpc != NULL);

  // Set up the enumeration
  enumer = udev_enumerate_new(bpc->bpc_udev);
  // Look for the input values
  udev_enumerate_add_match_sysattr(enumer, "driver", "pwm_test");
  // Scan and get the list of items found.
  udev_enumerate_scan_devices(enumer);
  devs = udev_enumerate_get_list_entry(enumer);

  udev_list_entry_foreach(dev_entry, devs) {
    const char *path;
    const char *name;
    struct udev_device *dev = NULL;

    path = udev_list_entry_get_name(dev_entry);
    dev = udev_device_new_from_syspath(bpc->bpc_udev, path);
    name = udev_device_get_property_value (dev, "OF_NAME");

    if(!bbb_pwm_controller_has_pwm(bpc, name)) {
      struct bbb_pwm_t *pwm;
      // Make a new pwm and add it.
      pwm = bbb_pwm_new(name, path);
      bbb_pwm_controller_add_pwm(bpc, pwm);
    }

    udev_device_unref(dev);
  }

  udev_enumerate_unref(enumer);
  return BPRC_OK;
}

/**
 * @brief Adds a PWM to a controller.
 *
 * @param bpc The controller to add to.
 * @param bp The PWM to add.
 *
 * @return A status code.
 */
int
bbb_pwm_controller_add_pwm(struct bbb_pwm_controller_t *bpc,
                           struct bbb_pwm_t *bp)
{
  struct bbb_pwm_t *cur = NULL;
  int result;

  assert(bpc != NULL);
  assert(bp != NULL);

  // Linked list insert in order.
  cur = bpc->bpc_head_pwm;
  if(cur == NULL) {
    // New list.
    bpc->bpc_head_pwm = bp;
    return BPRC_OK;
  }

  // Do our compare.
  result = strcmp(bp->bp_name, cur->bp_name);

  if(result == 0) {
    // Front was duplicate.
    return BPRC_DUPLICATE;
  }

  if(result < 0) {
    // Insert in front.
    bp->bp_next = cur;
    bpc->bpc_head_pwm = bp;
    return BPRC_OK;
  }

  // Not front or dupe, thus we must search!
  while(cur->bp_next != NULL) {
    // Do our compare.
    // Note we compare on the NEXT element.
    result = strcmp(bp->bp_name, cur->bp_next->bp_name);

    if(result == 0) {
      // Found a duplicate item.
      return BPRC_DUPLICATE;
    }

    if(result < 0) {
      // Found our place.
      bp->bp_next = cur->bp_next;
      cur->bp_next = bp;
      return BPRC_OK;
    }
    // This is not the pointer you were looking for.
    cur = cur->bp_next;
  }
  // If hit the end and we still haven't found our place, insert at end.
  cur->bp_next = bp;
  return BPRC_OK;
}

/**
 * @brief Removes a PWM from a controller.
 * Note, the PWM must not be in use.
 *
 * @param bpc The comtroller to remove the pwm from.
 * @param name The name of the PWM to remove.
 *
 * @return A status code.
 */
int
bbb_pwm_controller_remove_pwm(struct bbb_pwm_controller_t *bpc,
                              const char *name)
{
  struct bbb_pwm_t *cur;
  int result;

  assert(bpc != NULL);
  assert(name != NULL);

  // Remove from a sorted list

  cur = bpc->bpc_head_pwm;
  if(cur == NULL) {
    // Empty list.
    return BPRC_PWM_NOT_FOUND;
  }

  // Do our compare
  result = strcmp(cur->bp_name, name);

  if(result == 0) {
    if(!bbb_pwm_is_unclaimed(cur)) {
      // Make sure it isn't locked.
      return BPRC_BUSY;
    }
    // First item was the one to delete.
    bpc->bpc_head_pwm = cur->bp_next;
    bbb_pwm_delete(&cur);
    bpc->bpc_num_pwms--;
    return BPRC_OK;
  }

  if(result > 0) {
    // Item can't possibly be in our list.
    return BPRC_PWM_NOT_FOUND;
  }

  while(cur->bp_next != NULL) {
    // Do our compare.
    result = strcmp(cur->bp_next->bp_name, name);
    if(result == 0) {
      // Found our target.
      struct bbb_pwm_t *tmp = cur->bp_next;

      if(!bbb_pwm_is_unclaimed(cur)) {
        // Make sure it isn't locked.
        return BPRC_BUSY;
      }

      // Remove it from the linked list.
      cur->bp_next = tmp->bp_next;
      bbb_pwm_delete(&tmp);
      bpc->bpc_num_pwms--;

      return BPRC_OK;
    }

    if(result > 0) {
      // Item can't possibly be in our list.
      return BPRC_PWM_NOT_FOUND;
    }
    // Move on.
    cur = cur->bp_next;
    assert(cur != NULL);
  }

  // We are at the end and didn't find anything.
  return BPRC_PWM_NOT_FOUND;
}

/**
 * @brief Does a controller have a specific pwm.
 *
 * @param bpc The controller to query.
 * @param name The name of the pwm to look for.
 *
 * @return True/False was a pwm by that name found.
 */
bool
bbb_pwm_controller_has_pwm(struct bbb_pwm_controller_t *bpc,
                           const char *name)
{
  return bbb_pwm_controller_get_pwm(bpc, name) != NULL;
}

/**
 * @brief Get a specific pwm from the list of pwms.
 *
 * @param bpc The pwm_controller to get the pwm from.
 * @param name The name of the pwm to get.
 *
 * @return A pwm if found, NULL if not found.
 */
struct bbb_pwm_t *
bbb_pwm_controller_get_pwm(struct bbb_pwm_controller_t *bpc,
                           const char *name)
{
  assert(bpc != NULL);
  assert(name != NULL);

  foreach_pwm(bp, bpc) {
    int result = strcmp(bp->bp_name, name);
    if(result == 0) {
      return bp;
    }

    if(result > 0) {
      // Item can't possibly be in our list.
      return NULL;
    }
  }

  return NULL;
}

/**
 * @brief Get the first pwm in the list of pwms.
 *
 * @param bpc The controller to get the pwm from.
 *
 * @return The first pwm in the list or NULL if there are none.
 */
struct bbb_pwm_t *
bbb_pwm_controller_get_head_pwm(struct bbb_pwm_controller_t *bpc)
{
  assert(bpc != NULL);
  return bpc->bpc_head_pwm;
}

/**
 * @brief Creates a new bbb_pwm_t.
 *
 * @param name The name of the new pwm.
 * @param path
 *
 * @return A new pwm or NULL on failure.
 */
struct bbb_pwm_t *
bbb_pwm_new(const char *name, const char *path)
{
  int result;
  struct bbb_pwm_t *bp;

  if(name == NULL || path == NULL) {
    // Fail for bad input.
    return NULL;
  }

  bp = calloc(sizeof(struct bbb_pwm_t), 1);
  assert(bp != NULL);

  // Initially we are unclaimed.
  bp->bp_state = BPS_UNCLAIMED;

  // Copy the name
  bp->bp_name = (char *) strdup(name);
  if(bp->bp_name == NULL) {
    bbb_pwm_delete(&bp);
    goto out;
  }

  // Copy the path
  bp->bp_path = (char *) strdup(path);
  if(bp->bp_path == NULL) {
    bbb_pwm_delete(&bp);
    goto out;
  }

  // Setup our bp->bp_paths.
  result = asprintf(&(bp->bp_running_state_file_path),
                    "%s/%s", bp->bp_path, "run");
  if(result < 0) {
    bbb_pwm_delete(&bp);
    goto out;
  }

  result = asprintf(&(bp->bp_duty_file_path),
                    "%s/%s", bp->bp_path, "duty");
  if(result < 0) {
    bbb_pwm_delete(&bp);
    goto out;
  }

  result = asprintf(&(bp->bp_period_file_path),
                    "%s/%s", bp->bp_path, "period");
  if(result < 0) {
    bbb_pwm_delete(&bp);
    goto out;
  }

  result = asprintf(&(bp->bp_polarity_file_path),
                    "%s/%s", bp->bp_path, "polarity");
  if(result < 0) {
    bbb_pwm_delete(&bp);
    goto out;
  }

out:
  return bp;
}

/**
 * @brief Deletes a PWM.
 * This will unclaim if claimed.
 *
 * @param bp_ptr The pwm to delete.
 */
void
bbb_pwm_delete(struct bbb_pwm_t **bp_ptr)
{
  struct bbb_pwm_t *bp;
  assert(bp_ptr != NULL);

  bp = *bp_ptr;
  if(bp == NULL) {
    return;
  }

  // Unclaim, since we are freeing.
  bbb_pwm_unclaim(bp);

  // Free what has been allocated.
  if(bp->bp_name != NULL) {
    free(bp->bp_name);
  }

	if(bp->bp_path != NULL) {
		free(bp->bp_path);
	}

	if(bp->bp_running_state_file_path != NULL) {
    free(bp->bp_running_state_file_path);
  }

  if(bp->bp_duty_file_path != NULL) {
    free(bp->bp_duty_file_path);
  }

  if(bp->bp_period_file_path != NULL) {
    free(bp->bp_period_file_path);
  }

  if(bp->bp_polarity_file_path != NULL) {
    free(bp->bp_polarity_file_path);
  }

  free(bp);
  *bp_ptr = NULL;
}

/**
 * @brief Claims a pwm for later setting values.
 * If someone else has claimed this pwm, we fail and report BUSY.
 *
 * @param bp The pwm to claim.
 *
 * @return A status code.
 */
int
bbb_pwm_claim(struct bbb_pwm_t *bp)
{
  int result = BPRC_OK;

  assert(bp != NULL);

  if(bbb_pwm_is_claimed(bp)) {
    // We already claimed it.
    return BPRC_OK;
  }

  assert(bp->bp_running_state_file_path != NULL);
  assert(bp->bp_duty_file_path != NULL);
  assert(bp->bp_period_file_path != NULL);
  assert(bp->bp_polarity_file_path != NULL);

  // Open the necessary files.
  bp->bp_running_state_file =
    file_open_and_claim(bp->bp_running_state_file_path, "r+");
  if(bp->bp_running_state_file == NULL) {
    result = BPRC_BUSY;
    goto out;
  }

  bp->bp_duty_file = file_open_and_claim(bp->bp_duty_file_path, "r+");
  if(bp->bp_duty_file == NULL) {
    result = BPRC_BUSY;
    goto out;
  }

  bp->bp_period_file = file_open_and_claim(bp->bp_period_file_path, "r+");
  if(bp->bp_period_file == NULL) {
    result = BPRC_BUSY;
    goto out;
  }

  bp->bp_polarity_file = file_open_and_claim(bp->bp_polarity_file_path, "r+");
  if(bp->bp_polarity_file == NULL) {
    result = BPRC_BUSY;
    goto out;
  }

  // Load the cached values.
  result = file_read_int8(bp->bp_running_state_file, &(bp->bp_running_state));
  if(result != BPRC_OK) {
    goto out;
  }

  result = file_read_uint32(bp->bp_duty_file, &(bp->bp_duty_cycle));
  if(result != BPRC_OK) {
    goto out;
  }

  result = file_read_uint32(bp->bp_period_file, &(bp->bp_period));
  if(result != BPRC_OK) {
    goto out;
  }

  result = file_read_int8(bp->bp_polarity_file, &(bp->bp_polarity));
  if(result != BPRC_OK) {
    goto out;
  }

out:
  if(result != BPRC_OK) {
    // On failure, unclaim will force a cleanup.
    bbb_pwm_unclaim(bp);
  } else {
    bp->bp_state = BPS_CLAIMED;
  }
  return result;
}

/**
 * @brief Unclaims a pwm.
 *
 * @param bp The pwm to unclaim.
 *
 * @return A status code.
 */
int
bbb_pwm_unclaim(struct bbb_pwm_t *bp)
{
  assert(bp != NULL);

  bp->bp_state = BPS_UNCLAIMED;

  // Close all files.
  if(bp->bp_running_state_file != NULL) {
    file_close_and_unclaim(bp->bp_running_state_file);
    bp->bp_running_state_file = NULL;
  }

  if(bp->bp_duty_file != NULL) {
    file_close_and_unclaim(bp->bp_duty_file);
    bp->bp_duty_file = NULL;
  }

  if(bp->bp_period_file != NULL) {
    file_close_and_unclaim(bp->bp_period_file);
    bp->bp_period_file = NULL;
  }

  if(bp->bp_polarity_file != NULL) {
    file_close_and_unclaim(bp->bp_polarity_file);
    bp->bp_polarity_file = NULL;
  }
  return BPRC_OK;
}

/**
 * @brief Check to see if the pwm is unclaimed.
 *
 * @param bp The pwm to check.
 *
 * @return True/False is the pwm unclaimed.
 */
int
bbb_pwm_is_unclaimed(struct bbb_pwm_t *bp)
{
  assert(bp != NULL);
  return bp->bp_state == BPS_UNCLAIMED;
}

/**
 * @brief Check to see if we have claimership of the pwm.
 *
 * @param bp The pwm to check.
 *
 * @return True/False if we have claimership.
 */
int
bbb_pwm_is_claimed(struct bbb_pwm_t *bp)
{
  assert(bp != NULL);
  return bp->bp_state == BPS_CLAIMED;
}

/**
 * @brief Checks to see if the pwm is currently running.
 *
 * @param bp The pwm to check.
 *
 * @return -1 on failure, 0 when not running, 1 when running.
 */
int
bbb_pwm_is_running(struct bbb_pwm_t *bp)
{
  int8_t running_state;

  if(bbb_pwm_get_running_state(bp, &running_state) != BPRC_OK) {
    return -1;
  }

  return running_state ? 1 : 0;
}

/**
 * @brief Get the next pwm in a sorted list of pwms.
 *
 * @param bp The pwm to get the next pwm from.
 *
 * @return NULL if there is no next pwm.
 */
struct bbb_pwm_t *
bbb_pwm_get_next_pwm(struct bbb_pwm_t *bp)
{
  assert(bp != NULL);
  return bp->bp_next;
}

/**
 * @brief Start a pwm.
 *
 * @param bp The pwm to start.
 *
 * @return A status code.
 */
int
bbb_pwm_start(struct bbb_pwm_t *bp)
{
  return bbb_pwm_set_running_state(bp, 1);
}

/**
 * @brief Stop the pwm from running.
 *
 * @param bp The pwm to stop.
 *
 * @return A status code.
 */
int
bbb_pwm_stop(struct bbb_pwm_t *bp)
{
  return bbb_pwm_set_running_state(bp, 0);
}

/**
 * @brief Set the running_state of the pwm.
 * NOTE: The pwm must be claimed to set anything on it.
 *
 * @param bp The pwm to set.
 * @param running_state The running_state to set the pwm to.
 *
 * @return A status code.
 */
int
bbb_pwm_set_running_state(struct bbb_pwm_t *bp, int8_t running_state)
{
  int result;
  assert(bp != NULL);

  if(!bbb_pwm_is_claimed(bp)) {
    return BPRC_NOT_CLAIMED;
  }

  if(running_state < 0 || running_state > 1) {
    return BPRC_RANGE;
  }

  // Write the data.
  result = file_write_int8(bp->bp_running_state_file, running_state);
  if(result != BPRC_OK) {
    return result;
  }

  bp->bp_running_state = running_state;
  return BPRC_OK;
}

/**
 * @brief Set the duty of the pwm.
 * NOTE: The pwm must be claimed to set anything on it.
 *
 * @param bp The pwm to set.
 * @param duty The duty to set the pwm to.
 *
 * @return A status code.
 */
int
bbb_pwm_set_duty_cycle(struct bbb_pwm_t *bp, uint32_t duty_cycle)
{
  uint32_t period;
  int result;
  assert(bp != NULL);

  if(!bbb_pwm_is_claimed(bp)) {
    return BPRC_NOT_CLAIMED;
  }

  result = bbb_pwm_get_period(bp, &period);
  if(result != BPRC_OK)	{
    return result;
  }

  // duty cycle must be less than or equal to period.
  if(duty_cycle > period) {
    return BPRC_RANGE;
  }

  // Write the data.
  result = file_write_uint32(bp->bp_duty_file, duty_cycle);
  if(result != BPRC_OK) {
    return result;
  }

  bp->bp_duty_cycle = duty_cycle;
  return BPRC_OK;
}

/**
 * @brief Set the period of the pwm.
 * NOTE: The pwm must be claimed to set anything on it.
 *
 * @param bp The pwm to set.
 * @param period The period to set it to.
 *
 * @return A status code.
 */
int
bbb_pwm_set_period(struct bbb_pwm_t *bp, uint32_t period)
{
  int result;
  assert(bp != NULL);

  if(!bbb_pwm_is_claimed(bp)) {
    return BPRC_NOT_CLAIMED;
  }

  result = file_write_uint32(bp->bp_period_file, period);
  if(result != BPRC_OK) {
    return result;
  }

  bp->bp_period = period;
  return BPRC_OK;
}

/**
 * @brief Sets the polarity of the pwm.
 * NOTE: The pwm must be claimed to set anything on it.
 *
 * @param bp The pwm to set.
 * @param polarity The polarity to set the pwm to.
 *
 * @return A status code.
 */
int
bbb_pwm_set_polarity(struct bbb_pwm_t *bp, int8_t polarity)
{
  int result;
  assert(bp != NULL);

  if(!bbb_pwm_is_claimed(bp)) {
    return BPRC_NOT_CLAIMED;
  }

  // TODO: Do I need to disable the pwm first?
  // https://www.kernel.org/doc/Documentation/pwm.txt

  if(polarity != -1 && polarity != 1) {
    // TODO Verify these limits.
    return BPRC_RANGE;
  }

  result = file_write_uint32(bp->bp_polarity_file, polarity);
  if(result != BPRC_OK) {
    return result;
  }

  bp->bp_polarity = polarity;
  return BPRC_OK;
}

/**
 * @brief Set the duty percentage or throttle of a pwm.
 *
 * @param bp The pwm to set.
 * @param percent The percentage to set the duty to.
 *
 * @return A status code.
 */
int
bbb_pwm_set_duty_percent(struct bbb_pwm_t *bp, float percent)
{
  uint32_t duty_cycle, period;
  int result;
  assert(bp != NULL);

  if(!bbb_pwm_is_claimed(bp)) {
    return BPRC_NOT_CLAIMED;
  }

  if(percent < 0.0f || percent > 100.0f) {
    return BPRC_RANGE;
  }

  result = bbb_pwm_get_period(bp, &period);
  if(result != BPRC_OK) {
    return result;
  }
  // We need to invert the percentage.
  // 0 Should be FULL STOP 100 should be FULL SPEED
  duty_cycle = (uint32_t)(((float) period) * (percent / 100.0f));

  return bbb_pwm_set_duty_cycle(bp, duty_cycle);
}

/**
 * @brief Set the period frequency of a pwm.
 *
 * @param bp The pwm to set.
 * @param hertz The frequency in herts to set.
 *
 * @return A status code.
 */
int
bbb_pwm_set_frequency(struct bbb_pwm_t *bp, uint32_t hertz)
{
  int result;
  uint32_t period;
  uint32_t duty;

  assert(bp != NULL);

  if(!bbb_pwm_is_claimed(bp)) {
    return BPRC_NOT_CLAIMED;
  }

  // period can't be less than 1
  // rule out divide by zero.
  if(hertz > 1e9 || hertz <= 0) {
    return BPRC_RANGE;
  }

  // Convert hertz to period in nanoseconds.
  period = 1e9 / hertz;

  result = bbb_pwm_get_duty_cycle(bp, &duty);
  if(result != BPRC_OK) {
    return result;
  }

  // Duty can't exceede period.
  // Duty should probably be throttled to zero first!
  // Weird shit might happen :/
  if(period < duty) {
    return BPRC_RANGE;
  }

  // Set the new period.
  return bbb_pwm_set_period(bp, period);
}

/**
 * @brief Get the name of a pwm.
 *
 * @param bp THe pwm to get the name of.
 *
 * @return The name of the pwm.
 */
const char *
bbb_pwm_get_name(struct bbb_pwm_t *bp)
{
  assert(bp != NULL);
  return bp->bp_name;
}

/**
 * @brief Get the path to a pwm.
 *
 * @param bp The pwm to get the path to.
 *
 * @return The path to the pwm.
 */
const char *
bbb_pwm_get_path(struct bbb_pwm_t *bp)
{
  assert(bp != NULL);
  return bp->bp_path;
}


/**
 * @brief Get the current running state of a pwm.
 * If the pwm isn't claimed, we attempt to open the right file for reading.
 * Note that this may fail if someone else owns it.
 *
 * @param bp The pwm to get the running state of.
 * @param[out] out_running_state The running state.
 *
 * @return A statuse code.
 */
int
bbb_pwm_get_running_state(struct bbb_pwm_t *bp, int8_t *out_running_state)
{
  FILE *run_file = NULL;
  int result = BPRC_OK;

  assert(bp != NULL);
  assert(out_running_state != NULL);

  if(bbb_pwm_is_claimed(bp)) {
    *out_running_state = bp->bp_running_state;
    goto out;
  }

  run_file = fopen(bp->bp_running_state_file_path, "r");
  if(run_file == NULL) {
    result = BPRC_BUSY;
    goto out;
  }

  result = file_read_int8(run_file, out_running_state);
out:
  if(run_file != NULL) {
    fclose(run_file);
  }
  return result;
}

/**
 * @brief Gets the duty cycle of a pwm.
 * If the pwm isn't claimed, we attempt to open the right file for reading.
 * Note that this may fail if someone else owns it.
 *
 * @param bp The pwm to read from.
 * @param[out] out_duty The duty cycle read.
 *
 * @return A status code.
 */
int
bbb_pwm_get_duty_cycle(struct bbb_pwm_t *bp, uint32_t *out_duty)
{
  FILE *duty_file = NULL;
  int result = BPRC_OK;

  assert(bp != NULL);
  assert(out_duty != NULL);

  if(bbb_pwm_is_claimed(bp)) {
    // Value was cached.
    *out_duty = bp->bp_duty_cycle;
    goto out;
  }

  duty_file = fopen(bp->bp_duty_file_path, "r");
  if(duty_file == NULL) {
    result = BPRC_BUSY;
    goto out;
  }

  result = file_read_uint32(duty_file, out_duty);
out:
  if(duty_file != NULL) {
    fclose(duty_file);
  }
  return result;
}

/**
 * @brief Gets the period of a pwm.
 * If the pwm isn't claimed, we attempt to open the right file for reading.
 * Note that this may fail if someone else owns it.
 *
 * @param bp The pwm to read from.
 * @param[out] out_period The period read.
 *
 * @return A status code.
 */
int
bbb_pwm_get_period(struct bbb_pwm_t *bp, uint32_t *out_period)
{
  FILE *period_file = NULL;
  int result = BPRC_OK;

  assert(bp != NULL);
  assert(out_period != NULL);

  if(bbb_pwm_is_claimed(bp)) {
    *out_period = bp->bp_period;
    goto out;
  }

  period_file = fopen(bp->bp_period_file_path, "r");
  if(period_file == NULL) {
    result = BPRC_BUSY;
    goto out;
  }

  result = file_read_uint32(period_file, out_period);
out:
  if(period_file != NULL) {
    fclose(period_file);
  }
  return result;
}

/**
 * @brief Gets the polarity of a pwm.
 * If the pwm isn't claimed, we attempt to open the right file for reading.
 * Note that this may fail if someone else owns it.
 *
 * @param bp The pwm to read from.
 * @param[out] out_polarity The polarity read.
 *
 * @return A status code.
 */
int
bbb_pwm_get_polarity(struct bbb_pwm_t *bp, int8_t *out_polarity)
{
  FILE *polarity_file = NULL;
  int result = BPRC_OK;

  assert(bp != NULL);
  assert(out_polarity != NULL);

  if(bbb_pwm_is_claimed(bp)) {
    *out_polarity = bp->bp_polarity;
    goto out;
  }

  polarity_file = fopen(bp->bp_polarity_file_path, "r");
  if(polarity_file == NULL) {
    result = BPRC_BUSY;
    goto out;
  }

  result = file_read_int8(polarity_file, out_polarity);
out:
  if(polarity_file != NULL) {
    fclose(polarity_file);
  }
  return result;
}

/**
 * @brief Get the current duty percent, or throttle from the pwm.
 *
 * @param bp The pwm to read from.
 * @param[out] out_percent The percentage calcualted from
 * the period and duty cycle.
 *
 * @return A status code.
 */
int
bbb_pwm_get_duty_percent(struct bbb_pwm_t *bp, float *out_percent)
{
  uint32_t duty, period;
  int result;

  result = bbb_pwm_get_period(bp, &period);
  if(result != BPRC_OK) {
    return result;
  }

  result = bbb_pwm_get_duty_cycle(bp, &duty);
  if(result != BPRC_OK) {
    return result;
  }

  *out_percent = ((float) duty / (float) period) * 100.0f;
  return BPRC_OK;
}

/**
 * @brief Get the current frequncy of the period in hertz.
 *
 * @param bp The pwm to read from.
 * @param[out] out_hertz The hertz calculated from the period.
 *
 * @return A status code.
 */
int
bbb_pwm_get_frequency(struct bbb_pwm_t *bp, uint32_t *out_hertz)
{
  uint32_t period;
  int result;

  result = bbb_pwm_get_period(bp, &period);
  if(result != BPRC_OK) {
    return result;
  }

  // Convert nanoseconds to hertz.
  *out_hertz = 1e9 / period;
  return BPRC_OK;
}

/**
 * @brief Open a file and then set a lock on it.
 * Fail if we can't get a lock.
 *
 * @param path The path to the file we are getting and locking.
 *
 * @return A FILE* if we could lock and open the file, else NULL.
 */
FILE *
file_open_and_claim(const char *path, const char *mode)
{
  struct flock lock;
  FILE *file;
  int fd;

  file = fopen(path, mode);
  if(file == NULL) {
    // Failed to open the file!
    return NULL;
  }

  fd = fileno(file);
  if(fd < 0) {
    // Failed to get the file descriptor.
    fclose(file);
    return NULL;
  }

  // Lock the whole file.
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;

  // Figure out the lock type.
  if(file_can_write(file)) {
    lock.l_type = F_WRLCK;
  } else {
    lock.l_type = F_RDLCK;
  }

  if(fcntl(fd, F_SETLK, &lock) < 0) {
    // Failed to lock the file.
    fclose(file);
    return NULL;
  }

  return file;
}

/**
 * @brief Close the file and then unlock it.
 *
 * @param file The file to close and then unlock.
 */
void
file_close_and_unclaim(FILE *file)
{
  int fd;
  struct flock unlock;

  if(file == NULL) {
    return;
  }

  fd = fileno(file);
  if(fd < 0) {
    return;
  }

  // Unlock the whole file.
  unlock.l_whence = SEEK_SET;
  unlock.l_start = 0;
  unlock.l_len = 0;

  unlock.l_type = F_UNLCK;

  // Unlock the file (If we had a claim on it.)
  fcntl(fd, F_SETLK, &unlock);

  // Close the file.
  fclose(file);
}

/**
 * @brief Check to see if we can write to a file.
 *
 * @param file The FILE* to check.
 *
 * @return True/False can we write to the file.
 */
int
file_can_write(FILE *file)
{
  int fd, mode;
  if(file == NULL) {
    return 0;
  }

  fd = fileno(file);
  if(fd < 0) {
    return 0;
  }

  mode = fcntl(fd, F_GETFL);
  return ((mode & O_WRONLY) == O_WRONLY) || ((mode & O_RDWR) == O_RDWR);
}

/**
 * @brief Check to see if we can read from a file.
 *
 * @param file THe FILE* to check.
 *
 * @return True/False can we read from a file.
 */
int
file_can_read(FILE *file)
{
  int fd, mode;
  if(file == NULL) {
    return 0;
  }

  fd = fileno(file);
  if(fd < 0) {
    return 0;
  }

  mode = fcntl(fd, F_GETFL);
  return ((mode & O_WRONLY) != O_WRONLY) || ((mode & O_RDWR) == O_RDWR);
}

/**
 * @brief Reads an uint32 from a file.
 *
 * @param file The file to read from.
 * @param[out] out_data The uint32 if successful.
 *
 * @return A status code.
 */
int
file_read_uint32(FILE *file, uint32_t *out_data)
{
  int result;
  if(file == NULL || out_data == NULL) {
    return BPRC_NULL_PTR;
  }
  if(!file_can_read(file)) {
    return BPRC_BAD_FILE;
  }
  // Set to 0
  result = fseek(file, 0, SEEK_SET);
  if(result != 0) {
    return BPRC_BAD_FILE;
  }
  // Read the data.
  result = fscanf(file, "%"PRIu32"", out_data);
  if(result < 0) {
    return BPRC_NO_DATA;
  }
  return BPRC_OK;
}

/**
 * @brief Reads an int8 from a file.
 *
 * @param file The file to read from.
 * @param[out] out_data The int8 if successful.
 *
 * @return A status code.
 */
int
file_read_int8(FILE *file, int8_t *out_data)
{
  int result;
  if(file == NULL || out_data == NULL) {
    return BPRC_NULL_PTR;
  }
  if(!file_can_read(file)) {
    return BPRC_BAD_FILE;
  }
  // Set to 0
  result = fseek(file, 0, SEEK_SET);
  if(result != 0) {
    return BPRC_BAD_FILE;
  }
  // Read the data.
  result = fscanf(file, "%"SCNd8"", out_data);
  if(result < 0) {
    return BPRC_NO_DATA;
  }
  return BPRC_OK;
}

/**
 * @brief Write an uint32 to a file.
 *
 * @param file The file to write to.
 * @param data The uint32 to write.
 *
 * @return A status code.
 */
int
file_write_uint32(FILE *file, uint32_t data)
{
  int result;
  if(file == NULL) {
    return BPRC_BAD_FILE;
  }
  if(!file_can_write(file)) {
    return BPRC_BAD_FILE;
  }
  // Truncate the file.
  if(freopen(NULL, "w+", file) == NULL) {
    return BPRC_BAD_FILE;
  }
  // Write the data
  result = fprintf(file, "%"PRIu32"", data);
  if(result <= 0) {
    return BPRC_BAD_WRITE;
  }
  return BPRC_OK;
}

/**
 * @brief Write an int8 to a file.
 *
 * @param file The file to write to.
 * @param data THe int8 to write.
 *
 * @return A status code.
 */
int
file_write_int8(FILE *file, int8_t data)
{
  int result;
  if(file == NULL) {
    return BPRC_BAD_FILE;
  }
  if(!file_can_write(file)) {
    return BPRC_BAD_FILE;
  }
  // Truncate the file.
  if(freopen(NULL, "w+", file) == NULL) {
    return BPRC_BAD_FILE;
  }
  // Write the data
  result = fprintf(file, "%"PRId8"", data);
  if(result <= 0) {
    return BPRC_BAD_WRITE;
  }
  return BPRC_OK;
}
