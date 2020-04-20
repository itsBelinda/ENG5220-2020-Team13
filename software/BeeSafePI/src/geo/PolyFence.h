/**
 * \file PolyFence.h
 * \class PolyFence
 *
 * \ingroup Geo
 *
 * \brief The PolyFence class contains the type specific methods and parameters relating to fences that are polygon shaped.
 *
 * The PolyFence class contains the type specific methods and parameters relating to fences that are polygon shaped.
 * The functionality related to polygon fences describes the calculations of whether a location falls within a fence or not, setting
 * the boundaries/dimensions of the polygon, the times and days of a fence counting as a safe zone, and the convesion of PolyFence
 * objects into JSON format.
 *
 * \author BeeSafe Team, Team 13
 *
 * \version v1.0
 *
 * \date 2020/04/20
 *
 * Contact: beesafe.uofg@gmail.com
 *
 * Licence: MIT
 */


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
    PolyFence(std::string &name, bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week, const std::vector<std::pair<double, double>>& coordinates);
    PolyFence(std::string &name, bool safe, const std::vector<std::pair<double, double>>& coordinates);
    PolyFence(const PolyFence &polyFence);

public:

    // Getters and setters.
    const std::vector<std::pair<double, double>>& getCoordinates();
    const std::vector<double>& getConstants();
    const std::vector<double>& getMultiples();

    // Generic object behaviour.
    void calculateFenceConstants();

    // Inherited interfaces.
    bool isInLocation(std::pair<double, double> &latLng) override;
    web::json::value serialiseFence() override;

private:

    // Attributes.
    std::vector<std::pair<double, double>> coordinates;
    std::vector<double> constants;
    std::vector<double> multiples;

};

#endif //BEESAFEPI_POLYFENCE_H
