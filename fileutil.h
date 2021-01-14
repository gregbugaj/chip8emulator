#ifndef CHIP8EMULATOR_FILEUTIL_H
#define CHIP8EMULATOR_FILEUTIL_H

#include <string>
#include <vector>
#include "types.h"

/**
 * Validate that the file exists
 *
 * @param filename
 */
void validateFileExists(const std::string &filename);

/**
 * Read file as byte vector
 * @param filename
 * @return
 */
std::vector<byte_t> readAsByteVector(const std::string &filename);

#endif //CHIP8EMULATOR_FILEUTIL_H
