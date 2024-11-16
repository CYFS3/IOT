
	 EXTERN  OSRunning
	 EXTERN  OSPrioCur
	 EXTERN  OSPrioHighRdy
	 EXTERN  OSTCBCur
	 EXTERN  OSTCBHighRdy
	 EXTERN  OSIntNesting
	 EXTERN  OSIntExit
	 EXTERN  OSTaskSwHook
 
 
	EXPORT	OS_CPU_SR_Save
	EXPORT	OS_CPU_SR_Restore
	EXPORT	OSStartHighRdy
	EXPORT	OSCtxSw
	;EXPORT	OSIntCtxSw
	AREA os_cpu_asm, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8
 
OS_CPU_SR_Save
	MRS	R0, PRIMASK
	CPSID	I
	BX	LR
 
 
 
OS_CPU_SR_Restore
	MSR	PRIMASK, R0
	BX	LR
 
 
 
OSStartHighRdy
    LDR     R0, __OSTaskSwHook	 ; Call OSTaskSwHook()
    BLX     R0	 
 
    LDR     R0, __OSRunning	 ; OSRunning=1
    MOVS    R1, #0x01
    STRB    R1, [R0]	  
 
    LDR     R0, __OSTCBHighRdy	 ; SP = OSTCBHighRdy->OSTCBStkPtr
    LDR     R1, [R0]
    LDR     R2, [R1]
    MSR     MSP, R2
 ; Restore registers from new task stack
    POP    {R0-R7}	 ; Restore R4-R7, R8-R11
    MOV    R11, R3
    MOV    R10, R2
    MOV    R9, R1
    MOV    R8, R0
 
    ADD    SP, #0x10	 ; Restore PSR, PC, LR, R12
    POP    {R0-R3}
    MOV    R12, R0
    MOV    LR,	R1
    PUSH   {R2}	 ; Push PC into stack
    MSR    PSR, R3
     
    SUB    SP, #0x1C	 ; Restore R0-R3
    POP    {R0-R3}
    ADD    SP, #0x0C
    CPSIE  I	 ; Enable interrupts. NOTE: must not omit!
    POP    {PC}	 ; Jump to the task and execute, not return.
 
 
 
 
 
OSCtxSw
	CPSID	I	 
	; Save registers into current task stack
	SUB	SP, #0x20	 ; Save R4-R7
	PUSH	{R4-R7}
	ADD	SP, #0x30	 ; Save PSR, PC, LR, R12, R0-R3
	MRS R7, PSR
	MOV R6, LR	 ; NOTE!
	MOV R5, LR
	MOV R4, R12
	PUSH	{R0-R7}	 
	SUB	SP, #0x10	 ; Save R8-R11
	MOV R3, R11
	MOV R2, R10
	MOV R1, R9
	MOV R0, R8
	PUSH	{R0-R3}
	 ; Save SP in current stask stack
	MRS	R0, MSP	 ; OSTCBCur->OSTCBStkPtr = SP
	LDR	R1, __OSTCBCur
	LDR	R2, [R1]
	STR	R0, [R2]
	LDR	R0, __OSTaskSwHook	 ; Call OSTaskSwHook()
	BLX	R0
	LDR	R0, __OSPrioCur	 ; OSPrioCur = OSPrioHighRdy
	LDR	R1, __OSPrioHighRdy
	LDRB	R2, [R1]
	STRB	R2, [R0]
	LDR R0, __OSTCBCur	 ; OSTCBCur = OSTCBHighRdy
	LDR	R1, __OSTCBHighRdy
	LDR	R2, [R1]
	STR	R2, [R0]
	LDR R0, [R2]	 ; SP = OSTCBHighRdy->OSTCBStkPtr
	MSR	MSP, R0
	 ; Restore registers from new task (with high priority) stack
	POP	{R0-R7}	 ; Restore R4-R7, R8-R11
	MOV R11, R3
	MOV R10, R2
	MOV R9, R1
	MOV R8, R0
	ADD SP, #0X10	 ; Restore PSR, PC, LR, R12
	POP	{R0-R3}
	MSR PSR, R3
	PUSH	{R2}
	MOV LR, R1
	MOV R12, R0
	SUB SP, #0X1C	 ; Restore R0-R3
	POP	{R0-R3}
	ADD	SP, #0X0C
	CPSIE	I	 ; Enable interrupts! NOTE: must not omit!
	POP	{PC}	 ; Jump to task and execute, not return
	NOP
 
 
 
__OSRunning
    DCD    OSRunning
 
__OSTaskSwHook
    DCD    OSTaskSwHook
 
__OSIntExit
    DCD    OSIntExit
 
__OSIntNesting
    DCD    OSIntNesting
 
__OSPrioCur
    DCD    OSPrioCur
 
__OSPrioHighRdy
    DCD    OSPrioHighRdy
 
__OSTCBCur
    DCD    OSTCBCur
 
__OSTCBHighRdy
    DCD    OSTCBHighRdy
 
    END