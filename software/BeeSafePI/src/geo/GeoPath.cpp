#include "GeoPath.h"

#include <cmath>

// Constructor for the geo path.
GeoPath::GeoPath(const bool safe, std::vector<std::pair<double, double>>& coordinates)
        : Fence(safe)
{
    this->coordinates = coordinates;
}

// Get the vector of coordinates that defines the path.
std::vector<std::pair<double, double>>& GeoPath::getCoordinates()
{
    return coordinates;
};

// Whether or not the geo path is valid i.e. correctly defined.
bool GeoPath::isValid()
{
    return true;
}

// Geometrically determine whether or not latitude and longitude within the geo path.
bool GeoPath::isInside(const double latitude, const double longitude)
{

    return false;
}



