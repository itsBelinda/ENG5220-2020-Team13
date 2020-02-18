#ifndef BEESAFEPI_CONTACT_H
#define BEESAFEPI_CONTACT_H

#include <string>

// Json keys for the contact class.
#define CONTACT_KEY_FORENAME "Forename"
#define CONTACT_KEY_SURNAME "Surname"
#define CONTACT_KEY_NUMBER "Number"
#define CONTACT_KEY_KEY "Key"

// Contact class defines a legal guardian for the child.
class Contact
{

public:
    Contact(std::string forename, std::string surname, std::string number, std::string key);

public:
    std::string getForename();
    std::string getSurname();
    std::string getNumber();
    std::string getKey();

private:
    std::string forename;
    std::string surname;
    std::string number;
    std::string key;

};

#endif //BEESAFEPI_CONTACT_H
