#include "Contact.h"

// Constructor for device contact.
Contact::Contact(const std::string& forename, const std::string& surname,
                 const std::string& number, const std::string& key)
{
    this->forename = forename;
    this->surname = surname;
    this->number = number;
    this->key = key;
}

// The destructor used by the contact.
Contact::~Contact() = default;

// The first name of the contact.
const std::string& Contact::getForename()
{
    return forename;
}

// The last name of the contact.
const std::string& Contact::getSurname()
{
    return surname;
}

// The number of the contact.
const std::string& Contact::getNumber()
{
    return number;
}

// The API key of the contact.
const std::string& Contact::getKey()
{
    return key;
}

// Serialise the contact into a JSON element.
web::json::value Contact::serialiseContact()
{
    web::json::value jsonContact = web::json::value::object();
    jsonContact[utility::string_t(U(JSON_KEY_CONTACT_FORENAME))] = web::json::value::string(U(forename));
    jsonContact[utility::string_t(U(JSON_KEY_CONTACT_SURNAME))] = web::json::value::string(U(surname));
    jsonContact[utility::string_t(U(JSON_KEY_CONTACT_NUMBER))] = web::json::value::string(U(number));
    jsonContact[utility::string_t(U(JSON_KEY_CONTACT_KEY))] = web::json::value::string(U(key));
    return jsonContact;
}