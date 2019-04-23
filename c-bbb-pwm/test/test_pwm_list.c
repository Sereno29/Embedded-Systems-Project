
#include <bbb_pwm_internal.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_order();

int
main()
{
  test_order();
}


void
test_order()
{
  struct bbb_pwm_t *cur;
  struct bbb_pwm_controller_t *ctrl;

  ctrl = calloc(sizeof(struct bbb_pwm_controller_t), 1);
  assert(ctrl != NULL);

  cur = bbb_pwm_new("a", "");
  assert(bbb_pwm_controller_add_pwm(ctrl, cur) == BPRC_OK);

  cur = bbb_pwm_new("b", "");
  assert(bbb_pwm_controller_add_pwm(ctrl, cur) == BPRC_OK);

  cur = bbb_pwm_new("c", "");
  assert(bbb_pwm_controller_add_pwm(ctrl, cur) == BPRC_OK);

  cur = ctrl->bpc_head_pwm;
  while(cur != NULL) {
    fprintf(stderr, "%s ", cur->bp_name);
    cur = cur->bp_next;
  }

  assert(strcmp(ctrl->bpc_head_pwm->bp_name, "a") == 0);
  assert(strcmp(ctrl->bpc_head_pwm->bp_next->bp_name, "b") == 0);
  assert(strcmp(ctrl->bpc_head_pwm->bp_next->bp_next->bp_name, "c") == 0);

  bbb_pwm_controller_delete(&ctrl);
}
