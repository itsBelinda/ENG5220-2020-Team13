/**
 * \file Account.cpp
 * \class Account
 *
 * \ingroup Device
 *
 * \brief The Account class creates the overall user Account object with the relevant Fences and Contacts
 *
 * The Account class creates the overall user Account object. This includes the Fences specified by the user with all their
 * relevant details, as well as the Contacts who need to be notified in case the device leaves or heads into a Fence when
 * not supposed to. The Account class is also the overall manager of JSON-C++ object conversions, facilitating the creation of
 * the overall JSON data file related to a user.
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

#include "Account.h"

#include "../geo/RoundFence.h"
#include "../geo/PolyFence.h"

#include <iostream>
#include <cpprest/json.h>

/**
 * Constructor used to create an instance of the Account class.
 *
 * @param name: The name of the account
 * @param contacts: the contacts added to this account
 * @param fences: the fences set up for this account
 */
Account::Account(std::string &name, std::vector<Contact*>& contacts,
                 std::vector<Fence*>& fences)
{
    this->name = name;
    this->contacts = contacts;
    this->fences = fences;
}

/**
 * Copy constructor for the account object.
 *
 * @param account: the account to be copied
 */
Account::Account(const Account &account)
{
    this->name = name;
    this->contacts = account.contacts;
    this->fences = account.fences;
}

/**
 * Destructor used to clear all the memory occupied by the account.
 */
Account::~Account()
{

    // Free all of the contacts.
    for (auto& contact : contacts) {
        delete contact;
    }

    // Free all of the fences.
    for (auto& fence : fences) {
        delete fence;
    }
}

/**
 * A getter for the name of the account.
 * A public accessor to an otherwise private value.
 *
 * @return the name of the account.
 */
std::string& Account::getName()
{
    return name;
}

/**
 * Get the vector containing the contact details for the account.
 * A public accessor to an otherwise private value.
 *
 * @return a vector containing all the contact objects associated with this account
 */
std::vector<Contact*>& Account::getContacts()
{
    return contacts;
}

/**
 * Get the vector of the fences for this account.
 * A public accessor to an otherwise private value.
 *
 * @return a vector containing all the fence objects associated with this account
 */
std::vector<Fence*>& Account::getFences()
{
    return fences;
}

/**
 * Serialise the account object.
 *
 * @return a json object containing the account object
 */
web::json::value Account::serialiseAccount()
{
    web::json::value jsonAccount = web::json::value::object();
    jsonAccount[utility::string_t(U(JSON_KEY_ACCOUNT_NAME))]
            = web::json::value::string(U(name));
    jsonAccount[utility::string_t(U(JSON_KEY_ACCOUNT_CONTACTS))]
            = serialiseAccountContacts();
    jsonAccount[utility::string_t(U(JSON_KEY_ACCOUNT_FENCES))]
            = serialiseAccountFences();
    return jsonAccount;
}

/**
 * Serialise the contacts for this account.
 *
 * @return a json object containing the contacts for this account
 */
web::json::value Account::serialiseAccountContacts()
{
    web::json::value jsonContacts = web::json::value::array();
    for (int i = 0; i < contacts.size(); ++i) {
        jsonContacts[i] = contacts[i]->serialiseContact();
    }
    return jsonContacts;
}

/**
 * Serialise the fences for this account.
 *
 * @return a JSON object containing the fences for this account
 */
web::json::value Account::serialiseAccountFences()
{
    web::json::value jsonFences = web::json::value::array();
    for (int i = 0; i < fences.size(); ++i) {
        jsonFences[i] = fences[i]->serialiseFence();
    }
    return jsonFences;
}

/**
 * Serialise the json account instance and output it.
 *
 * @param path: the filepath of the JSON file to save the serialised account to
 * @return True if successful, false otherwise
 */
bool Account::saveSerialisedAccount(const std::string& path)
{

    // Serialise the account instance.
    web::json::value jsonAccount = serialiseAccount();

    // Output the serialised json account to a file stream.
    utility::ofstream_t of(path);
    of << jsonAccount.serialize();
    of.close();

    // We have successfully written to the file.
    return true;
}