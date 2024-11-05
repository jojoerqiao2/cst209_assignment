#pragma once

#ifndef FILE_BUFFER_HXX
#define FILE_BUFFER_HXX

#include <filesystem>
#include <vector>
#include <string_view>

namespace io
{

class FileBuffer
{
#define fs ::std::filesystem
private:
    static constexpr std::ios_base::openmode read_mode = 
        std::ios_base::in | std::ios_base::binary;
    std::vector<char> m_buffer;
public:
    FileBuffer(fs::path path);

    auto buffer() const noexcept -> std::string_view
    { return std::string_view(this->m_buffer.data(), this->m_buffer.size()); }
#undef fs
};

}

#endif // FILE_BUFFER_HXX
