// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef BLD_ASMBLER_H
#define BLD_ASMBLER_H

#include "builder.h"

void BLD_appendOpcodes(BAB_CTX* biteASM, const char* opcodes, int len);

void BLD_assemble(BAB_CTX* biteASM, const char* asmCode, int len);

int BLD_getRegisterIndex(const char* reg);

void BLD_appendInt(BAB_CTX* biteASM, void* x, int size);

#endif // BLD_ASMBLER_H