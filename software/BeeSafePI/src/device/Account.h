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
