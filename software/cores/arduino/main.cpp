#include <Arduino.h>

extern "C"{
int __main(void);
}

int __main(void)
{
	construct();

	init();

	setup();

	for (;;) {
	  loop();
	}

	return 0;
}

/* mark as a weak main function implementation */
int main() __attribute__ ((weak,alias("__main")));
