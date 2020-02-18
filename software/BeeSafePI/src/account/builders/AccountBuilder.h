#ifndef BEESAFEPI_ACCOUNTBUILDER_H
#define BEESAFEPI_ACCOUNTBUILDER_H

#include "../Account.h"
#include "../../geo/Fence.h"
#include "../../geo/GeoFence.h"
#include "../../geo/GeoPath.h"

#include <cpprest/json.h>

#include <iostream>
#include <vector>

// Account builder is the means by which the JSON definition is converted to an Account object.
class AccountBuilder
{

public:
    AccountBuilder(utility::stringstream_t &stream, std::error_code &errorCode);

public:
    Account* build();

private:

    // Building general vectors.
    bool buildContactsVector(web::json::array &jsonContacts, std::vector<Contact*> &contacts);
    bool buildFenceVector(web::json::array &jsonFences, std::vector<Fence*> &fences);

    // Building specific instances.
    Contact* buildContact(web::json::value &jsonContact);
    GeoFence* buildGeoFence(web::json::value &jsonGeoFence);
    GeoPath* buildGeoPath(web::json::value &jsonGeoPath);

    // Check the structure of the Json element.
    bool isContactStructure(web::json::value &jsonContact);
    bool isGeoFenceStructure(web::json::value &jsonGeoFence);
    bool isGeoPathStructure(web::json::value &jsonGeoPath);

private:
    web::json::value root;

};

#endif //BEESAFEPI_ACCOUNTBUILDER_H
