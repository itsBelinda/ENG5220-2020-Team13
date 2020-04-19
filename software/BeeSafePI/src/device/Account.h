/**
 * \file Account.h
 * \class Account
 *
 * \defgroup Device Device
 * \brief The package handling the account creation and management
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


#ifndef BEESAFEPI_ACCOUNT_H
#define BEESAFEPI_ACCOUNT_H

#include "../contact/Contact.h"
#include "../geo/Fence.h"

// Library inclusions.
#include <cpprest/json.h>

// System inclusions.
#include <string>
#include <vector>

// Define the keys used by the device for json files.
#define JSON_KEY_ACCOUNT_NAME "Name"
#define JSON_KEY_ACCOUNT_CONTACTS "Contacts"
#define JSON_KEY_ACCOUNT_FENCES "Fences"

// Account defines the general device device for the parent.
class Account
{

public:

    // Constructors / destructors.
    Account(std::string &name, std::vector<Contact*>& contacts, std::vector<Fence*>& fences);
    Account(const Account &account);
    ~Account();

public:

    // Get the name of the account.
    std::string &getName();

    // Get the contacts and fences.
    std::vector<Contact*>& getContacts();
    std::vector<Fence*>& getFences();

    // Account serialisation.
    web::json::value serialiseAccount();
    web::json::value serialiseAccountContacts();
    web::json::value serialiseAccountFences();
    bool saveSerialisedAccount(const std::string& path);

private:

    // The name of the account.
    std::string name;

    // The contacts and fences for the account.
    std::vector<Contact*> contacts;
    std::vector<Fence*> fences;

};

#endif //BEESAFEPI_ACCOUNT_H
