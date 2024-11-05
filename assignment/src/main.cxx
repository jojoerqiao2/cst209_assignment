#include "csv.hxx"
#include "file_buffer.hxx"

#include <exception>
#include <iostream>
#include <variant>

union JustForTest {
    io::FileBuffer file_buffer;
    csv::Parser csv_parser;

    JustForTest() noexcept {}
    ~JustForTest() noexcept {}
};
[[maybe_unused]]
static const JustForTest test;

int main()
{
    io::FileBuffer postal_code_CN("assets/postcode_CN.csv");
    io::FileBuffer fmt_postal_code("assets/postcode.fmt.csv");
    csv::Parser parser(
        std::string_view(fmt_postal_code.buffer())
    );
    try {
        parser.append_data(std::string_view(postal_code_CN.buffer()));
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    for (auto const& field_name : parser.field_names()) {
        static bool is_first = true;
        std::cout << (is_first ? "" : ", ") << '\"' << field_name << '\"';
        is_first = false;
    }
    std::cout << std::endl;

    for (std::size_t i = 0; i < parser.record_count(); ++i) {
        bool is_first = true;
        for (std::size_t j = 0; j < parser.field_count(); ++j) {
            using namespace std::string_literals;
            csv::Type::parsed_data_type value{};
            try {
                value = parser.parse_record_at(i, j);
            } catch (std::exception const& e) {
                std::cerr << e.what() << std::endl;
            }
            std::cout << (is_first ? "" : ", ")
                << '\"'
                << std::visit(
                    [](auto&& arg) -> std::string {
                        using T = std::decay_t<decltype(arg)>;
                        if constexpr (std::is_same_v<T, csv::Parser::string>) {
                            return arg;
                        } else if constexpr (std::is_same_v<T, csv::Parser::integer>) {
                            return std::to_string(arg);
                        } else if constexpr (std::is_same_v<T, csv::Parser::invalid>) {
                            return "INVALID";
                        } else {
                            static_assert(false, "Impossible to reach here!");
                        }
                    }, value
                )
                << '\"';
            is_first = false;
        }
        std::cout << std::endl;
    }
    return 0;
}
