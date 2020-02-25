#include "Fence.h"

// System inclusions.
#include <sstream>

// Defines the constructor for the fence.
Fence::Fence(bool safe)
{
    this->safe = safe;
}

// Explicitly define the fence.
Fence::Fence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week)
{
    this->safe = safe;
    this->week = week;
}

// Fence destructor.
Fence::~Fence() = default;

// If the fence is regarded as being safe.
bool Fence::isSafe()
{
    return safe;
}

// Check if the device is within the fence at the current time.
bool Fence::isPresent()
{
    const std::time_t systemTime = std::time(nullptr);
    return isPresent(systemTime);
}

// Check if the time is present in the virtual fence.
bool Fence::isPresent(const std::time_t& time)
{

    // Extract information from system time.
    std::tm time_tm = *std::localtime(&time);
    auto iter = week.find(time_tm.tm_wday);

    // Check if time information exists.
    if (iter == week.end()) {
        return true;
    } else if (iter->second.empty()) {
        return true;
    }

    // Iterate through days list of from and to times.
    auto& dayTimes = iter->second;
    for (const std::pair<std::tm, std::tm>& dayTime : dayTimes) {

        // If time is before from time, we are not present.
        if (time_tm.tm_hour < dayTime.first.tm_hour) {
            return false;
        } else if (time_tm.tm_hour == dayTime.first.tm_hour) {
            if (time_tm.tm_min < dayTime.first.tm_min) {
                return false;
            }
        }

        // If the time is after the to time, we are not present.
        if (time_tm.tm_hour > dayTime.second.tm_hour) {
            return false;
        } else if (time_tm.tm_hour == dayTime.second.tm_hour) {
            if (time_tm.tm_min > dayTime.second.tm_min) {
                return false;
            }
        }
    }

    // By default the user is within the fence.
    return true;
}

// Get a map of all the days and their times.
const std::map<int, std::vector<std::pair<std::tm, std::tm>>>& Fence::getWeek()
{
    return week;
}

// Get all the times associated with a given day.
const std::vector<std::pair<std::tm, std::tm>>& Fence::getTimes(const int day)
{
    return week[day];
}