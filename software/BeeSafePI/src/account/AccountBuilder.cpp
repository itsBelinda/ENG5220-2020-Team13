#include "AccountBuilder.h"

#include <string>

// Initialise the account builder.
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

// Check that json has account attributes.
bool AccountBuilder::hasAccountAttributes(web::json::value &element)
{
    return !element.is_null() && element.is_object()
           && element.has_array_field(U(JSON_KEY_ACCOUNT_CONTACTS))
           && element.has_array_field(U(JSON_KEY_ACCOUNT_FENCES));
}

// Check that JSON element has Contact attributes i.e. structure.
bool AccountBuilder::hasContactAttributes(web::json::value &element)
{
    return !element.is_null() && element.is_object()
           && element.has_string_field(U(JSON_KEY_CONTACT_FORENAME))
           && element.has_string_field(U(JSON_KEY_CONTACT_SURNAME))
           && element.has_string_field(U(JSON_KEY_CONTACT_NUMBER))
           && element.has_string_field(U(JSON_KEY_CONTACT_KEY));
}

// Check that the element has general fence attributes.
bool AccountBuilder::hasFenceAttributes(web::json::value &element)
{
    return !element.is_null() && element.is_object()
           && element.has_boolean_field(U(JSON_KEY_FENCE_SAFE))
           && element.has_array_field(U(JSON_KEY_FENCE_WEEK))
           && element.has_object_field(U(JSON_KEY_FENCE_FENCE));
}

// Check that json element has round fence attributes.
bool AccountBuilder::hasRoundFenceAttributes(web::json::value &element)
{
    return !element.is_null() && element.is_object()
           && element.has_double_field(U(JSON_KEY_ROUND_FENCE_LATITUDE))
           && element.has_double_field(U(JSON_KEY_ROUND_FENCE_LONGITUDE))
           && element.has_double_field(U(JSON_KEY_ROUND_FENCE_RADIUS));
}

// Check whether the element has poly fence structure.
bool AccountBuilder::hasPolyFenceAttributes(web::json::value &element)
{
    return !element.is_null() && element.is_object()
           && element.has_double_field(U(JSON_KEY_POLY_FENCE_LATITUDE))
           && element.has_double_field(U(JSON_KEY_POLY_FENCE_LONGITUDE));
}

// Build a contact vector.
bool AccountBuilder::buildContactsVector(web::json::array &jsonContacts, std::vector<Contact> &contacts)
{
    for (auto& element : jsonContacts) {
        if (!hasContactAttributes(element)) {
            return false;
        }
        contacts.push_back(buildContact(element));
    }
    return true;
}

// Build a fence vector.
bool AccountBuilder::buildFenceVector(web::json::array &jsonFences, std::vector<Fence> &fences)
{
    for (auto& element : jsonFences) {

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


