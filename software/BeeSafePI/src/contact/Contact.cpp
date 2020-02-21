#include "Contact.h"

// Constructor for account contact.
Contact::Contact(const std::string& forename, const std::string& surname,
                 const std::string& number, const std::string& key)
{
    this->forename = forename;
    this->surname = surname;
    this->number = number;
    this->key = key;
}

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