#include "RoundFence.h"

#include <cmath>

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