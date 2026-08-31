#include <sofre/shader.hpp>
#include <sofre/log.hpp>

#include <glutil/shader.hpp>

#include <string>
#include <algorithm>
#include <filesystem>
#include <fstream>

namespace sofre {

using namespace shader;
    
std::string shader::readFile(const std::filesystem::path& sourceFile, bool isUTF8withoutBOM) {
    if (isUTF8withoutBOM) {
        auto result = glutil::ShaderLoader::loadFile(sourceFile);
        if (!result.ok) {
            Log::error(result.error);
            return {};
        }
        return std::string(*result.string(), static_cast<size_t>(result.length()));
    }

    auto result = glutil::ShaderLoader::loadFile(sourceFile);
    if (!result.ok) {
        Log::error(result.error);
        return {};
    }
    return std::string(*result.string(), static_cast<size_t>(result.length()));
}

void shader::utf8_to_ascii_replace(std::string& utf8Str) {
    std::replace_if(utf8Str.begin(), utf8Str.end(),
        [](unsigned char ch) { return ch & 0x80; }, ' ');
}
void shader::utf8_to_ascii(const char* data, size_t size, std::string& out) {
    out.clear();
    out.reserve(size);
    for(size_t i = 0; i < size; ++i) {
        unsigned char ch = (unsigned char)data[i];
        if (ch <= 0x7F)
            out.push_back((char)ch);
    }
}

void shader::utf16_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian) {
    out.clear();
    out.reserve(size / 2);

    for (size_t i = 0; i + 1 < size;) {
        uint16_t u;

        if (littleEndian)
            u = (uint8_t)data[i] | ((uint8_t)data[i + 1] << 8);
        else
            u = ((uint8_t)data[i] << 8) | (uint8_t)data[i + 1];

        i += 2;

        // surrogate pair (skip)
        if (u >= 0xD800 && u <= 0xDFFF) {
            if (i + 1 < size)
                i += 2;
            continue;
        }

        if (u <= 0x7F)
            out.push_back((char)u);
    }
}

void shader::utf32_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian) {
    out.clear();
	out.reserve(size / 4);

	for (size_t i = 0; i + 3 < size; i += 4) {
		uint32_t u;

		if (littleEndian)
			u = (uint8_t)data[i]
			  | ((uint8_t)data[i + 1] << 8)
			  | ((uint8_t)data[i + 2] << 16)
			  | ((uint8_t)data[i + 3] << 24);
		else
			u = ((uint8_t)data[i] << 24)
			  | ((uint8_t)data[i + 1] << 16)
			  | ((uint8_t)data[i + 2] << 8)
			  | (uint8_t)data[i + 3];

		if (u <= 0x7F)
			out.push_back((char)u);
	}
}

} // namespace sofre
