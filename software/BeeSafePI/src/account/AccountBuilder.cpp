#include "AccountBuilder.h"

#include <string>

// Build an account by streaming the a file.
AccountBuilder::AccountBuilder(utility::stringstream_t &stream, std::error_code &errorCode)
{
    this->root = web::json::value::parse(stream, errorCode);
}

// Builds an instance of the Account class.
Account* AccountBuilder::build()
{

    // Check whether or not a contacts json array exists.
    web::json::value &jsonContactsValue = root.at(U(JSON_KEY_ACCOUNT_CONTACTS));
    if (jsonContactsValue.is_null() || !jsonContactsValue.is_array()) {
        return nullptr;
    }

    web::json::array &jsonContacts = jsonContactsValue.as_array();
    auto *accountContacts = new std::vector<Contact*>;

    // Parse the list of contacts creating them in the process.
    if (!buildContactsVector(jsonContacts, *accountContacts)) {
        delete accountContacts;
        return nullptr;
    }

    // Check whether or not the fence json array exists.
    web::json::value &jsonFencesValue = root.at(U(JSON_KEY_ACCOUNT_FENCES));
    if (jsonFencesValue.is_null() || !jsonFencesValue.is_array()) {
        delete accountContacts;
        return nullptr;
    }

    web::json::array &jsonFences = jsonFencesValue.as_array();
    auto *accountFences = new std::vector<Fence*>;

    // Parse the fences json, consequently creating an instance for each fence.
    if (!buildFenceVector(jsonFences, *accountFences)) {
        delete accountContacts;
        delete accountFences;
        return nullptr;
    }

    // We have successfully created the contacts and fences.
    return new Account(accountContacts, accountFences);
}

// Build the contact vector from the array.
bool AccountBuilder::buildContactsVector(web::json::array &jsonContacts, std::vector<Contact*> &contacts)
{
    Contact *contact = nullptr;
    for (auto &jsonContact : jsonContacts) {
        if (isContactStructure(jsonContact) || (contact = buildContact(jsonContact)) == nullptr) {
            return false;
        }
        contacts.push_back(contact);
    }
    return true;
}

// Build the vector containing the fences.
bool AccountBuilder::buildFenceVector(web::json::array &jsonFences, std::vector<Fence*> &fences)
{
    Fence *fence = nullptr;
    for (auto &jsonFence : jsonFences) {

        // Determine the type of geo fence being created.
        if (isGeoFenceStructure(jsonFence)) {
            fence = buildGeoFence(jsonFence);
        } else if (isGeoPathStructure(jsonFence)) {
            fence = buildGeoPath(jsonFence);
        } else {
            return false;
        }

        // Add it only if it was created successfully.
        if (fence == nullptr) {
            return false;
        }
        fences.push_back(fence);
    }
    return true;
}

// Build a Conatct instance from a json element.
Contact* AccountBuilder::buildContact(web::json::value &jsonContact)
{
    return new Contact(
            jsonContact.at(U(JSON_KEY_CONTACT_FORENAME)).as_string(),
            jsonContact.at(U(JSON_KEY_CONTACT_SURNAME)).as_string(),
            jsonContact.at(U(JSON_KEY_CONTACT_NUMBER)).as_string(),
            jsonContact.at(U(JSON_KEY_CONTACT_KEY)).as_string()
    );
}

// Build a GeoFence instance from the json element.
GeoFence* AccountBuilder::buildGeoFence(web::json::value &jsonGeoFence)
{
    return new GeoFence(
            jsonGeoFence.at(U(JSON_KEY_FENCE_SAFE)).as_bool(),
            jsonGeoFence.at(U(GEO_FENCE_KEY_LATITUDE)).as_double(),
            jsonGeoFence.at(U(GEO_FENCE_KEY_LONGITUDE)).as_double(),
            jsonGeoFence.at(U(GEO_FENCE_KEY_RADIUS)).as_double()
    );
}

// Build a GeoPath instance from the json element.
GeoPath* AccountBuilder::buildGeoPath(web::json::value &jsonGeoPath)
{
    return nullptr;
}

// Checks JSON structure for Contact representation.
bool AccountBuilder::isContactStructure(web::json::value &jsonContact)
{
    return !jsonContact.is_null() && jsonContact.is_object()
           && jsonContact.has_string_field(U(JSON_KEY_CONTACT_FORENAME))
           && jsonContact.has_string_field(U(JSON_KEY_CONTACT_SURNAME))
           && jsonContact.has_string_field(U(JSON_KEY_CONTACT_NUMBER))
           && jsonContact.has_string_field(U(JSON_KEY_CONTACT_KEY));
}

// Check json element structure to determine if value is GeoFence or not.
bool AccountBuilder::isGeoFenceStructure(web::json::value &jsonGeoFence)
{
    return !jsonGeoFence.is_null() && jsonGeoFence.is_object()
           && jsonGeoFence.has_boolean_field(U(JSON_KEY_FENCE_SAFE))
           && jsonGeoFence.has_double_field(U(GEO_FENCE_KEY_LATITUDE))
           && jsonGeoFence.has_double_field(U(GEO_FENCE_KEY_LONGITUDE))
           && jsonGeoFence.has_double_field(U(GEO_FENCE_KEY_RADIUS));
}

// Check json element structure to determine if value is GeoPath or not.
bool AccountBuilder::isGeoPathStructure(web::json::value &jsonGeoPath)
{
    return !jsonGeoPath.is_null() && jsonGeoPath.is_object()
           && jsonGeoPath.has_boolean_field(U(JSON_KEY_FENCE_SAFE))
           && jsonGeoPath.has_array_field(U(GEO_PATH_KEY_PATHS));
}




