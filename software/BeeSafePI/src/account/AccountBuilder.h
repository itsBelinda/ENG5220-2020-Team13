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
#include <string>
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
    bool hasAccountAttributes(const web::json::value& jsonElement);
    bool hasContactAttributes(const web::json::value& jsonElement);
    bool hasFenceAttributes(const web::json::value& jsonElement);
    bool hasRoundFenceAttributes(const web::json::value& jsonElement);
    bool hasPolyFenceAttributes(const web::json::value& jsonElement);

    // Building general vectors.
    bool buildContactsVector(const web::json::array& jsonContacts, std::vector<Contact*> &contacts);
    bool buildFenceVector(const web::json::array& jsonFences, std::vector<Fence*> &fences);

    // Clear a vector.
    template<typename T> void clearVector(std::vector<T>& vector);

    // Building specific instances.
    Contact* buildContact(const web::json::value& jsonContact);
    Fence* buildFence(const web::json::value& jsonFence);
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> buildFenceWeekMap(const web::json::value& week);
    RoundFence* buildRoundFence(bool safe, std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week, const web::json::value& jsonRoundFence);
    PolyFence* buildPolyFence(bool safe, std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week, const web::json::value& jsonPolyFence);

private:
    web::json::value root;

};

#endif //BEESAFEPI_ACCOUNTBUILDER_H
