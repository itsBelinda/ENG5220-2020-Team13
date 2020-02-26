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

public:

    // Constructors.
    RoundFence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week, double latitude, double longitude, double radius);
    RoundFence(bool safe, double latitude, double longitude, double radius);

public:

    // Getters and setters.
    double getLatitude();
    double getLongitude();
    double getRadius();

    // Inherited interfaces.
    bool isInside(double latitude, double longitude) override;
    web::json::value serialiseFence() override;

private:

    // Attributes.
    double latitude;
    double longitude;
    double radius;

};

#endif //BEESAFEPI_ROUNDFENCE_H
