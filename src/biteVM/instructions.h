// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef BITEVM_INST_H
#define BITEVM_INST_H

/* enum defines */

typedef enum REGISTERS{
    // ===== General perpose registers ===== //
    // FULL 64b | LOWER 32b | LOWER 16b | LOWER 8b |
      RA64=10,  RA32,       RA16,       RA8,        // accumulator
      RS64=20,  RS32,       RS16,       RS8,        // stack pointer
      RB64=30,  RB32,       RB16,       RB8,        // base address
      RD64=40,  RD32,       RD16,       RD8,        // data/ return
      RX64=50,  RX32,       RX16,       RX8,        // any use

    // ===== System ===== //
    RST='s', // status register (each bit is a status flag)

} REGISTERS;

typedef enum OPCODES{
    // ===== arithmetic ===== //
    ADC = 0x10, ADD, SUB, MUL, DIV, POW, MOD,
    NUM, // specifies use of number in argument, ex. 'MOV NUM 3244, REG R1'

    // ===== bitwise ===== //
    AND = 0x20, OR, NOT, XOR, 
    SHL, SHR, // bit shift left/ right
    ROL, ROR,

    // ===== conditional & assignment ===== //
    MOV = 0x30, JMP, CALL, RET, CMP, 
    FUNC, END, LABEL, GOTO,

    // - note: following based on CONDITION flag
    JME, JML, JMG, // jump if equal, less, or greater
    CAE, CAL, CAG, // call if equal, less, or greater

    // ===== system & stack ===== //
    SYS = 0x40, INT, // SYSCALL, Interupt
    PUSH, POP, VAR2REG,
    MEM, // specifies use of memory in argument, ex. 'MOV NUM 3244, MEM 0xf2c1bf'

    // ===== register ===== //
    REG = 0x50, // specifies use of register in argument, ex. 'MOV NUM 3244, REG R1'

} OPCODES;

typedef enum INTERRUPTS{
    OPEN, CLOSE, READ, WRITE, SEEK, STAT,
    MALLOC, FREE
} INTERRUPTS;

#endif // BITEVM_INST_H