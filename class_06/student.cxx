#include <iostream>
#include <string>
#include <string_view>
#include <format>

#include "../include/utility.hxx"

struct Person {
protected:
    std::string m_name;
    int m_age;
    double m_height;
    double m_weight;

public:
    Person(std::string_view name, int age, double height, double weight)
        : m_name(name), m_age(age), m_height(height), m_weight(weight)
    {}
    virtual ~Person() = default;
    virtual fn to_string() const noexcept -> std::string
    {
        return std::format(
            "{{\n"
            "  \"Name\": \"{}\"\n"
            "  \"Age\": {}\n"
            "  \"Height\": {:.2f}\n"
            "  \"Weight\": {:.2f}\n"
            "}}",
            m_name, m_age, m_height, m_weight
        );
    }
};

struct Student : public Person {
protected:
    std::string m_school;
    std::string m_major;
    std::string m_student_id;

public:
    Student(std::string_view name, int age, double height, double weight, std::string_view school, std::string_view major, std::string_view student_id)
        : Person(name, age, height, weight), m_school(school), m_major(major), m_student_id(student_id)
    {}
    virtual ~Student() = default;
    virtual fn to_string() const noexcept -> std::string override
    {
        return std::format(
            "{{\n"
            "  \"Name\": \"{}\"\n"
            "  \"Age\": {}\n"
            "  \"Height\": {:.2f}\n"
            "  \"Weight\": {:.2f}\n"
            "  \"School\": \"{}\"\n"
            "  \"Major\": \"{}\"\n"
            "  \"Student ID\": \"{}\"\n"
            "}}",
            m_name, m_age, m_height, m_weight, m_school, m_major, m_student_id
        );
    }
};

int main()
{
    Student s1{ "John Doe", 20, 1.75, 70.5, "Xiamen University Malaysia", "Computer Science", "CST2209399" };
    std::cout << sizeof(Student) << std::endl << s1.to_string() << std::endl;
}
