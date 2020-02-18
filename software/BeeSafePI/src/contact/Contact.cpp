#include "Contact.h"

// Constructor defines a contact for the device.
Contact::Contact(std::string forename, std::string surname,
                 std::string number, std::string key)
{
    this->forename = forename;
    this->surname = surname;
    this->number = number;
    this->key = key;
}

// Get the first name of a legal guardian contact.
std::string Contact::getForename()
{
    return forename;
}

// Get the last name of a legal guardian contact.
std::string Contact::getSurname()
{
    return surname;
}

// Get the phone number of a legal guardian contact.
std::string Contact::getNumber()
{
    return number;
}

// Get the live location API key for that contact.
std::string Contact::getKey()
{
    return key;
}