#ifndef BEESAFEPI_GEOFENCE_H
#define BEESAFEPI_GEOFENCE_H

#include "Fence.h"

// Defines the JSON keys for the GeoFence.
#define GEO_FENCE_KEY_LATITUDE "Forename"
#define GEO_FENCE_KEY_LONGITUDE "Surname"
#define GEO_FENCE_KEY_RADIUS "Number"

// A basic geo fence utilising coordinates and radius.
class GeoFence: public Fence
{

public:
    GeoFence(bool safe, double latitude, double longitude, double radius);

public:
    bool isValid() override;
    bool isInside(double latitude, double longitude) override;

public:
    double getLatitude();
    double getLongitude();
    double getRadius();

private:
    double latitude;
    double longitude;
    double radius;

};

#endif //BEESAFEPI_GEOFENCE_H
