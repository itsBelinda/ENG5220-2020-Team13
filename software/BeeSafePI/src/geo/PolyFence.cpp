#include "PolyFence.h"

// System inclusions.
#include <cmath>

// Basic poly fence constructor.
PolyFence::PolyFence(bool safe, std::vector<std::pair<double, double>> &coordinates)
        : Fence(safe)
{
    this->coordinates = coordinates;
}

// Get a vector of latitude and longitude pairs.
std::vector<std::pair<double, double>>& PolyFence::getCoordinates()
{
    return coordinates;
}

// Whether or not the definition is correct.
bool PolyFence::isValid()
{
    return true;
}

// Calculates whether or not latitude and longitude inside the poly fence.
bool PolyFence::isInside(double latitude, double longitude)
{

    // TODO: Check whether or not the coordinates are within the polygon!

    return true;
}


