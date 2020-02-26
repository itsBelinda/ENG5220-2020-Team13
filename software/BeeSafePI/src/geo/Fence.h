#ifndef BEESAFEPI_FENCE_H
#define BEESAFEPI_FENCE_H

// System inclusions.
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <cpprest/json.h>
#include <sstream>

// JSON keys used to initialise the fence.
#define JSON_KEY_FENCE_SAFE "Safe"
#define JSON_KEY_FENCE_WEEK "Week"
#define JSON_KEY_FENCE_DAYS {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}
#define JSON_KEY_FENCE_TIME_FROM "From"
#define JSON_KEY_FENCE_TIME_TO "To"
#define JSON_KEY_FENCE_FENCE "Fence"

// A virtual fence definition.
class Fence
{

public:

    // Constructors.
    explicit Fence(bool safe);
    Fence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week);

    // Destructors.
    virtual ~Fence() = 0;

public:

    // Getters and setters.
    const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& getWeek();
    const std::vector<std::pair<std::tm, std::tm>>& getTimes(int day);

    bool isSafe();
    bool isPresent();
    bool isPresent(const std::time_t& time);

    // Inherited interfaces.
    virtual bool isInside(double latitude, double longitude) = 0;
    virtual web::json::value serialiseFence();

private:

    // Attributes.
    bool safe;
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> week;

};

#endif //BEESAFEPI_FENCE_H
