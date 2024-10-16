#include <iostream>
#include <format>
#include <string>
#include <array>
#include <utility>
using namespace std;

#define let auto

class Person{
    private:
        string firstName;
        string lastName;
        string fullName;
	string passID;
	int idNumber;

        void generateFullName(){
            fullName = firstName + " " + lastName;
        }
	void generatePassID() {
	    passID = std::format("{}{}", lastName, idNumber);
	}
    public:
        Person(string fName, string lName, idNum)
		: firstName{std::move(fName)}, lastName{std::move(lName)}, idNumber{idNumber}
	{
            generateFullName();
	    generatePassID();
        }

        string getFirstName(){
            return firstName;
        }

        string getLastName(){
            return lastName;
        }

        string getFullName(){
            return fullName;
        }
	
	string getPassID() {
	    return passID;
	}

	int getIDNumber() {
            return idNumber;
	}
};

int main()
{
    int constexpr array_size = 3;
    std::array<Person, array_size> persons{
	    Person{"John", "Smith", 24577},
            Person{"Helen", "Keller", 98833},
	    Person{"Jack", "Babara", 63568},
    };
    for (let const& person : persons) {
	std::cout << std::format(
		"{}\n"
		"{}\n"
		"{}\n"
		"{}\n"
		"{}\n",
		person.getFirstName(), person.getLastName(), person.getFullName(), person.getIDNumber(), person.getPassID()
	);
    }
    return 0;
}

