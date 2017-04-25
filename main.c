#include <board.h>
#include <stdio.h>
#include <timer.h>
#include <serial.h>
#include <lcd.h>
#include <adc.h>
#include "hw-timer-lpc1343.h"

#define CH 1

struct adc_val {
	uint64_t sum;
        uint32_t curr_val;
	uint32_t count;
} data;


void sum_current(void *timer)
{
        data.curr_val = adc_read_raw(CH);
	data.sum += data.curr_val;
	data.count++;
	hw_timer_clean_intr();
}

void print_data(void *timer)
{
	static uint32_t sec = 0;
	struct adc_val tmp = data;
	uint64_t milli_amp_ms;
	uint32_t milli_amp_h_L;
	uint32_t milli_amp_h_R;

	sec++;
	lcd_clear();
	printf("%u mA\r\n", (tmp.curr_val * 3300UL) >> 10);
	milli_amp_ms = (tmp.sum * 10 * 3300UL) >> 10;
	milli_amp_h_L = milli_amp_ms / (1000 * 3600);
	milli_amp_h_R = milli_amp_ms % (1000 * 3600);
	printf("%u: %u.%u mAh", sec, milli_amp_h_L, (milli_amp_h_R * 1000) / (1000 * 3600));

	timer_setup(timer, 1000);
}

int main(void)
{
	timer_t timer;
	
	board_init();
	board_stdout(LCD_STDOUT);
	lcd_init();
	timer_init();
	adc_init();

	adc_enable(CH);

	timer_setup(&timer, 1000);
	timer_setcb(&timer, print_data);

	hw_timer_init();
	hw_timer_setcb(sum_current);
	hw_timer_setup(10);

	event_poll();
	
	return 0;
}
