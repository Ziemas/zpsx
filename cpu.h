// Copyright: 2021 - 2021, Ziemas
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "bus.h"
#include "types.h"
#include <array>
#include <string>

class cpu {
  public:
    cpu(bus &bus) : m_bus(bus){};

    bool run();
    void reset() {
        m_next_pc = RESET_VECTOR;
        m_regs = {};
    };

  private:
    static constexpr u32 RESET_VECTOR = 0xbfc00000;
    static constexpr int REGISTER_COUNT = 32;
    std::array<u32, REGISTER_COUNT> m_regs{0};

    bus &m_bus;

    bool m_branch{false};
    u8 m_branch_delay{0};

    u32 m_pc{RESET_VECTOR};
    u32 m_next_pc{0};
    u64 m_cycles{0};

    // clang-format off
    enum class regname {
        zero,
        at,
        v0, v1,
        a0, a1, a2, a3,
        t0, t1, t2, t3, t4, t5, t6, t7,
        s1, s2, s3, s4, s5, s6, s7,
        t8, t9,
        k0, k1,
        gp,
        sp,
        fp,
        ra,
    };
    // clang-format on

    enum class opcode {
        SPECIAL = 0x0,
        REGIMM = 0x1,
        J = 0x2,
        JAL = 0x3,
        BEQ = 0x4,
        BNE = 0x5,
        BLEZ = 0x6,
        BGETZ = 0x7,
        ADDI = 0x8,
        ADDIU = 0x9,
        SLTI = 0xa,
        SLTIU = 0xb,
        ANDI = 0xc,
        ORI = 0xd,
        XORI = 0xe,
        LUI = 0xf,
        COP0 = 0x10,
        COP1 = 0x11,
        COP2 = 0x12,
        COP3 = 0x13,
        LB = 0x20,
        LH = 0x21,
        LWL = 0x22,
        LW = 0x23,
        LBU = 0x24,
        LHU = 0x25,
        LWR = 0x26,
        SB = 0x28,
        SH = 0x29,
        SW = 0x2b,
        SWL = 0x2a,
        SWR = 0x2E,
    };

    enum class func_regimm {
        BLTZ = 0x0,
        BGEZ = 0x1,
        BLTZAL = 0x10,
        BGEZAL = 0x11,
    };

    enum class func_special {
        SLL = 0x0,
        SRL = 0x2,
        SRA = 0x3,
        SLLV = 0x4,
        SRLV = 0x6,
        SRAV = 0x7,
        JR = 0x8,
        JALR = 0x9,
        SYSCALL = 0xc,
        BREAK = 0xd,
        MFHI = 0x10,
        MTHI = 0x11,
        MFLO = 0x12,
        MTLO = 0x13,
        MULT = 0x18,
        MULTU = 0x19,
        DIV = 0x1a,
        DIVU = 0x1b,
        ADD = 0x20,
        ADDU = 0x21,
        SUB = 0x22,
        SUBU = 0x23,
        AND = 0x24,
        OR = 0x25,
        XOR = 0x26,
        NOR = 0x27,
        SLT = 0x2a,
        SLTU = 0x2b,
    };

    void branch(u32 addr) {
        m_next_pc = addr;
        m_branch = true;
        m_branch_delay = 1;
    };

    void inst_lui(u32 instr);
    void inst_addi(u32 instr);
    void inst_addiu(u32 instr);
    void inst_ori(u32 instr);
    void inst_sw(u32 instr);
    void inst_bne(u32 instr);
    void inst_j(u32 instr);

    void inst_special(u32 instr);
    void inst_slt(u32 instr);
    void inst_sll(u32 instr);

    static std::string_view reg_to_string(int reg);
    static std::string_view disassmble(u32 instr);
};
