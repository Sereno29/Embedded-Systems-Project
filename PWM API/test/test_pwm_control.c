/**
 * @file test_pwm_control.c
 * @brief Test the pwm controls.
 * @author Travis Lane
 * @version
 * @date 2014-12-22
 */



#include "test_macros.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/bbb_pwm_internal.h"

void test_claim_unclaim();

void test_set_get_running_state();
void test_set_get_duty_cycle();
void test_set_get_period();
void test_set_get_polarity();

void test_set_get_duty_percent();
void test_set_get_frequency();

void test_invalid_set_duty_cycle();
void test_invalid_set_period();
void test_invalid_set_polarity();

void test_invalid_get_duty_cycle();
void test_invalid_get_period();
void test_invalid_get_polarity();

struct bbb_pwm_t *bbb_pwm_test_new(const char *name,
                                   int create_files, int init_files);
void bbb_pwm_test_delete(struct bbb_pwm_t **bp_ptr);

int
main()
{
  test_claim_unclaim();

  test_set_get_duty_cycle();
  test_set_get_period();
  test_set_get_polarity();
  test_set_get_duty_percent();
  test_set_get_frequency();

  test_invalid_set_duty_cycle();
  test_invalid_set_period();
  test_invalid_set_polarity();

  test_invalid_get_duty_cycle();
  test_invalid_get_period();
  test_invalid_get_polarity();

  return 0;
}

/**
 * @brief Test claiming and unclaiming a pwm.
 */
void
test_claim_unclaim()
{
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_claim_unclaim", 1, 1);

  expect_eq(bbb_pwm_claim(bp), BPRC_OK);
  expect_eq(bbb_pwm_unclaim(bp), BPRC_OK);

  expect_eq(bbb_pwm_claim(bp), BPRC_OK);
  expect_eq(bbb_pwm_claim(bp), BPRC_OK);
  expect_eq(bbb_pwm_unclaim(bp), BPRC_OK);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test setting and getting valid running states
 */
void
test_set_get_running_state()
{
  int8_t running_state = 0;
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_set_get_running_state", 1, 1);

  expect_eq(bbb_pwm_claim(bp), BPRC_OK);

  expect_eq(bbb_pwm_set_running_state(bp, 1), BPRC_OK);
  expect_eq(bbb_pwm_get_running_state(bp, &running_state), BPRC_OK);
  expect_eq(running_state, 100);

  expect_eq(bbb_pwm_set_running_state(bp, 0), BPRC_OK);
  expect_eq(bbb_pwm_get_running_state(bp, &running_state), BPRC_OK);
  expect_eq(running_state, 0);

  bbb_pwm_test_delete(&bp);
}


/**
 * @brief Test setting and getting valid duty cycles.
 */
void
test_set_get_duty_cycle()
{
  uint32_t duty = 0;
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_set_get_duty_cycle", 1, 1);

  expect_eq(bbb_pwm_claim(bp), BPRC_OK);

  expect_eq(bbb_pwm_set_duty_cycle(bp, 100), BPRC_OK);
  expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
  expect_eq(duty, 100);

  expect_eq(bbb_pwm_set_duty_cycle(bp, 0), BPRC_OK);
  expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
  expect_eq(duty, 0);

  expect_eq(bbb_pwm_set_duty_cycle(bp, 10), BPRC_OK);
  expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
  expect_eq(duty, 10);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test setting then getting valid periods.
 */
void
test_set_get_period()
{
  uint32_t period = 0;
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_set_get_period", 1, 1);

  expect_eq(bbb_pwm_claim(bp), BPRC_OK);

  expect_eq(bbb_pwm_set_period(bp, 100), BPRC_OK);
  expect_eq(bbb_pwm_get_period(bp, &period), BPRC_OK);
  expect_eq(period, 100);

  expect_eq(bbb_pwm_set_period(bp, 0), BPRC_OK);
  expect_eq(bbb_pwm_get_period(bp, &period), BPRC_OK);
  expect_eq(period, 0);

  expect_eq(bbb_pwm_set_period(bp, 10), BPRC_OK);
  expect_eq(bbb_pwm_get_period(bp, &period), BPRC_OK);
  expect_eq(period, 10);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test setting and getting valid polarities.
 */
void
test_set_get_polarity()
{
  int8_t polarity = 0;
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_set_get_polarity", 1, 1);

  expect_eq(bbb_pwm_claim(bp), BPRC_OK);

  expect_eq(bbb_pwm_set_polarity(bp, 1), BPRC_OK);
  expect_eq(bbb_pwm_get_polarity(bp, &polarity), BPRC_OK);
  expect_eq(polarity, 1);

  expect_eq(bbb_pwm_set_polarity(bp, -1), BPRC_OK);
  expect_eq(bbb_pwm_get_polarity(bp, &polarity), BPRC_OK);
  expect_eq(polarity, -1);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test getting and setting the duty percent.
 */
void
test_set_get_duty_percent()
{
  float percent;
  uint32_t duty;
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_set_get_duty_percent", 1, 1);

  expect_eq(bbb_pwm_claim(bp), BPRC_OK);
  expect_eq(bbb_pwm_set_period(bp, 100), BPRC_OK);

  expect_eq(bbb_pwm_set_duty_percent(bp, 100.0f), BPRC_OK);
  expect_eq(bbb_pwm_get_duty_percent(bp, &percent), BPRC_OK);
  expect_eq(100.0f, percent);
  expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
  expect_eq(100, duty);

  expect_eq(bbb_pwm_set_duty_percent(bp, 50.0f), BPRC_OK);
  expect_eq(bbb_pwm_get_duty_percent(bp, &percent), BPRC_OK);
  expect_eq(50.0f, percent);
  expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
  expect_eq(50, duty);

  expect_eq(bbb_pwm_set_duty_percent(bp, 0.0f), BPRC_OK);
  expect_eq(bbb_pwm_get_duty_percent(bp, &percent), BPRC_OK);
  expect_eq(0.0f, percent);
  expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
  expect_eq(0, duty);


  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test setting and getting the frequency.
 */
void
test_set_get_frequency()
{
  uint32_t period, frequency;
  struct bbb_pwm_t *bp;

  bp = bbb_pwm_test_new("test_set_get_frequency", 1, 1);
  expect_eq(bbb_pwm_claim(bp), BPRC_OK);
  expect_eq(bbb_pwm_set_duty_cycle(bp, 25), BPRC_OK);

  expect_eq(bbb_pwm_set_frequency(bp, 100), BPRC_OK);
  expect_eq(bbb_pwm_get_frequency(bp, &frequency), BPRC_OK);
  expect_eq(100, frequency);
  expect_eq(bbb_pwm_get_period(bp, &period), BPRC_OK);
  expect_eq(period, 10000000);

  expect_eq(bbb_pwm_set_frequency(bp, 200), BPRC_OK);
  expect_eq(bbb_pwm_get_frequency(bp, &frequency), BPRC_OK);
  expect_eq(200, frequency);

  expect_eq(bbb_pwm_set_frequency(bp, 50), BPRC_OK);
  expect_eq(bbb_pwm_get_frequency(bp, &frequency), BPRC_OK);
  expect_eq(50, frequency);

  expect_eq(bbb_pwm_set_frequency(bp, 500), BPRC_OK);
  expect_eq(bbb_pwm_get_frequency(bp, &frequency), BPRC_OK);
  expect_eq(500, frequency);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test setting an invalid duty cycle.
 */
void
test_invalid_set_duty_cycle()
{
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_invalid_set_duty_cycle", 1, 1);

  // Didn't claim.
  expect_neq(bbb_pwm_set_duty_cycle(bp, 10), BPRC_OK);
  expect_eq(bbb_pwm_claim(bp), BPRC_OK);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test setting an invalid period.
 */
void
test_invalid_set_period()
{
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_invalid_set_period", 1, 1);

  // Didn't claim.
  expect_neq(bbb_pwm_set_period(bp, 10), BPRC_OK);
  expect_eq(bbb_pwm_claim(bp), BPRC_OK);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test setting an invalid polarity.
 */
void
test_invalid_set_polarity()
{
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_invalid_set_polarity", 1, 1);

  // Didn't claim.
  expect_neq(bbb_pwm_set_polarity(bp, 10), BPRC_OK);
  expect_eq(bbb_pwm_claim(bp), BPRC_OK);

  // Too small
  expect_neq(bbb_pwm_set_polarity(bp, -2), BPRC_OK);
  // Too big
  expect_neq(bbb_pwm_set_polarity(bp, 2), BPRC_OK);
  // 0
  expect_neq(bbb_pwm_set_polarity(bp, 0), BPRC_OK);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test getting from an invalid duty cycle file.
 */
void
test_invalid_get_duty_cycle()
{
  uint32_t duty = 0;
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_invalid_set_duty_cycle", 1, 0);

  // Didn't claim, which is ok, but no data.
  expect_neq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test getting from an invalid period file.
 */
void
test_invalid_get_period()
{
  uint32_t period = 0;
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_invalid_set_period", 1, 0);

  // Didn't claim, which is ok, but no data.
  expect_neq(bbb_pwm_get_period(bp, &period), BPRC_OK);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Test getting from an invalid polarity file.
 */
void
test_invalid_get_polarity()
{
  int8_t polarity = 0;
  struct bbb_pwm_t *bp;
  bp = bbb_pwm_test_new("test_invalid_set_polarity", 1, 0);

  // Didn't claim, which is ok, but no data.
  expect_neq(bbb_pwm_get_polarity(bp, &polarity), BPRC_OK);

  bbb_pwm_test_delete(&bp);
}

/**
 * @brief Create a new test pwm and associated files.
 *
 * @param name The name of the new pwm.
 * @param create_files Should we create the pwm's files.
 * @param init_files Should we initialize the created files.
 *
 * @return A new bbb_pwm_t* on success, NULL on failure.
 */
struct bbb_pwm_t *
bbb_pwm_test_new(const char *name, int create_files, int init_files)
{
  struct bbb_pwm_t *bp;
  char path[1024];

  if(getcwd(path, sizeof(path)) == NULL) {
    fprintf(stderr, "Error getting cwd!\n");
    exit(errno);
  }

  bp = bbb_pwm_new(name, path);
  assert(bp != NULL);

  if(create_files) {
    FILE *fp;
    // Touch files and init if necessary
    // Running state.
    fp = fopen(bp->bp_running_state_file_path, "w");
    if(init_files) {
      fprintf(fp, "%d", 0);
    }
    assert(fp != NULL);
    fclose(fp);
    // Duty
    fp = fopen(bp->bp_duty_file_path, "w");
    if(init_files) {
      fprintf(fp, "%d", 0);
    }
    assert(fp != NULL);
    fclose(fp);
    // Period
    fp = fopen(bp->bp_period_file_path, "w");
    assert(fp != NULL);
    if(init_files) {
      fprintf(fp, "%d", 20000000);
    }
    fclose(fp);
    // Polarity
    fp = fopen(bp->bp_polarity_file_path, "w");
    assert(fp != NULL);
    if(init_files) {
      fprintf(fp, "%d", 1);
    }
    fclose(fp);
  }

  return bp;
}

/**
 * @brief Delete a test pwm and remove related files.
 *
 * @param bp_ptr The pwm to delete.
 */
void
bbb_pwm_test_delete(struct bbb_pwm_t **bp_ptr)
{
  struct bbb_pwm_t *bp;
  assert(bp_ptr != NULL);

  bp = *bp_ptr;
  if(bp == NULL) {
    return;
  }

  bbb_pwm_unclaim(bp);

  // Delete the files if they existed.
  remove(bp->bp_duty_file_path);
  remove(bp->bp_period_file_path);
  remove(bp->bp_polarity_file_path);

  bbb_pwm_delete(bp_ptr);
}
