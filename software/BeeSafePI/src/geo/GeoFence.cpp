#include "GeoFence.h"

#include <cmath>

// Constructor initialises geo fence and super class.
GeoFence::GeoFence(const bool safe, const double latitude,
                   const double longitude, const double radius)
        : Fence(safe)
{
    this->latitude = latitude;
    this->longitude = longitude;
    this->radius = radius;
}

// Get the latitude of the geo fence.
double GeoFence::getLatitude()
{
    return latitude;
}

// Get the longitude of the geo fence.
double GeoFence::getLongitude()
{
    return longitude;
}

// Get the radius of the geo fence.
double GeoFence::getRadius()
{
    return radius;
}

// Whether or not the geo fence is correctly defined.
bool GeoFence::isValid()
{
    return true;
}

// Geometrically calculate whether or not latitude and longitude in fence.
bool GeoFence::isInside(const double latitude, const double longitude)
{
    double distance = std::sqrt(
            (latitude - this->longitude) * (latitude - this->latitude) +
            (longitude - this->longitude) * (longitude - this->longitude)
    );
    return distance <= radius;
}