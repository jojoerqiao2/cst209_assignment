#include "file_buffer.hxx"

#include <fstream>
#include <system_error>
#include <bit>

namespace io
{
#define fs ::std::filesystem

FileBuffer::FileBuffer(fs::path path)
    : m_buffer()
{
    if (!fs::exists(path)) {
        const auto file_path_string = path.string();
        throw std::system_error(
            std::make_error_code(std::errc::no_such_file_or_directory), 
            file_path_string
        );
    }
    std::ifstream file(path, read_mode | std::ios_base::ate);
    const std::size_t file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);

    m_buffer.resize(std::bit_ceil(file_size));
    file.read(m_buffer.data(), file_size);
}

#undef fs
}
