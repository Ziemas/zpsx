// Copyright: 2021 - 2021, Ziemas
// SPDX-License-Identifier: GPL-3.0-or-later
#include "cpu.h"
#include "magic_enum.hpp"
#include <fmt/format.h>

bool cpu::run() {
    m_pc += 4;

    if (m_branch) {
        if (m_branch_delay == 0) {
            m_branch = false;
            m_pc = m_next_pc;
            fmt::print("jumping to {:x}\n", m_next_pc);
        } else {
            m_branch_delay--;
        }
    }

    u32 instr;
    m_bus.fetch_instr(m_pc, instr);
    auto op = opcode(instr >> 26);

    switch (op) {
    case opcode::SPECIAL:
        inst_special(instr);
        break;
    case opcode::LUI:
        inst_lui(instr);
        break;
    case opcode::ORI:
        inst_ori(instr);
        break;
    case opcode::SW:
        inst_sw(instr);
        break;
    case opcode::ADDI:
        inst_addi(instr);
        break;
    case opcode::ADDIU:
        inst_addiu(instr);
        break;
    case opcode::BNE:
        inst_bne(instr);
        break;
    case opcode::J:
        inst_j(instr);
        break;
    default:
        fmt::print("{:x}: Unimplemented opcode {}\n", m_pc, disassmble(instr));
        return false;
    }

    fmt::print("{:x}: executing {}\n", m_pc, disassmble(instr));
    for (int i = 0; i < REGISTER_COUNT; i++) {
        fmt::print("{}: {:x}\n", magic_enum::enum_name(regname(i)), m_regs[i]);
    }

    m_cycles++;

    return true;
}

// TODO exception?
void cpu::inst_addi(u32 instr) {
    s32 imm = static_cast<s16>(instr & 0xffff);
    auto rt = (instr >> 16) & 0x1f;
    auto rs = (instr >> 21) & 0x1f;

    m_regs[rt] = m_regs[rs] + imm;
}

void cpu::inst_addiu(u32 instr) {
    s32 imm = static_cast<s16>(instr & 0xffff);
    auto rt = (instr >> 16) & 0x1f;
    auto rs = (instr >> 21) & 0x1f;

    m_regs[rt] = m_regs[rs] + imm;
}

void cpu::inst_sw(u32 instr) {
    s32 offset = static_cast<s16>(instr & 0xffff);
    auto rt = (instr >> 16) & 0x1f;
    auto base = (instr >> 21) & 0x1f;

    m_bus.store32(base + offset, m_regs[rt]);
}

void cpu::inst_ori(u32 instr) {
    auto imm = instr & 0xffff;
    auto rt = (instr >> 16) & 0x1f;
    auto rs = (instr >> 21) & 0x1f;

    m_regs[rt] = m_regs[rs] | imm;
}

void cpu::inst_lui(u32 instr) {
    auto imm = instr & 0xffff;
    auto rt = (instr >> 16) & 0x1f;
    m_regs[rt] = imm << 16;
}

void cpu::inst_bne(u32 instr) {
    s32 imm = static_cast<s16>(instr & 0xffff);
    auto rt = (instr >> 16) & 0x1f;
    auto rs = (instr >> 21) & 0x1f;
    auto target = (imm << 2) + m_pc + 4;

    if (m_regs[rt] != m_regs[rs])
        branch(m_next_pc = target);
}

void cpu::inst_j(u32 instr) {
    auto target = ((instr & 0x03ffffffff) << 2);
    branch((m_pc & 0xf0000000) + 4 + target);


}

void cpu::inst_special(u32 instr) {
    auto func = func_special(instr & 0x3f);
    switch (func) {
    case func_special::SLL:
        inst_sll(instr);
        break;
    case func_special::SLT:
        inst_slt(instr);
        break;
    default:
        fmt::print("{:x}: Unimplemented opcode {}\n", m_pc, disassmble(instr));
        break;
    }
}

void cpu::inst_slt(u32 instr) {
    auto rd = (instr >> 11) & 0x1f;
    auto rt = (instr >> 16) & 0x1f;
    auto rs = (instr >> 21) & 0x1f;

    if (m_regs[rs] < m_regs[rt]) {
        m_regs[rd] = 1;
    } else {
        m_regs[rd] = 0;
    }

}

void cpu::inst_sll(u32 instr) {
    auto sa = (instr >> 6) & 0x1f;
    auto rd = (instr >> 11) & 0x1f;
    auto rt = (instr >> 16) & 0x1f;

    m_regs[rd] = m_regs[rt] << sa;
}

std::string_view cpu::disassmble(u32 instr) {
    auto op = opcode(instr >> 26);

    if (op == opcode::SPECIAL) {
        auto func = func_special(instr & 0x3f);
        return magic_enum::enum_name(func);
    }

    if (op == opcode::REGIMM) {
        auto func = func_regimm((instr >> 16) & 0x1f);
        return magic_enum::enum_name(func);
    }

    return magic_enum::enum_name(op);
}

std::string_view cpu::reg_to_string(int reg) {
    return magic_enum::enum_name(static_cast<regname>(reg));
}
