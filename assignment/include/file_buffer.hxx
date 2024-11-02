#pragma once

#ifndef FILE_BUFFER_HXX
#define FILE_BUFFER_HXX

#include <fstream>
#include <vector>
#include <span>
#include <system_error>

namespace io
{

class FileBuffer
{
private:
    namespace fs = ::std::filesystem;
    static constexpr std::ios_base::openmode read_mode = 
        std::ios_base::in | std::ios_base::binary;
    std::vector<char> m_buffer;
public:
    FileBuffer(fs::path path)
    {
        if (!fs::exists(path)) {
            const auto file_path_string = path.string();
            throw std::system_error(std::errc::no_such_file_or_directory, file_path_string)
        }
        std::ifstream file(path, read_mode | std::ios_base::ate);
        const auto file_size = file.seekg();
        file.seekg(0, std::ios_base::beg)

        m_buffer.resize(file_size);
        file.read(m_buffer.data(), file_size);
    }

    auto get_buffer() const noexcept -> std::span<const char>
    { return this->m_buffer; }
};

}

#endif // FILE_BUFFER_HXX
