/**
 * \file AccountBuilder.h
 * \class AccountBuilder
 *
 * \ingroup Device
 *
 * \brief The AccountBuilder class creates and maintains Fence, Account and Contact objects
 *
 * The AccountBuilder class is a utility, builder class creating Account, different types of Fence and Contact C++ objects from
 * JSON file data from outside the immediate program. It parses the JSON objects and populates the appropriate C++ objects in the
 * relevant format. It also verifies the correctness of the information relating to each class.
 *
 * \author BeeSafe Team, Team 13
 *
 * \version v1.0
 *
 * \date 2020/04/20
 *
 * Contact: beesafe.uofg@gmail.com
 *
 * Licence: MIT
 */

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
    // Constructor
    AccountBuilder(utility::stringstream_t &stream, std::error_code &errorCode);

private:

    // For checking the json structure.
    bool hasAccountAttributes(const web::json::value& jsonAccountElement);
    bool hasContactAttributes(const web::json::value& jsonElement);
    bool hasFenceAttributes(const web::json::value& jsonFenceElement);
    bool hasRoundFenceAttributes(const web::json::value& jsonElement);
    bool hasPolyFenceAttributes(const web::json::value& jsonElement);

    // Clear a vector.
    template<typename T> void clearVector(std::vector<T>& vector);

    // Building general vectors.
    bool buildContacts(const web::json::array &jsonContacts, std::vector<Contact *> &contacts);
    bool buildFences(const web::json::array &jsonFences, std::vector<Fence *> &fences);

    // Building specific instances.
    Contact* buildContact(const web::json::value& jsonContact);
    Fence* buildFence(const web::json::value& jsonFence);
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> buildWeekMap(const web::json::value &week);
    RoundFence* buildRoundFence(std::string &name, bool safe, std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week, const web::json::value& jsonRoundFence);
    PolyFence* buildPolyFence(std::string &name, bool safe, std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week, const web::json::value& jsonPolyFence);

    // Used for building the Account instance from the string stream.
public:
    Account* build();

private:
    web::json::value root;

};

#endif //BEESAFEPI_ACCOUNTBUILDER_H
