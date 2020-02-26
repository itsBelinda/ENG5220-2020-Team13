#include "PolyFence.h"

// System inclusions.
#include <cmath>

// Explicit poly fence constructor
PolyFence::PolyFence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>> &week,
                     const std::vector<std::pair<double, double>> &coordinates)
        : Fence(safe, week)
{
    this->coordinates = coordinates;
    calculateFenceConstants();
}

// Basic poly fence constructor.
PolyFence::PolyFence(bool safe, const std::vector<std::pair<double, double>>& coordinates)
        : Fence(safe)
{
    this->coordinates = coordinates;
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

// Calculate any poly constants.
void PolyFence::calculateFenceConstants()
{

    // Clear the constants for recalculation.
    constants.clear();
    multiples.clear();

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
bool PolyFence::isInside(const double latitude, const double longitude)
{

    bool oddNodes = false;
    bool current = coordinates.back().second > longitude;
    bool previous;

    // Determines whether or not latitude and longitude are within the fence.
    for (int i = 0; i < coordinates.size(); ++i) {
        previous = current;
        current = coordinates[i].second > longitude;
        if (current != previous) {
            oddNodes ^= longitude * multiples[i] + constants[i] < latitude;
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
        jsonFence[U(JSON_KEY_FENCE_FENCE)][i][U(JSON_KEY_POLY_FENCE_LATITUDE)]
                = web::json::value::number(coordinates[i].first);
        jsonFence[U(JSON_KEY_FENCE_FENCE)][i][U(JSON_KEY_POLY_FENCE_LATITUDE)]
                = web::json::value::number(coordinates[i].second);
    }

    // Finally, return the serialised fence.
    return jsonFence;
}
