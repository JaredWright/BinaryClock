#define __SFR_OFFSET 0 
#include <avr/io.h> 

.equ      OUTBIT,   0

;
; The WS2812b receives data using a 1 wire serial protocol that mixes the clock
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

; extern void ws2812b_strand_draw(uint8_t * buffer, uint16_t size);
;
; r17 = busy wait counter
; r18 = data byte
; r19 = 7-bit count
; r20 = 1 output
; r21 = 0 output
; r22 = SREG save
; r24:25 = 16-bit count
; r26:27 (X) = data pointer
.global ws2812b_strand_draw
ws2812b_strand_draw:
	movw   r26, r24      ;r26:27 = X = p_buf
	movw   r24, r22      ;r24:25 = count
	in     r22, SREG     ;save SREG (global int state)
	cli                  ;no interrupts from here on, we're cycle-counting
	in     r20, PORTB
	ori    r20, (1<<OUTBIT)         ;our '1' output
	in     r21, PORTB
	andi   r21, ~(1<<OUTBIT)        ;our '0' output
	ldi    r19, 7        ;7 bit counter (8th bit is different)
	ld     r18,X+        ;get first data byte
loop1:
	out    PORTB, r20    ; 1   +0 start of a bit pulse
	lsl    r18           ; 1   +1 next bit into C, MSB first
	brcs   L1            ; 1/2 +2 branch if 1
	nop                  ; 1   +3
	nop                  ; 1   +4
	nop                  ; 1   +5
	nop                  ; 1   +6
	nop                  ; 1   +7
	out    PORTB, r21    ; 1   +8 end hi for '0' bit (3 clocks hi)
	nop                  ; 1   +9
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
	breq   bit8          ; 1/2 +22 last bit, do differently
	rjmp   loop1         ; 2   +23, 10 total for 0 bit
L1:
	nop                  ; 1   +4
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
	out    PORTB, r21    ; 1   +17 end hi for '1' bit (7 clocks hi)
	nop                  ; 1   +18
	nop                  ; 1   +19
	nop                  ; 1   +20
	nop                  ; 1   +21
	nop                  ; 1   +22
	brne   loop1         ; 2/1 +23 10 total for 1 bit (fall thru if last bit)
bit8:
	ldi    r19, 7        ; 1   +24 bit count for next byte
	out    PORTB, r20    ; 1   +0 start of a bit pulse
	brts   L2            ; 1/2 +1 branch if last bit is a 1
	nop                  ; 1   +2
	nop                  ; 1   +3
	nop                  ; 1   +4
	nop                  ; 1   +5
	nop                  ; 1   +6
	nop                  ; 1   +7
	out    PORTB, r21    ; 1   +8 end hi for '0' bit (3 clocks hi)
	nop                  ; 1   +9
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
	rjmp loop1			 ; 2   +24
return1:
	out    SREG, r22     ; restore global int flag
	ret
L2:
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
	out    PORTB, r21    ; 1   +17 end hi for '1' bit (17 clocks hi)
	nop                  ; 1   +18
	nop                  ; 1   +19
	nop                  ; 1   +20
	sbiw   r24, 1        ; 2   +21 dec byte counter
	breq return2		 ; 1/2 +23
	rjmp loop1			 ; 2   +24
return2:
	out    SREG, r22     ; restore global int flag
	ret
