/**
 * \file RoundFence.h
 * \class RoundFence
 *
 * \ingroup Geo
 *
 * \ingroup Geo
 *
 * \brief The RoundFence class contains the type specific methods and parameters relating to fences that are circular shaped.
 *
 * The RoundFence class contains the type specific methods and parameters relating to fences that are circular shaped.
 * The functionality related to round fences describes the calculations of whether a location falls within a fence or not, setting
 * the boundary of the circle, the times and days of a fence counting as a safe zone, and the convesion of RoundFence
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
    RoundFence(std::string &name, bool safe,
               const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week,
               double latitude, double longitude, double radius);
    RoundFence(std::string &name, bool safe, double latitude, double longitude, double radius);
    RoundFence(const RoundFence &roundFence);

public:

    // Getters and setters.
    double getLatitude();
    double getLongitude();
    double getRadius();

    // Inherited interfaces.
    bool isInLocation(std::pair<double, double> &latLng) override;
    web::json::value serialiseFence() override;

private:

    // Round Fence attributes.
    double latitude;
    double longitude;
    double radius;

};

#endif //BEESAFEPI_ROUNDFENCE_H
