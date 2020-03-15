#ifndef BEESAFEPI_CONTACT_H
#define BEESAFEPI_CONTACT_H

#include <string>
#include <cpprest/json.h>

// JSON keys for accessing contact attributes.
#define JSON_KEY_CONTACT_FORENAME "Forename"
#define JSON_KEY_CONTACT_SURNAME "Surname"
#define JSON_KEY_CONTACT_NUMBER "Number"
#define JSON_KEY_CONTACT_KEY "Key"

// Contact class defines a legal guardian for the child.
class Contact
{

public:

    // Constructors / destructors.
    Contact(const std::string& forename, const std::string& surname,
            const std::string& number, const std::string& key);
    Contact(const Contact &contact);
    ~Contact();

public:

    // Getters and setters for the contact.
    const std::string& getForename();
    const std::string& getSurname();
    const std::string& getNumber();
    const std::string& getKey();

    // Serialise the contact into a JSON element.
    web::json::value serialiseContact();

private:

    // Attributes.
    std::string forename;
    std::string surname;
    std::string number;
    std::string key;

};

#endif //BEESAFEPI_CONTACT_H
