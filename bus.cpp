// Copyright: 2021 - 2021, Ziemas
// SPDX-License-Identifier: GPL-3.0-or-later
#include "bus.h"
#include <fmt/format.h>

void bus::store32(u32 addr, u32 value) {
    fmt::print("32bit write to {:x} <- \n", addr, value);
    *(u32*)&m_ram[addr & 0x0fffffff] = value;
}

void bus::store16(u32 addr, u16 value) {
    fmt::print("16bit write to {:x} <- {}\n", addr, value);
    *(u16*)&m_ram[addr & 0x0fffffff] = value;
}

u32 bus::read32(u32 addr) {
    return *(u32*)&m_ram[addr & 0x0fffffff];
}

void bus::fetch_instr(u32 pc, u32 &value) {
    u8 segment = pc >> 28;

    u32 addr = pc & 0x0fffffff;
    if (addr >= 0x0fc00000) {
        value = *reinterpret_cast<u32*>(&m_rom[addr & 0xfffff]);
    } else {
        value = *reinterpret_cast<u32*>(&m_ram[addr & 0x0fffffff]);
    }
}
