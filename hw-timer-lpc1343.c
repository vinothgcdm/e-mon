#include <mmio.h>
#include "hw-timer-lpc1343.h"

/* Timer counter 16 bit controller */
#define TMR16B0_BASE_ADDR 0x4000C000
#define TMR16B0IR TMR16B0_BASE_ADDR + 0x000
#define TMR16B0TCR TMR16B0_BASE_ADDR + 0x004
#define TMR16B0TC TMR16B0_BASE_ADDR + 0x008
#define TMR16B0PR TMR16B0_BASE_ADDR + 0x00C
#define TMR16B0PC TMR16B0_BASE_ADDR + 0x010
#define TMR16B0MCR TMR16B0_BASE_ADDR + 0x014
#define TMR16B0MR0 TMR16B0_BASE_ADDR + 0x018
#define TMR16B0MR1 TMR16B0_BASE_ADDR + 0x01C
#define TMR16B0MR2 TMR16B0_BASE_ADDR + 0x020
#define TMR16B0MR3 TMR16B0_BASE_ADDR + 0x024
#define TMR16B0CCR TMR16B0_BASE_ADDR + 0x028
#define TMR16B0CR0 TMR16B0_BASE_ADDR + 0x02C
#define TMR16B0EMR TMR16B0_BASE_ADDR + 0x03C
#define TMR16B0CTCR TMR16B0_BASE_ADDR + 0x070
#define TMR16B0PWMC TMR16B0_BASE_ADDR + 0x074

#define SYSAHBCLKCTRL 0x40048080

void hw_timer_init(void)
{
	/*
	 * Here we don't use catch and match pin,
	 * so no pin config
	 */
	mmio_set_bit(SYSAHBCLKCTRL, 7, 1); /* CT16B0 */
	mmio_write(TMR16B0TCR, 2); /* Reset TC and PC */
}

void hw_timer_setup(uint32_t milli_sec)
{
	mmio_set_bit(TMR16B0MCR, 0, 1); /* Interrupt on MR0 */
	mmio_set_bit(TMR16B0MCR, 1, 1); /* Reset on MR0 */
	mmio_write(TMR16B0PR, 72); /* 1 micro sec */
	mmio_write(TMR16B0MR0, (milli_sec * 1000));
	mmio_write(TMR16B0TCR, 1); /* Enable TC and PC to count */
}

void hw_timer_clean_intr(void)
{
	mmio_set_bit(TMR16B0IR, 0, 1); /* Clear Interrupt */
}
