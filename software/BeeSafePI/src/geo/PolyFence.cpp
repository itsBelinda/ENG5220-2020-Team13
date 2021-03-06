/**
 * \file PolyFence.cpp
 * \class PolyFence
 *
 * \ingroup Geo
 *
 * \brief The PolyFence class contains the type specific methods and parameters relating to fences that are polygon shaped.
 *
 * The PolyFence class contains the type specific methods and parameters relating to fences that are polygon shaped.
 * The functionality related to polygon fences describes the calculations of whether a location falls within a fence or not, setting
 * the boundaries/dimensions of the polygon, the times and days of a fence counting as a safe zone, and the convesion of PolyFence
 * objects into JSON format.
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


#include "PolyFence.h"

// System inclusions.
#include <cmath>

// Explicit poly fence constructor
PolyFence::PolyFence(std::string &name, bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>> &week,
                     const std::vector<std::pair<double, double>> &coordinates)
        : Fence(name, safe, week)
{
    this->coordinates = coordinates;
    calculateFenceConstants();
}

// Basic poly fence constructor.
PolyFence::PolyFence(std::string &name, bool safe, const std::vector<std::pair<double, double>>& coordinates)
        : Fence(name, safe)
{
    this->coordinates = coordinates;
    calculateFenceConstants();
}

// Copy constructor for the poly fence.
PolyFence::PolyFence(const PolyFence &polyFence)
        : Fence(polyFence)
{
    this->coordinates = polyFence.coordinates;
    calculateFenceConstants();
}

// Get a vector of latitude and longitude pairs.
const std::vector<std::pair<double, double>>& PolyFence::getCoordinates()
{
    return coordinates;
}

// Get the vector of precomputed constants.
const std::vector<double>& PolyFence::getConstants()
{
    return constants;
}

// Get the vector of precomputed multiples.
const std::vector<double>& PolyFence::getMultiples()
{
    return multiples;
}

// Calculate any poly constants; saves us having to compute these values each time.
void PolyFence::calculateFenceConstants()
{

    // Clear the constants for recalculation.
    constants.clear();
    multiples.clear();

    // If the coordinates are empty, nothing can be done.
    if (coordinates.empty()) {
        return;
    }

    // Calculate poly fence constants.
    unsigned long i, j = coordinates.size() - 1;
    for (i = 0; i < coordinates.size(); ++i) {
        if (coordinates[i].second == coordinates[j].second) {
            constants.push_back(coordinates[i].first);
            multiples.push_back(0);
        } else {
            constants.push_back(coordinates[i].first
                                - (coordinates[i].second * coordinates[j].first)
                                  / (coordinates[j].second - coordinates[i].second)
                                + (coordinates[i].second * coordinates[i].first)
                                  / (coordinates[j].second - coordinates[i].second));
            multiples.push_back((coordinates[j].first - coordinates[i].first)
                                / (coordinates[j].second - coordinates[i].second));
        }
        j = i;
    }
}

// Calculates whether or not latitude and longitude inside the poly fence.
bool PolyFence::isInLocation(std::pair<double, double> &latLng)
{

    bool oddNodes = false;
    bool current = coordinates.back().second > latLng.second;
    bool previous;

    // Determines whether or not latitude and longitude are within the fence.
    for (int i = 0; i < coordinates.size(); ++i) {
        previous = current;
        current = coordinates[i].second > latLng.second;
        if (current != previous) {
            oddNodes ^= latLng.second * multiples[i] + constants[i] < latLng.first;
        }
    }

    return oddNodes;
}

// Serialise the poly fence into a JSON element.
web::json::value PolyFence::serialiseFence()
{

    // Serialise the super class attributes.
    web::json::value jsonFence = Fence::serialiseFence();

    // Serialise PolyFence specific attributes.
    for (int i = 0; i < coordinates.size(); ++i) {
        jsonFence[U(JSON_KEY_FENCE_DEFINITION)][i][U(JSON_KEY_POLY_FENCE_LATITUDE)]
                = web::json::value::number(coordinates[i].first);
        jsonFence[U(JSON_KEY_FENCE_DEFINITION)][i][U(JSON_KEY_POLY_FENCE_LONGITUDE)]
                = web::json::value::number(coordinates[i].second);
    }

    // Finally, return the serialised fence.
    return jsonFence;
}
