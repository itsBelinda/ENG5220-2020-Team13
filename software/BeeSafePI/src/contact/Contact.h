#ifndef BEESAFEPI_CONTACT_H
#define BEESAFEPI_CONTACT_H

#include <string>

// JSON keys for accessing contact attributes.
#define JSON_KEY_CONTACT_FORENAME "Forename"
#define JSON_KEY_CONTACT_SURNAME "Surname"
#define JSON_KEY_CONTACT_NUMBER "Number"
#define JSON_KEY_CONTACT_KEY "Key"

// Contact class defines a legal guardian for the child.
class Contact
{

    // Contact constructor.
public:
    Contact(std::string& forename, std::string& surname,
            std::string& number, std::string& key);

    // Getters and setters.
public:
    std::string& getForename();
    std::string& getSurname();
    std::string& getNumber();
    std::string& getKey();

    // Attributes.
private:
    std::string forename;
    std::string surname;
    std::string number;
    std::string key;

};

#endif //BEESAFEPI_CONTACT_H
