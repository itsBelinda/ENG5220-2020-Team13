#include "Account.h"

#include "../geo/RoundFence.h"
#include "../geo/PolyFence.h"

// Constructor used to create an instance of the Account class.
Account::Account(std::vector<Contact>* contacts, std::vector<Fence>* fences)
{
    this->contacts = contacts;
    this->fences = fences;
}

// Get the vector containing the contact details for the account.
std::vector<Contact>* Account::getContacts()
{
    return contacts;
}

// Get the vector of geo fences for the account.
std::vector<Fence>* Account::getFences()
{
    return fences;
}

// Checks whether or not the account and subsequent details are valid.
bool Account::isValid()
{
    return true;
}

// Save the account as a JSON file at a given path.
bool Account::save(std::string path)
{
    return false;
}