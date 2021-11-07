// Copyright: 2021 - 2021, Ziemas
// SPDX-License-Identifier: GPL-3.0-or-later
#include "cpu.h"
#include "magic_enum.hpp"
#include <fmt/format.h>

bool cpu::run() {
    u32 instr;
    m_bus.fetch_instr(m_next_pc, instr);

    m_pc = m_next_pc;
    m_next_pc += 4;

    // fmt::print("pc: {:x}, instr {:x}\n", m_next_pc, instr);

    auto op = instr >> 26;

    switch (op) {
    default:
        fmt::print("{:x}: Unimplemented opcode {}\n", m_pc, disassmble(instr));
        return false;
    }

    return true;
}

std::string_view cpu::disassmble(u32 instr) {
    auto op = opcode(instr >> 26);

    if (op == opcode::SPECIAL)
    {
        auto func = func_special(instr & 0x3f);
        return magic_enum::enum_name(func);
    }

    if (op == opcode::REGIMM)
    {
        auto func = func_regimm((instr >> 16) & 0x1f);
        return magic_enum::enum_name(func);
    }

    return magic_enum::enum_name(op);
}

std::string_view cpu::reg_to_string(int reg) {
    return magic_enum::enum_name(static_cast<regname>(reg));
}
