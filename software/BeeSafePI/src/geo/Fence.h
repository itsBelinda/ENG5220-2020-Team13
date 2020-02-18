#ifndef BEESAFEPI_FENCE_H
#define BEESAFEPI_FENCE_H

// System inclusions.
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <utility>
#include <map>

// Fence JSON keys.
#define JSON_KEY_FENCE_SAFE "Safe"
#define JSON_KEY_FENCE_DAYS "Days"
#define JSON_KEY_FENCE_TIME_FROM "From"
#define JSON_KEY_FENCE_TIME_TO "To"
#define JSON_KEY_FENCE_FENCE "Fence"

// A virtual fence definition.
class Fence
{

    // Constructors.
protected:
    explicit Fence(bool safe);
    Fence(bool safe, std::map<int, std::vector<std::pair<std::string, std::string>>>& days);

    // Getters and setters.
public:
    bool isSafe();
    bool isPresent(std::time_t& time);
    std::map<int, std::vector<std::pair<std::tm, std::tm>>>& getDays();
    std::vector<std::pair<std::tm, std::tm>>& getTimes(int day);

    // Interface that's to be implemented.
public:
    virtual bool isValid() = 0;
    virtual bool isInside(double latitude, double longitude) = 0;

    // Attributes.
private:
    bool safe;
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> days;

};

#endif //BEESAFEPI_FENCE_H
