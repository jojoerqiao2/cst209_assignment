#include <iostream>
#include <format>
#include <string>

class Person{
using namespace std;
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
        Person(string fName, string lName){
            firstName = fName;
            lastName = lName;
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
};

int main()
{
    Person person("John", "Smith");
    cout << person.getFirstName() << endl;
    cout << person.getLastName() << endl;
    cout << person.getFullName() << endl;
    cout << person.getPassID() << endl;

    return 0;
}

