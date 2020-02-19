#ifndef BEESAFEPI_ACCOUNTBUILDER_H
#define BEESAFEPI_ACCOUNTBUILDER_H

// File inclusions.
#include "Account.h"
#include "../geo/Fence.h"
#include "../geo/RoundFence.h"
#include "../geo/PolyFence.h"

// Library inclusions.
#include <cpprest/json.h>

// System inclusions.
#include <iostream>
#include <vector>
#include <utility>
#include <map>

// Account builder is the means by which the JSON definition is converted to an Account object.
class AccountBuilder
{

public:
    AccountBuilder(utility::stringstream_t &stream, std::error_code &errorCode);

public:
    Account* build();

private:

    // For checking the json structure.
    bool hasAccountAttributes(web::json::value& element);
    bool hasContactAttributes(web::json::value& element);
    bool hasFenceAttributes(web::json::value& element);
    bool hasRoundFenceAttributes(web::json::value& element);
    bool hasPolyFenceAttributes(web::json::value& element);

    // Building general vectors.
    bool buildContactsVector(web::json::array& jsonContacts, std::vector<Contact> &contacts);
    bool buildFenceVector(web::json::array& jsonFences, std::vector<Fence> &fences);

    // Building specific instances.
    Contact buildContact(web::json::value& json);
    RoundFence buildRoundFence(web::json::value& element);
    PolyFence buildPolyFence(web::json::value& element);

private:
    web::json::value root;

};

#endif //BEESAFEPI_ACCOUNTBUILDER_H
