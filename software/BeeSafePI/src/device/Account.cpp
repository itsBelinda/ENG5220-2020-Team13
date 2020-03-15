#include "Account.h"

#include "../geo/RoundFence.h"
#include "../geo/PolyFence.h"

#include <iostream>
#include <cpprest/json.h>

// Constructor used to create an instance of the Account class.
Account::Account(std::vector<Contact*>& contacts, std::vector<Fence*>& fences)
{
    this->contacts = contacts;
    this->fences = fences;
}

// Copy constructor for the account object.
Account::Account(const Account &account)
{
    this->contacts = account.contacts;
    this->fences = account.fences;
}

// Destructor used to clear all the memory occupied by the account.
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

// Get the vector containing the contact details for the device.
const std::vector<Contact*>& Account::getContacts()
{
    return contacts;
}

// Get the vector of geo fences for the device.
const std::vector<Fence*>& Account::getFences()
{
    return fences;
}

// Serialise the account object.
web::json::value Account::serialiseAccount()
{
    web::json::value jsonAccount = web::json::value::object();
    jsonAccount[utility::string_t(U(JSON_KEY_ACCOUNT_CONTACTS))] = serialiseAccountContacts();
    jsonAccount[utility::string_t(U(JSON_KEY_ACCOUNT_FENCES))] = serialiseAccountFences();
    return jsonAccount;
}

// Serialise the contacts for this account.
web::json::value Account::serialiseAccountContacts()
{
    web::json::value jsonContacts = web::json::value::array();
    for (int i = 0; i < contacts.size(); ++i) {
        jsonContacts[i] = contacts[i]->serialiseContact();
    }
    return jsonContacts;
}

// Serialise the fences for this account.
web::json::value Account::serialiseAccountFences()
{
    web::json::value jsonFences = web::json::value::array();
    for (int i = 0; i < fences.size(); ++i) {
        jsonFences[i] = fences[i]->serialiseFence();
    }
    return jsonFences;
}

// Serialise the json account instance and output it.
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