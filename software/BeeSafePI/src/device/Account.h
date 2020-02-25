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

    // Constructors and destructors..
public:
    Account(std::vector<Contact*>& contacts, std::vector<Fence*>& fences);
    ~Account();

    // Getters and setters.
public:
    const std::vector<Contact*>& getContacts();
    const std::vector<Fence*>& getFences();

    // Object behaviour.
public:
    bool save(std::string path);

    // Hidden object behaviour.
private:
    bool saveContacts(web::json::value& rootElement);
    bool saveFences(web::json::value& rootElement);

    // Attributes.
private:
    std::vector<Contact*> contacts;
    std::vector<Fence*> fences;

};

#endif //BEESAFEPI_ACCOUNT_H
