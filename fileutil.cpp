#include "fileutil.h"
#include "types.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>

void validateFileExists(const std::string &filename) {
    std::ifstream f(filename);
    if (!f.good()) {
        std::stringstream ss;
        ss << "File not found : " << filename;
        throw std::runtime_error(ss.str());
    }
}

std::vector<byte_t> readAsByteVector(const std::string &filename) {
    // binary mode is only for switching off newline translation
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    file.unsetf(std::ios::skipws);
    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<byte_t> vec(file_size);
    vec.insert(vec.begin(), std::istream_iterator<byte_t>(file), std::istream_iterator<byte_t>());
    //    auto src = (char*) new char[vec.size()];
    //    std::copy(vec.begin(), vec.end(), src);
    return vec;
}