#ifndef HW_TIMER_H
#define HW_TIMER_H

#include <irq.h>

#define hw_timer_setcb(fun_cb) \
	irq_setcb(IRQ_CT16B0, fun_cb);

void hw_timer_init(void);
/* We can set upto 65 milli second. If you want more time change
 * TMR16B0PR */
void hw_timer_setup(uint32_t milli_sec);
void hw_timer_clean_intr(void);

#endif
