// Copyright: 2021 - 2021, Ziemas
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

/* Bitfield convenience class for use with unions
 * should be allowed as per common initial subsequence rule regarding unions */
template <typename sourcetype, typename type, int width, int pos>
class bitfield {
  public:
    constexpr type get() {
        return static_cast<type>((underlying >> pos) & mask());
    };

    constexpr void set(type value) {
        underlying &= ~mask();
        underlying |= (value & mask()) << pos;
    };

  private:
    sourcetype underlying;

    constexpr int mask() { return (1 << width) - 1; };
};
