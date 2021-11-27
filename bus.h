// Copyright: 2021 - 2021, Ziemas
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "types.h"
#include <memory>

class bus {
  public:
    bus(std::unique_ptr<byte[]> ram, std::unique_ptr<byte[]> rom)
        : m_ram(std::move(ram)), m_rom(std::move(rom)){};

    void fetch_instr(u32 pc, u32 &value);
    void store32(u32 addr, u32 value);
    void store16(u32 addr, u16 value);

    u32 read32(u32 addr);

  private:
    std::unique_ptr<byte[]> m_ram;
    std::unique_ptr<byte[]> m_rom;
};
