/**
 * @file list_bbb_pwms.c
 * @brief
 * @author Travis Lane
 * @version
 * @date 2015-01-10
 */

#include <stdio.h>
#include <stdlib.h>

#include <bbb_pwm.h>

int main()
{
	struct bbb_pwm_controller_t *bpc = NULL;
	
	bpc = bbb_pwm_controller_new();

	foreach_pwm(bp, bpc) {
		printf("%s\n", bbb_pwm_get_name(bp));
	}

	bbb_pwm_controller_delete(&bpc);

  return 0;
}
