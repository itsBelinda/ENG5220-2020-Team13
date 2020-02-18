#ifndef BEESAFEPI_POLYFENCE_H
#define BEESAFEPI_POLYFENCE_H

#include "Fence.h"

// System inclusions.
#include <vector>
#include <utility>

// JSON keys for major poly geo fence attributes.
#define JSON_KEY_POLY_FENCE_LATITUDE "Latitude"
#define JSON_KEY_POLY_FENCE_LONGITUDE "Longitude"

// Define the poly geo fence.
class PolyFence : public Fence
{

    // Constructors.
public:
    PolyFence(bool safe, std::vector<std::pair<double, double>>& coordinates);

    // Getters and setters.
public:
    std::vector<std::pair<double, double>>& getCoordinates();

    // Overridden interface.
public:
    bool isValid() override;
    bool isInside(double latitude, double longitude) override;

    // Attributes.
private:
    std::vector<std::pair<double, double>> coordinates;

};

#endif //BEESAFEPI_POLYFENCE_H
