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

public:

    // Constructors.
    PolyFence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week, const std::vector<std::pair<double, double>>& coordinates);
    PolyFence(bool safe, const std::vector<std::pair<double, double>>& coordinates);
    PolyFence(const PolyFence &polyFence);

public:

    // Getters and setters.
    const std::vector<std::pair<double, double>>& getCoordinates();
    const std::vector<double>& getConstants();
    const std::vector<double>& getMultiples();

    // Generic object behaviour.
    void calculateFenceConstants();

    // Inherited interfaces.
    bool isInside(double latitude, double longitude) override;
    web::json::value serialiseFence() override;

private:

    // Attributes.
    std::vector<std::pair<double, double>> coordinates;
    std::vector<double> constants;
    std::vector<double> multiples;

};

#endif //BEESAFEPI_POLYFENCE_H
