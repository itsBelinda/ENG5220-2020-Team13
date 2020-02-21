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
    Contact(const std::string& forename,
            const std::string& surname,
            const std::string& number,
            const std::string& key);

    // Getters and setters.
public:
    const std::string& getForename();
    const std::string& getSurname();
    const std::string& getNumber();
    const std::string& getKey();

    // Attributes.
private:
    std::string forename;
    std::string surname;
    std::string number;
    std::string key;

};

#endif //BEESAFEPI_CONTACT_H
