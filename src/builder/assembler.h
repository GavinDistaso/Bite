// SPDX-FileCopyrightText: 2022 Gavin Distaso
// SPDX-License-Identifier: MIT License

#ifndef BLD_ASMBLER_H
#define BLD_ASMBLER_H

#include "builder.h"

void BLD_appendOpcodes(BAB_CTX* biteASM, const char* opcodes, int len);

void BLD_assemble(BAB_CTX* biteASM, const char* asmCode);

#endif // BLD_ASMBLER_H