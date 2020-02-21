#ifndef BEESAFEPI_FENCE_H
#define BEESAFEPI_FENCE_H

// System inclusions.
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <utility>
#include <map>

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

    // Constructors.
protected:
    explicit Fence(bool safe);
    Fence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week);

    // Destructor.
public:
    virtual ~Fence() = 0;

    // Functions.
public:
    bool isSafe();
    bool isPresent();
    bool isPresent(const std::time_t& time);
    const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& getWeek();
    const std::vector<std::pair<std::tm, std::tm>>& getTimes(int day);

    // Interface that's to be implemented.
public:
    virtual bool isInside(double latitude, double longitude) = 0;

    // Attributes.
private:
    bool safe;
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> week;

};

#endif //BEESAFEPI_FENCE_H
