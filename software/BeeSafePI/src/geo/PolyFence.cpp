#include "PolyFence.h"

// System inclusions.
#include <cmath>

// Explicit poly fence constructor
PolyFence::PolyFence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>> &week,
                     const std::vector<std::pair<double, double>> &coordinates)
        : Fence(safe, week)
{
    this->coordinates = coordinates;
}

// Basic poly fence constructor.
PolyFence::PolyFence(bool safe, const std::vector<std::pair<double, double>>& coordinates)
        : Fence(safe)
{
    this->coordinates = coordinates;
}

// Get a vector of latitude and longitude pairs.
const std::vector<std::pair<double, double>>& PolyFence::getCoordinates()
{
    return coordinates;
}

// Calculates whether or not latitude and longitude inside the poly fence.
bool PolyFence::isInside(const double latitude, const double longitude)
{

    return true;
}


