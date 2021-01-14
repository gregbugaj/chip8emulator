#ifndef CHIP8EMULATOR_TYPES_H
#define CHIP8EMULATOR_TYPES_H

#include <bitset>
#include <climits>
#include <iostream>
#include <vector>
#include <iomanip>

using byte_t = u_char;
using word_t = uint16_t; // two bytes long and stored big-endian
using namespace std;

/**
 * Output value as a bit string
 * @tparam T
 * @param out
 */
template<class T>
std::string asPrettyBitHexString(int width, const T &out) noexcept {
    bitset<CHAR_BIT * sizeof(T)> bs(out);
    auto val = static_cast<int>(out);
    std::stringstream ss;
    ss //<< showbase // show the 0x prefix, does not work for zeros (0)
            << internal // fill between the prefix and the number
            << setfill('0'); // fill with 0s

    ss << "0x"
       << std::setw(width) << std::hex << val << " : "
       << std::setw(width) << std::dec << val << " : "
       << bs << std::endl;

    return ss.str();
}

template<class T>
std::string asBitString(const T &out) noexcept {
    bitset<CHAR_BIT * sizeof(T)> bs(out);
    std::stringstream ss;
    ss << bs;
    return ss.str();
}

template<class T>
std::string asHexString(int width, const T &out) noexcept {
    bitset<CHAR_BIT * sizeof(T)> bs(out);
    auto val = static_cast<int>(out);
    std::stringstream ss;
    ss //<< showbase // show the 0x prefix, does not work for zeros (0)
            << internal // fill between the prefix and the number
            << setfill('0'); // fill with 0s
    ss << "0x"
       << std::setw(width) << std::hex << val;
    return ss.str();
}

template<class T>
std::string asDecString(int width, const T &out) noexcept {
    bitset<CHAR_BIT * sizeof(T)> bs(out);
    auto val = static_cast<int>(out);
    std::stringstream ss;
    ss //<< showbase // show the 0x prefix, does not work for zeros (0)
            << internal // fill between the prefix and the number
            << setfill('_') // fill with 0s
            << std::setw(width) << std::dec << val;
    return ss.str();
}


#endif //CHIP8EMULATOR_TYPES_H
