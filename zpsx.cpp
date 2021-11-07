// Copyright: 2021 - 2021, Ziemas
// SPDX-License-Identifier: GPL-3.0-or-later
#include "bus.h"
#include "cpu.h"
#include "types.h"
#include <fmt/format.h>
#include <fstream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fmt::print("Usage: ./zpsx [romfile]\n");
    }

    std::fstream romfile(argv[1], std::fstream::binary | std::fstream::in);
    auto rom = std::make_unique<byte[]>(512 * 1024);
    romfile.read(reinterpret_cast<char *>(rom.get()), 512 * 1024);

    auto ram = std::make_unique<byte[]>(2048 * 1024);

    auto sysbus = bus(std::move(ram), std::move(rom));
    auto syscpu = cpu(sysbus);

    while (syscpu.run()) {

    }


    return 0;
}
