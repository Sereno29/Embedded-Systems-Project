
#include <stdio.h>
#include <stdlib.h>

#include "bbb_pwm.h"

int main() 
{
	struct bbb_pwm_controller_t* bpc = NULL; 
	struct bbb_pwm_t* bp = NULL;
	char* pwm_name = "pwm_test_P8_46";

	bpc = bbb_pwm_controller_new();

	foreach_pwm(bp, bpc) {
		printf("%s\n", bbb_pwm_get_name(bp));
	}

	// Try getting a pwm.
	bp = bbb_pwm_controller_get_pwm(bpc, pwm_name);

	if(bp == NULL) {
		fprintf(stderr, "Failed to get pwm %s.\n", pwm_name);
		goto out;
	}

	// Claim it so we can open some files.
	bbb_pwm_claim(bp);

	// Stop the pwm so we don't accidentally do something.
	bbb_pwm_stop(bp);

	// Lets try setting the duty_percent to FULL SPEED.
	bbb_pwm_set_duty_percent(bp, 100.0f);

out:
		// Free the controller and all of the PWMs it manages.  
		bbb_pwm_controller_delete(&bpc);
	return 0;
}
