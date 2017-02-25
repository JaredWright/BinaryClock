#define __SFR_OFFSET 0 
#include <avr/io.h> 
;
; This file implements a driver for drawing strands of WS2812b LEDs (adafruit neopixels)
; A WS2812b LED receives data using a 1 wire serial protocol that mixes the clock
; and data together, and uses timing to distinguish values of '0' and '1'
;
; A value of '0' looks like this:
; .4us (8 ticks @ 20MHz)
; ___________
;            \
;             \
;               ------------------------
;                .85us (17 ticks @ 20MHz)
;
;
; A value of '1' looks like this:
; .85us (17 ticks @ 20MHz)
; _________________________
;                          \
;                           \
;                            ------------
;                            .4us (8 ticks @ 20MHz)

; Usage: 
; void ws2812b_strand_draw(uint8_t * buffer, uint16_t size,
;							volatile uint8_t * port, uint8_t pin);
;
; Registers:
; r18 = Current value (byte) being written 
; r19 = Counter for how many bits of r18 have been output
; r20 = '1' output
; r21 = '0' output
; r22 = Save SREG to restore before return
; r24:25 = Length of buffer pointed to by 26:27 (uint16_t size)
; r26:27 = (X) = Data pointer to an array of 24-bit GRB values (uint8_t * buffer)
; r28:29 = (Y) = MMIO address of the PORT to use as the output
; r30 = Which pin on the specified PORT to use as the output
;

.global ws2812b_strand_draw
ws2812b_strand_draw:
	; move arguments into registers as defined above
	movw   r26, r24		 
	movw   r24, r22
	movw   r28, r20
	mov    r30, r18
	in     r22, SREG
	ld     r20, Y
	or     r20, r30
	ld     r21, Y
	com    r30
	and    r21, r30
	ldi    r19, 7        ; counter for first 7 bits (8th bit handled different)
	ld     r18,X+        ; load first data byte to be clocked out
	cli    				 ; no interrupts from here on
pulse_begin:
	st     Y , r20		 ; 2 
	lsl    r18           ; 1   +1 next bit into C, MSB first
	brcs   pulse_one     ; 1/2 +2 branch if 1
pulse_zero:
	nop                  ; 1   +4
	nop                  ; 1   +5
	nop                  ; 1   +6
	nop                  ; 1   +7
	st      Y, r21		 ; 2
	nop                  ; 1   +10
	nop                  ; 1   +11
	nop                  ; 1   +12
	nop                  ; 1   +13
	nop                  ; 1   +14
	nop                  ; 1   +15
	nop                  ; 1   +16
	nop                  ; 1   +17
	nop                  ; 1   +18
	nop                  ; 1   +19
	bst    r18, 7        ; 1   +20 save last bit of data for fast branching
	subi   r19, 1        ; 1   +21 how many more bits for this byte?
	breq   eighth_pulse    ; 1/2 +22 last bit, do differently
	rjmp   pulse_begin   ; 2   +23, 10 total for 0 bit
pulse_one:
	nop                  ; 1   +5
	nop                  ; 1   +6
	nop                  ; 1   +7
	nop                  ; 1   +8
	nop                  ; 1   +9
	nop                  ; 1   +10
	nop                  ; 1   +12
	nop                  ; 1   +12
	nop                  ; 1   +13
	nop                  ; 1   +14
	bst    r18, 7        ; 1   +15 save last bit of data for fast branching
	subi   r19, 1        ; 1   +16 how many more bits for this byte
	st     Y, r21		 ; 2
	nop                  ; 1   +19
	nop                  ; 1   +20
	nop                  ; 1   +21
	nop                  ; 1   +22
	brne   pulse_begin   ; 2/1 +23 10 total for 1 bit (fall thru if last bit)
eighth_pulse:
	ldi    r19, 7        ; 1   +24 bit count for next byte
	st     Y , r20		 ; 2
	brts   eighth_one      ; 1/2 +1 branch if last bit is a 1
eighth_zero:
	nop                  ; 1   +3
	nop                  ; 1   +4
	nop                  ; 1   +5
	nop                  ; 1   +6
	nop                  ; 1   +7
	st    Y , r21		 ; 2
	nop                  ; 1   +10
	nop                  ; 1   +11
	nop                  ; 1   +12
	nop                  ; 1   +13
	nop                  ; 1   +14
	nop                  ; 1   +15
	nop                  ; 1   +16
	nop                  ; 1   +17
	nop                  ; 1   +18
	ld     r18, X+       ; 2   +19 fetch next byte
	sbiw   r24, 1        ; 2   +21 dec byte counter
	breq return1		 ; 1/2 +23
	rjmp pulse_begin	 ; 2   +24
return1:
	out    SREG, r22     ; restore global int flag
	ret
eighth_one:
	ld     r18, X+       ; 2   +3 fetch next byte
	nop					 ; 1   +5
	nop					 ; 1   +6
	nop                  ; 1   +7
	nop                  ; 1   +8
	nop                  ; 1   +9
	nop                  ; 1   +10
	nop                  ; 1   +11
	nop                  ; 1   +12
	nop                  ; 1   +13
	nop                  ; 1   +14
	nop                  ; 1   +15
	nop                  ; 1   +16
	st     Y , r21		 ; 2
	nop                  ; 1   +19
	nop                  ; 1   +20
	sbiw   r24, 1        ; 2   +21 dec byte counter
	breq return2		 ; 1/2 +23
	rjmp pulse_begin	 ; 2   +24
return2:
	out    SREG, r22     ; restore global int flag
	ret

