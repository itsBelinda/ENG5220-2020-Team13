#include "Fence.h"

// System inclusions.
#include <sstream>

// The format according to which fence times are broken down.
#define FENCE_TIME_FORMAT "%H:%M"

// Defines the constructor for the fence.
Fence::Fence(bool safe)
{
    this->safe = safe;
}

// Explicitly define the fence.
Fence::Fence(bool safe, std::map<int, std::vector<std::pair<std::tm, std::tm>>>& days)
{
    this->safe = safe;
    this->days = days;
}

// If the fence is regarded as being safe.
bool Fence::isSafe()
{
    return safe;
}

// Whether the user is present within the fence at the required time.
bool Fence::isPresent(std::time_t &time)
{

    return true;
}

// Get a map of all the days and their times.
std::map<int, std::vector<std::pair<std::tm, std::tm>>>& Fence::getDays()
{
    return days;
}

// Get all the times associated with a given day.
std::vector<std::pair<std::tm, std::tm>>& Fence::getTimes(int day)
{
    return days[day];
}


/*

// Determine if device within fence during defined times.
bool Fence::isPresent(std::time_t &time)
{

    // Check if fence has times; true if no times for day.
    std::tm time_out = *std::localtime(&time);

    auto iter = days.find(time_out.tm_wday);
    if (iter == days.end()) {
        return true;
    } else if (iter->second.empty()) {
        return true;
    }


    // Iterate through all times for the day.
    //auto fenceTimes = iter->second;
    std::tm fromTime;
    std::tm toTime;
    for (std::pair<std::string, std::string>& fenceTime : fenceTimes) {

        // Convert the time to a string.
        strptime(fenceTime.first.c_str(), FENCE_TIME_FORMAT, &fromTime);
        strptime(fenceTime.second.c_str(), FENCE_TIME_FORMAT, &toTime);


        // TODO: String conversion is poor! Store instances of struct tm instead.

    }


    return true;
}
*/