c-bbb-pwm
======================

[![Build Status](https://travis-ci.org/Coderlane/c-bbb-pwm.svg)](https://travis-ci.org/Coderlane/c-bbb-pwm)

A C library for managing pwm output from a beagle bone black.

## Installation

c-bbb-pwm relies on udev for finding devices, you'll need to install 
it to be able to build c-bbb-pwm. On Ubuntu the package you want is libudev-dev.

      git clone https://github.com/Coderlane/c-bbb-pwm.git
      cd c-bbb-pwm
      mkdir build && cd build
      ccmake ..
      make
      sudo make install

## Usage

### Setting Up uEnv.txt

Before you are able to do anything useful with the library,
you will need to configure your output pins. 
Open /boot/uEnv.txt as root using your favorite text editor.
Then look for these two lines.

      cape_disable=capemgr.disable_partno=
      cape_enable=capemgr.enable_partno=

You'll need to choose what you want to put on those lines.
For example, I don't need HDMI or the built in eMMC.

To disable them I added:

      cape_disable=capemgr.disable_partno=BB-BONELT-HDMI,BB-BONELT-HDMIN,BB-BONE-EMMC-2G

Then I chose to enable pwm\_P8\_46

      cape_enable=capemgr.enable_partno=BB-UART5,am33xx_pwm,bone_pwm_P8_46
      
You will most likely need to disable some parts to enable the pwms.
The documentation on this process is a bit iffy. 
I'll write something up to explain it better at some point.

To enable any pwm, you first need to enable the UART5 part,
as well as the am3xx\_pwm part. Then you can add any pwm you want.

When you are done editing, write out the file and reboot the pwm.
Then changes will be applied on the next boot, and will persist
until you modify them further. Be careful with what you disable,
if you are working off of the eMMC, don't disable that.

### Example Program
Here is a quick example. It simply sets up a new controller,
and then shows what devices we know about. Then we attempt to get
one of the devices. If we are successful it stops the PWM, if it 
was running, and sets it's duty percent to 100 or full speed. 
You would normally then start the PWM, but I don't want to right now.

	#include <stdio.h>
	#include <stdlib.h>
	#include <bbb_pwm/bbb_pwm.h>
     
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

## Documentation
 
 Build yer own! I use doxygen, just open the Doxyfile with doxywizard or 
simmilar and output whatever type of files you want. 
In the future I'll upload these online, but not yet.
 
## Known Issues

 * No automatic enabling/disabling of capemgr devices. 
 * Requires root :/
 * Probably not thread safe
 * Others. It's v0.2.0, I'm working on it!
