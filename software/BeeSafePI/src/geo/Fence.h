/**
 * \file Fence.h
 * \class Fence
 *
 * \defgroup Geo Geo
 * \brief The package handling the fence declaration and management.
 * \ingroup Geo
 *
 * \brief The Fence parent class providing generic functionality for handling fences.
 *
 * The Fence class is the parent class to the Poly- and RoundFence classes. It provides methods to create and manage
 * the details of fences regardless of type, such as what times on what days is it considered a safezone, checks if the
 * device is within the time and physical boundaries of the location they are at, and provides a super function to
 * converting the Fence objects into JSON objects to be handled by the online database.
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

#ifndef BEESAFEPI_FENCE_H
#define BEESAFEPI_FENCE_H

// System inclusions.
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <cpprest/json.h>
#include <sstream>

// JSON keys used to initialise the fence.
#define JSON_KEY_FENCE_NAME "Name"
#define JSON_KEY_FENCE_DEFINITION "Definition"
#define JSON_KEY_FENCE_SAFE "Safe"
#define JSON_KEY_FENCE_WEEK "Week"
#define JSON_KEY_FENCE_DAYS {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}
#define JSON_KEY_FENCE_TIME_FROM "From"
#define JSON_KEY_FENCE_TIME_TO "To"

// A virtual fence definition.
class Fence {

public:

    // Constructors.
    Fence(std::string &name, bool safe,
          const std::map<int, std::vector<std::pair<std::tm, std::tm>>> &week);
    Fence(std::string &name, bool safe);
    Fence(const Fence &fence);

    // Destructors.
    virtual ~Fence() = 0;

public:

    // Getters and setters.
    std::string &getName();
    const std::map<int, std::vector<std::pair<std::tm, std::tm>>> &getWeek();
    const std::vector<std::pair<std::tm, std::tm>> &getTimes(int day);

    bool isSafe();
    bool isInTime();
    bool isInTime(const std::time_t &time);

    // Inherited interfaces.
    virtual bool isInLocation(std::pair<double, double> &latLng) = 0;
    virtual web::json::value serialiseFence();

    // isInside attributes to both location and time to be valid for the fence.
    bool isInside(std::pair<double, double> &latLng);

private:

    // Fence attributes
    std::string name;
    bool safe;
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> week;

};

#endif //BEESAFEPI_FENCE_H
