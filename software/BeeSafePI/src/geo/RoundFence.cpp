#include "RoundFence.h"

#include <cmath>

// Explicit round fence constructor.
RoundFence::RoundFence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>> &week, double latitude,
                       double longitude, double radius)
        : Fence(safe, week)
{
    this->latitude = latitude;
    this->longitude = longitude;
    this->radius = radius;
}

// Basic RoundFence constructor.
RoundFence::RoundFence(bool safe, double latitude, double longitude, double radius)
        : Fence(safe)
{
    this->latitude = latitude;
    this->longitude = longitude;
    this->radius = radius;
}

// Get the latitude of the fence.
double RoundFence::getLatitude()
{
    return latitude;
}

// Get the longitude of the fence.
double RoundFence::getLongitude()
{
    return longitude;
}

// Get the radius of the fence.
double RoundFence::getRadius()
{
    return radius;
}

// Calculates whether or not latitude and longitude inside fence.
bool RoundFence::isInside(const double latitude, const double longitude)
{
    double distance = std::sqrt(
            (latitude - this->longitude) * (latitude - this->latitude) +
            (longitude - this->longitude) * (longitude - this->longitude)
    );
    return distance <= radius;
}