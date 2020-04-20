/**
 * \file Contact.cpp
 * \class Contact
 *
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


#include "Contact.h"

/**
 * The explicit constructor for the Contact class
 *
 * @param forename: The contact's forename
 * @param surname: The contact's surname
 * @param number: The contact's phone number
 * @param key: The user/contact's API key
 */
Contact::Contact(const std::string& forename, const std::string& surname,
                 const std::string& number, const std::string& key)
{
    this->forename = forename;
    this->surname = surname;
    this->number = number;
    this->key = key;
}

/**
 * The Contact class copy constructor.
 *
 * @param contact: the contact object to be copied
 */
Contact::Contact(const Contact &contact)
{
    this->forename = contact.forename;
    this->surname = contact.surname;
    this->number = contact.number;
    this->key = contact.key;
}

/**
 * The Contact class destructor.
 */
Contact::~Contact()
{};

/**
 * A getter for the first name of the contact.
 * A public accessor for an otherwise private variable.
 *
 * @return the contact's forename
 */
const std::string& Contact::getForename()
{
    return forename;
}

/**
 * A getter for the last name of the contact.
 * A public accessor for an otherwise private variable.
 *
 * @return the contact's surname.
 */
const std::string& Contact::getSurname()
{
    return surname;
}

/**
 * A getter for the number of the contact.
 * A public accessor for an otherwise private variable.
 *
 *  @return the contact phone number
 */
const std::string& Contact::getNumber()
{
    return number;
}

/**
 * A getter for the API key of the contact.
 * A public accessor for an otherwise private variable.
 *
 * @return the contact/user API key
 */
const std::string& Contact::getKey()
{
    return key;
}

/**
 * A method to serialise the contact into a JSON element.
 *
 * @return a JSON object containing the Contact information
 */
web::json::value Contact::serialiseContact()
{
    web::json::value jsonContact = web::json::value::object();
    jsonContact[utility::string_t(U(JSON_KEY_CONTACT_FORENAME))] = web::json::value::string(U(forename));
    jsonContact[utility::string_t(U(JSON_KEY_CONTACT_SURNAME))] = web::json::value::string(U(surname));
    jsonContact[utility::string_t(U(JSON_KEY_CONTACT_NUMBER))] = web::json::value::string(U(number));
    jsonContact[utility::string_t(U(JSON_KEY_CONTACT_KEY))] = web::json::value::string(U(key));
    return jsonContact;
}