%macro exceptions_error_code 1-9 8,10,11,12,13,14,17,30
    %assign exception_number %1
    %assign %%has_error_code 0
    %rep %0-1
        %rotate 1
        %if exception_number = %1
            %assign %%has_error_code 1
            %exitrep
        %endif
    %endrep

    %if %%has_error_code = 0
        ; push a zero value in exceptions that doesn't
        ; include an error code to align the stack
        push 0
    %endif
%endmacro

%macro context_save 0
    cld
    push gs
    push fs
    push es
    push ds
    push eax
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
%endmacro

%macro context_restore 0
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp
    pop eax
    pop ds
    pop es
    pop fs
    pop gs
%endmacro

; Generate an exception handler.
; define_interrupt_handler(handler, number)
%macro define_interrupt_handler 2
    exception_handler_%2:
        ; Normalize stack layout for error codes.
        exceptions_error_code %2
        push %2
        context_save
        mov eax, esp
        push eax
        call %1
        pop eax
        context_restore
        add esp, 8
        iret
%endmacro

; Create a range of interrupt handlers.
; define_interrupt_handlers(handler, start, end)
%macro define_interrupt_handlers 3
    %assign %%i %2
    %rep %3-%2
        define_interrupt_handler %1, %%i
        %assign %%i %%i+1
    %endrep
%endmacro

%macro exception_handler_name 1
    dd exception_handler_%1
%endmacro

; Create an interrupt handler table.
; define_interrupt_handler_table(name)
%macro define_interrupt_handler_table 2
    global %1
    %1:
        %assign i 0
        %rep %2
            exception_handler_name i
            %assign i i+1
        %endrep
%endmacro

[section .text]
    align 4

    magic:
        dd 0xcafecafe

    start:
        global start

        ; Call the kernel entry point.
        extern kernel_main
        call kernel_main

        ; Halt.
        int 3

    ; Our interrupt dispatcher implemented in C.
    extern interrupt_handler

    ; Create all the inerrupt handlers.
    define_interrupt_handlers interrupt_handler, 0, 256

[section .data]
    ; Create an interrupt handler table.
    define_interrupt_handler_table interrupt_handler_table, 256