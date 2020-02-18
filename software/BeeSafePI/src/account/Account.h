#ifndef BEESAFEPI_ACCOUNT_H
#define BEESAFEPI_ACCOUNT_H

#include "../contact/Contact.h"
#include "../geo/Fence.h"

#include <string>
#include <vector>

// Define the keys used by the account for json files.
#define JSON_KEY_ACCOUNT_CONTACTS "Contacts"
#define JSON_KEY_ACCOUNT_FENCES "Fences"

// Account defines the general device account for the parent.
class Account
{

public:
    Account(std::vector<Contact>* contacts, std::vector<Fence>* fences);

public:
    std::vector<Contact>* getContacts();
    std::vector<Fence>* getFences();

public:
    bool isValid();
    bool save(std::string path);

private:
    std::vector<Contact>* contacts;
    std::vector<Fence>* fences;

};

#endif //BEESAFEPI_ACCOUNT_H
