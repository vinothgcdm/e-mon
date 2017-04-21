#include <board.h>
#include <timer.h>
#include <serial.h>
#include <adc.h>
#include <lcd.h>
#include <stdio.h>
#include "hw-timer-lpc1343.h"

#define CH 1
#define RESISTANCE 1  // Ohm

static uint32_t curr_val = 0;
static uint64_t sum = 0;

void sum_current(void)
{
        curr_val = adc_read_raw(CH);
	sum += curr_val;
	hw_timer_clean_intr();
}

void test(void *timer)
{
	static uint32_t sec = 0;
	uint64_t l_sum = sum;
	uint32_t l_curr_val = curr_val;
	uint32_t milli_amp_ms;
	uint32_t milli_amp_h;
	char buf[10];

	sec++;
	lcd_clear();
	printf("%04u mV\r\n", ((l_curr_val * 3300UL) >> 10) / RESISTANCE);
	milli_amp_ms = ((l_sum * 3300UL) >> 10) / RESISTANCE;
	milli_amp_h = milli_amp_ms / (1000 * 60 * 60);
	sprintf(buf, "%d", milli_amp_ms % (1000 * 60 * 60));
	/* printf("%04u mAms\r\n", milli_amp_ms); */
	printf("%02u: %04u.%c%c mAh", sec, milli_amp_h, buf[0], buf[1]);

	timer_setup(timer, 1000);
}

int main(void)
{
	timer_t timer;
	
	board_init();
	/* board_stdout(SERIAL_STDOUT); */
	/* serial_init(); */
	board_stdout(LCD_STDOUT);
	lcd_init();
	timer_init();
	adc_init();

	adc_enable(CH);
	
	hw_timer_init();
	hw_timer_setcb(sum_current);
	hw_timer_setup(1);

	timer_setup(&timer, 1000);
	timer_setcb(&timer, test);
	event_poll();
	
	return 0;
}
