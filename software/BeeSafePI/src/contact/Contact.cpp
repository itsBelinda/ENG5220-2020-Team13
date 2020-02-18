#include "Contact.h"

// Constructor for account contact.
Contact::Contact(std::string& forename, std::string& surname,
                 std::string& number, std::string& key)
{
    this->forename = forename;
    this->surname = surname;
    this->number = number;
    this->key = key;
}

// The first name of the contact.
std::string& Contact::getForename()
{
    return forename;
}

// The last name of the contact.
std::string& Contact::getSurname()
{
    return surname;
}

// The number of the contact.
std::string& Contact::getNumber()
{
    return number;
}

// The API key of the contact.
std::string& Contact::getKey()
{
    return key;
}