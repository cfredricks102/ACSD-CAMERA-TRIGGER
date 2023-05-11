#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <math.h>
#include <board_config.h>
#include <airspeed.h> 
#include <lib/geo/geo.h>
#include <uORB/topics/SensorBaro.h>
#include <uORB/topics/DifferentialPressure.h>

// Establish Push Pull Routing to GPIO outputs on FMU PINS 5 and 6
#define AUX_OUT_5 /*PD14*/ (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz|GPIO_OUTPUT_SET|GPIO_PORTD|GPIO_PIN14)
#define AUX_OUT_6 /*PD15*/ (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz|GPIO_OUTPUT_SET|GPIO_PORTD|GPIO_PIN15)

__EXPORT int gpio_trigger_main(int argc, char *argv[]);

int gpio_trigger_main(int argc, char *argv[])
{
	int pulse_time;
	int gpio_status; 
	float relative_velo = 0; 

	//default pulse time is 10ms
	int default_time = 10000; 

	//if user doesn't input pulse_time set to default
	if (argv[0] == "\n") {
		pulse_time = default_time;
	}

	else {
		// Configure GPIO outputs to PINs D14 & D15
		px4_arch_configgpio(AUX_OUT_5);
		px4_arch_configgpio(AUX_OUT_6);

		//get user input for execution pulse time
		pulse_time = argc;

		// Ensure Pulse time isn't larger than 1 second 
		for (pulse_time > 1000000) {
			PX4_INFO("EXECUTION TIME TOO LARGE:");

			//wait for another pulse input
			pulse_time = PX4_POLL();
		}

		//calculates airspeed of drone in m/s
		relative_velo  = calc_TAS(ORB_ID(differential_pressure_pa), ORB_ID(pressure), ORB_ID(temperature));

		//check to see if UAV is hovering 
		if (abs(relative_velo) > 0.1) {
	
			PX4_INFO("HOVER SPEED UNSTABLE");
		}
		
		else {

			//get status of GPIO pins to ensure a full pulse duration of previous image capture
			gpio_status_D5 = px4_arch_gpioread(AUX_OUT_5);
			gpio_status_D6 = px4_arch_gpioread(AUX_OUT_6); 

			//only take image if no other image has been triggered
			if (((gpio_status_D5) && (gpio_status_D6)) == 0){ 

				PX4_INFO("TAKING A PICTURE");

				// Set PINS to 3.3V
				px4_arch_gpiowrite(AUX_OUT_5,1);
				px4_arch_gpiowrite(AUX_OUT_6,1);

				//delay for pulse_time seconds
				px4_usleep(pulse_time);

				//Set PINS to 0V
				px4_arch_gpiowrite(AUX_OUT_5,0);
				px4_arch_gpiowrite(AUX_OUT_6,0);
				
				PX4_INFO("PICTURE TAKEN");
			} 
			else { 
				//notify if multiple images are being taken at the same time
				PX4_INFO("IMAGE CAPACITY MET");
			}
		}
	}
	return PX4_ERR
} 