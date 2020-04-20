/**
 * \file Contact.h
 * \class Contact
 *
 * \defgroup Contact Contact
 * \brief The package handling the declaration and maintenance of emergency contact information
 * \ingroup Contact
 *
 * \brief The Contact class handling the declaration and maintenance of emergency contact information
 *
 * The Contact class creates the objects containing the information of emergency contacts that must be notified when the
 * device leaves a designated fence area/safe zone, and manages this information's conversion to JSON format for the database online.
 *
 * \author BeeSafe Team, Team 13
 *
 * \version v1.0
 *
 * \date 2020/04/20
 *
 * Contact: beesafe.uofg@gmail.com
 *
 * Licence: MIT
 */

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

    // Getters and setters for the contact details
    const std::string& getForename();
    const std::string& getSurname();
    const std::string& getNumber();
    const std::string& getKey();

    // Serialise the contact into a JSON element.
    web::json::value serialiseContact();

private:

    // Contact detail attributes
    std::string forename;
    std::string surname;
    std::string number;
    std::string key;

};

#endif //BEESAFEPI_CONTACT_H
