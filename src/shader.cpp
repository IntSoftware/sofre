#include <sofre/shader.hpp>
#include <sofre/log.hpp>

#include <filesystem>
#include <fstream>

namespace sofre {

Shader::Shader(ShaderType type, std::filesystem::path sourceFile) : m_type(type) {
    size_t size = std::filesystem::file_size(sourceFile);
    size_t offset = 0;

    m_source.clear();
    char* buffer = new char[size];

    std::ifstream file(sourceFile, std::ios::in | std::ios::binary);
    
    if (!file || !file.read(buffer, size)) {
        delete[] buffer;
        m_source = "";
        Log::error("Failed to load shader source file: " + sourceFile.string());
        return;
    }

    if (size >= 3 &&
        (unsigned char)buffer[0] == 0xEF &&
        (unsigned char)buffer[1] == 0xBB &&
        (unsigned char)buffer[2] == 0xBF) {
        //UTF8_BOM
        offset = 3; //remove BOM
        utf8_to_ascii(buffer + offset, size - offset, m_source);
    }
    else if (size >= 2 &&
        (unsigned char)buffer[0] == 0xFF &&
        (unsigned char)buffer[1] == 0xFE) {
        //UTF16_LE
        offset = 2; //remove BOM
        utf16_to_ascii(buffer + offset, size - offset, m_source, true);
    }
    else if (size >= 2 &&
        (unsigned char)buffer[0] == 0xFE &&
        (unsigned char)buffer[1] == 0xFF) {
        //UTF16_BE
        offset = 2; //remove BOM
        utf16_to_ascii(buffer + offset, size - offset, m_source, false);
    }
    else if (size >= 4 &&
        (unsigned char)buffer[0] == 0xFF &&
        (unsigned char)buffer[1] == 0xFE &&
        (unsigned char)buffer[2] == 0x00 &&
        (unsigned char)buffer[3] == 0x00) {
        //UTF32_LE
        offset = 4; //remove BOM
        utf32_to_ascii(buffer + offset, size - offset, m_source, true);
    }
    else if (size >= 4 &&
        (unsigned char)buffer[0] == 0x00 &&
        (unsigned char)buffer[1] == 0x00 &&
        (unsigned char)buffer[2] == 0xFE &&
        (unsigned char)buffer[3] == 0xFF) {
        //UTF32_BE
        offset = 4; //remove BOM
        utf32_to_ascii(buffer + offset, size - offset, m_source, false);
    }
    else {
        // assum ASCII/UTF8 without BOM. if else, glsl compile error may occur.
        m_source.assign(buffer, size);
    }

    delete[] buffer;
}

void Shader::utf8_to_ascii(std::string& utf8Str) {
    std::replace_if(utf8Str.begin(), utf8Str.end(),
        [](unsigned char ch) { return ch & 0x80; }, ' ');
}
void Shader::utf8_to_ascii(const char* data, size_t size, std::string& out) {
    out.reserve(size);
    for(size_t i = 0; i < size; ++i) {
        unsigned char ch = (unsigned char)data[i];
        if (ch <= 0x7F)
            out.push_back((char)ch);
    }
}

void Shader::utf16_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian) {
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

void Shader::utf32_to_ascii(const char* data, size_t size, std::string& out, bool littleEndian) {
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
