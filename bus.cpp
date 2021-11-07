#include "bus.h"
#include <fmt/format.h>

void bus::store32(u32 addr, u32 value) {
    *(u32*)&m_ram[addr & 0x0fffffff] = value;
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
