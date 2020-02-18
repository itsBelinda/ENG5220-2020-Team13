#ifndef BEESAFEPI_GEOPATH_H
#define BEESAFEPI_GEOPATH_H

#include "Fence.h"

#include <vector>
#include <utility>

// Used by JSON to identify and set key value pairs.
#define GEO_PATH_KEY_PATHS "Paths"

// Complex geo path defined using multiple latitude and longitude coordinates.
class GeoPath: public Fence
{

public:
    GeoPath(bool safe, std::vector<std::pair<double, double>>& coordinates);

public:
    bool isValid() override;
    bool isInside(double latitude, double longitude) override;

public:
    std::vector<std::pair<double, double>>& getCoordinates();

private:
    std::vector<std::pair<double, double>> coordinates;

};

#endif //BEESAFEPI_GEOPATH_H
