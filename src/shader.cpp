#include <sofre/shader.hpp>

#include <fstream>

namespace sofre {

Shader::Shader(ShaderType type, std::filesystem::path sourceFile) : m_type(type) {
    std::ifstream file(sourceFile, std::ios::in | std::ios::binary);

    // read all, if BOM exists, replace
    m_source.assign(std::istreambuf_iterator<char>(file),
                    std::istreambuf_iterator<char>());

        
    if (m_source.size() >= 3 &&
        (unsigned char)m_source[0] == 0xEF &&
        (unsigned char)m_source[1] == 0xBB &&
        (unsigned char)m_source[2] == 0xBF) {
        //encoding = Encoding::UTF8_BOM;
        m_source = m_source.substr(3); //BOM 제거
    }
    else if (m_source.size() >= 2 &&
        (unsigned char)m_source[0] == 0xFF &&
        (unsigned char)m_source[1] == 0xFE) {
        //encoding = Encoding::UTF16_LE;
    }
    else if (m_source.size() >= 2 &&
        (unsigned char)m_source[0] == 0xFE &&
        (unsigned char)m_source[1] == 0xFF) {
        //encoding = Encoding::UTF16_BE;
    }
    else if (m_source.size() >= 4 &&
        (unsigned char)m_source[0] == 0xFF &&
        (unsigned char)m_source[1] == 0xFE &&
        (unsigned char)m_source[2] == 0x00 &&
        (unsigned char)m_source[3] == 0x00) {
        //encoding = Encoding::UTF32_LE;
    }
    else if (m_source.size() >= 4 &&
        (unsigned char)m_source[0] == 0x00 &&
        (unsigned char)m_source[1] == 0x00 &&
        (unsigned char)m_source[2] == 0xFE &&
        (unsigned char)m_source[3] == 0xFF) {
        //encoding = Encoding::UTF32_BE;
    }
    else {
        //encoding = Encoding::UTF8; // BOM 없는 UTF-8 가정
    }
}

std::string Shader::utf8TOascii(const std::string& utf8Str) {
    std::replace_if(utf8Str.begin(), utf8Str.end(),
        [](char ch) { return ch & 0x80; }, ' ');
}

} // namespace sofre
