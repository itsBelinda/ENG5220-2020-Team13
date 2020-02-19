#ifndef BEESAFEPI_ROUNDFENCE_H
#define BEESAFEPI_ROUNDFENCE_H

#include "Fence.h"

// JSON keys for major round geo fence attributes.
#define JSON_KEY_ROUND_FENCE_LATITUDE "Latitude"
#define JSON_KEY_ROUND_FENCE_LONGITUDE "Longitude"
#define JSON_KEY_ROUND_FENCE_RADIUS "Radius"

// Define the round geo fence.
class RoundFence : public Fence
{

    // Constructors.
public:
    RoundFence(bool safe, double latitude, double longitude, double radius);

    // Getters and setters.
public:
    double getLatitude();
    double getLongitude();
    double getRadius();

    // Overridden interface.
public:
    bool isInside(double latitude, double longitude) override;

    // Attributes.
private:
    double latitude;
    double longitude;
    double radius;

};

#endif //BEESAFEPI_ROUNDFENCE_H
