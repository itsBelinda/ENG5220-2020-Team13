#include <cpprest/json.h>
#include "Account.h"

#include "../geo/RoundFence.h"
#include "../geo/PolyFence.h"

// Constructor used to create an instance of the Account class.
Account::Account(std::vector<Contact*>& contacts, std::vector<Fence*>& fences)
{
    this->contacts = contacts;
    this->fences = fences;
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

// Save the device as a JSON file at a given path.
bool Account::save(std::string path)
{
    web::json::value root;

    // Save the contact details to the root.
    if (saveContacts(root) && saveFences(root)) {
        return false;
    }

    // Write the root to a string stream.


    return true;
}

// Responsible for creating the JSON array that represents the contacts.
bool Account::saveContacts(web::json::value &rootElement)
{
    for (int i = 0; i < contacts.size(); ++i) {
        rootElement[U(JSON_KEY_ACCOUNT_CONTACTS)][i][U(JSON_KEY_CONTACT_FORENAME)] = web::json::value(U(contacts[i]->getForename()));
        rootElement[U(JSON_KEY_ACCOUNT_CONTACTS)][i][U(JSON_KEY_CONTACT_SURNAME)] = web::json::value(U(contacts[i]->getSurname()));
        rootElement[U(JSON_KEY_ACCOUNT_CONTACTS)][i][U(JSON_KEY_CONTACT_SURNAME)] = web::json::value(U(contacts[i]->getNumber()));
        rootElement[U(JSON_KEY_ACCOUNT_CONTACTS)][i][U(JSON_KEY_CONTACT_SURNAME)] = web::json::value(U(contacts[i]->getKey()));
    }
    return true;
}

// Responsible for creating a JSON array of fences.
bool Account::saveFences(web::json::value &rootElement)
{

    // Days of the week.
    const std::string days[] = JSON_KEY_FENCE_DAYS;

    // Week date related information.
    const std::map<int, std::vector<std::pair<std::tm, std::tm>>>* week = nullptr;
    const std::vector<std::pair<std::tm, std::tm>>* times = nullptr;
    const std::vector<std::pair<double, double>>* coordinates = nullptr;

    // Dynamic conversion pointers.
    RoundFence* roundFence = nullptr;
    PolyFence* polyFence = nullptr;

    // Time buffer.
    char timeBuffer[6];

    // Save the fences to the file iteratively.
    for (int i = 0; i < fences.size(); ++i) {

        // Save whether or not the fence is safe.
        rootElement[U(JSON_KEY_ACCOUNT_FENCES)][i][U(JSON_KEY_FENCE_SAFE)] = web::json::value(fences[i]->isSafe());

        // Save the timing related information for the fence.
        week = &fences[i]->getWeek();
        for (auto& day : *week) {
            times = &day.second;
            for (int j = 0; j < times->size(); ++j) {
                rootElement[U(JSON_KEY_ACCOUNT_FENCES)][i][U(JSON_KEY_FENCE_WEEK)][days[day.first]][j][U(JSON_KEY_FENCE_TIME_FROM)] =
                rootElement[U(JSON_KEY_ACCOUNT_FENCES)][i][U(JSON_KEY_FENCE_WEEK)][days[day.first]][j][U(JSON_KEY_FENCE_TIME_TO)] =
            }
        }

        // Dynamically cast the fences.
        if (roundFence = dynamic_cast<RoundFence*>(fences[i])) {

            // We have successfully cast to a round fence ptr.
            rootElement[U(JSON_KEY_ACCOUNT_FENCES)][i][U(JSON_KEY_FENCE_FENCE)][U(JSON_KEY_ROUND_FENCE_LATITUDE)]
                    = web::json::value(roundFence->getLatitude());
            rootElement[U(JSON_KEY_ACCOUNT_FENCES)][i][U(JSON_KEY_FENCE_FENCE)][U(JSON_KEY_ROUND_FENCE_LONGITUDE)]
                    = web::json::value(roundFence->getLongitude());
            rootElement[U(JSON_KEY_ACCOUNT_FENCES)][i][U(JSON_KEY_FENCE_FENCE)][U(JSON_KEY_ROUND_FENCE_RADIUS)]
                    = web::json::value(roundFence->getRadius());

        } else if (polyFence = dynamic_cast<PolyFence*>(fences[i])) {

            // We have successfully cast to a poly fence ptr.
            coordinates = &polyFence->getCoordinates();
            for (int k = 0; k < coordinates->size(); ++k) {
                rootElement[U(JSON_KEY_ACCOUNT_FENCES)][i][U(JSON_KEY_FENCE_FENCE)][k][U(JSON_KEY_POLY_FENCE_LATITUDE)]
                        = web::json::value((*coordinates)[k].first);
                rootElement[U(JSON_KEY_ACCOUNT_FENCES)][i][U(JSON_KEY_FENCE_FENCE)][k][U(JSON_KEY_POLY_FENCE_LONGITUDE)]
                        = web::json::value((*coordinates)[k].second);
            }

        } else {
            return false;
        }
    }

    return true;
}