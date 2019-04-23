; G8RTOS_SchedulerASM.s
; Holds all ASM functions needed for the scheduler
; Note: If you have an h file, do not have a C file and an S file of the same name

	; Functions Defined
	.def G8RTOS_Start, PendSV_Handler

	; Dependencies
	.ref CurrentlyRunningThread, G8RTOS_Scheduler

	.thumb		; Set to thumb mode
	.align 2	; Align by 2 bytes (thumb mode uses allignment by 2 or 4)
	.text		; Text section

; Need to have the address defined in file 
; (label needs to be close enough to asm code to be reached with PC relative addressing)
RunningPtr: .field CurrentlyRunningThread, 32

; G8RTOS_Start
;	Sets the first thread to be the currently running thread
;	Starts the currently running thread by setting Link Register to tcb's Program Counter
G8RTOS_Start:

	.asmfunc
	; Loads the currently running thread’s context into the CPU
	LDR R0, RunningPtr ; load R0 with currently running ptr
	LDR R1, [R0] ; load R1 with the value pointed to by runningptr
	LDR SP, [R1] ; load stack pointer with the curernt threads stack ptr
	; Save current context
	POP {R4-R11} ; R4-R11 are at top of stack
	POP {R0-R3} ; then comes R0-R3
	POP {R12} ; then R12
	ADD SP, SP, #4 ; skip the link register
	POP {LR} ; Pop the pc value into the link register
	ADD SP, SP, #4 ; skip the psr
	; Enable interrupts
	CPSIE I ; Enable IRQ by clearing PRIMASK

	BX LR
	.endasmfunc

; PendSV_Handler
; - Performs a context switch in G8RTOS
; 	- Saves remaining registers into thread stack
;	- Saves current stack pointer to tcb
;	- Calls G8RTOS_Scheduler to get new tcb
;	- Set stack pointer to new stack pointer from new tcb
;	- Pops registers from thread stack
PendSV_Handler:

	.asmfunc
	; Save old thread context
	; R0-R3, R12-R15, PSR pushed to stack automatically
	; Push the rest of the registers (R4-R11) to the stack
	PUSH {R4-R11}
	; Save current stack pointer to tcb
	LDR R0, RunningPtr ; load r0 with pointer to tcb pointer
	LDR R1, [R0] ; load R1 with the value of that pointer
	STR SP, [R1] ; store stack pointer for old thread in memory address SP to be retrieved later

	; Call the scheduler to update CurrentlyRunningThread
	PUSH {LR} ; Push link register to stack to return from updating CurrentlyRunningThread
	BL G8RTOS_Scheduler ; update CurrentlyRunningThread round robin style
	POP {LR} ; Remove link register from the stack

	; Load the new thread context
	LDR R0, RunningPtr ; load r0 with pointer to tcb pointer, RunningPtr has been updated with the next thread
	LDR R1, [R0] ; load R1 with value pointed to by pointer which is the tcbpointer
	LDR SP, [R1] ; load the new thread stack pointer which was saved from the threads last context switch

	; Pop new thread R4-R11
	POP {R4-R11}
	; return from PendSV_Handler will pop R0-R3, R12-R15, PSR
	BX LR
	.endasmfunc
	
	; end of the asm file
	.align
	.end
	