#ifndef BEESAFEPI_ACCOUNT_H
#define BEESAFEPI_ACCOUNT_H

#include "../contact/Contact.h"
#include "../geo/Fence.h"

#include <string>
#include <vector>

// Define the keys used by the device for json files.
#define JSON_KEY_ACCOUNT_CONTACTS "Contacts"
#define JSON_KEY_ACCOUNT_FENCES "Fences"

// Account defines the general device device for the parent.
class Account
{

public:

    // Constructors.
    Account(std::vector<Contact*>& contacts, std::vector<Fence*>& fences);

    // Destructors.
    ~Account();

public:

    // Getters and setters.
    const std::vector<Contact*>& getContacts();
    const std::vector<Fence*>& getFences();

    // Object behaviour.
    web::json::value serialiseAccount();
    web::json::value serialiseAccountContacts();
    web::json::value serialiseAccountFences();
    bool saveSerialisedAccount(const std::string& path);

private:

    // Attributes.
    std::vector<Contact*> contacts;
    std::vector<Fence*> fences;

};

#endif //BEESAFEPI_ACCOUNT_H
