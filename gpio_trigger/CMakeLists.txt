px4_add_module(
	MODULE examples__gpio_trigger
	MAIN gpio_trigger
	STACK_MAIN 2000
	SRCS
		gpio_trigger.c
	DEPENDS
	)