#include "Fence.h"

// System inclusions.
#include <sstream>
#include "iostream"

// Define the format according to which the date time will be written.
#define DAY_TIME_STRING_FORMAT "%d:%d"
#define DAY_TIME_BUFFER_SIZE 6

// Defines the constructor for the fence.
Fence::Fence(bool safe) {
    this->safe = safe;
}

// Explicitly define the fence.
Fence::Fence(bool safe, const std::map<int, std::vector<std::pair<std::tm, std::tm>>> &week) {
    this->safe = safe;
    this->week = week;
}

// The copy constructor.
Fence::Fence(const Fence &fence) {
    this->safe = fence.safe;
    this->week = fence.week;
}

// Fence destructor.
Fence::~Fence() = default;

bool isInLocation(std::pair<double, double> &pair);

bool isInTime();

// Get a map of all the days and their times.
const std::map<int, std::vector<std::pair<std::tm, std::tm>>> &Fence::getWeek() {
    return week;
}

// Get all the times associated with a given day.
const std::vector<std::pair<std::tm, std::tm>> &Fence::getTimes(const int day) {
    return week[day];
}

// If the fence is regarded as being safe.
bool Fence::isSafe() {
    return safe;
}

// Check if the device is within the fence at the current time.
bool Fence::isInTime() {
    const std::time_t systemTime = std::time(nullptr);
    return isInTime(systemTime);
}

// Check if the time is present in the virtual fence.
//TODO: make sure this returns true if no time is set
bool Fence::isInTime(const std::time_t &time) {

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
    auto &dayTimes = iter->second;
    for (const std::pair<std::tm, std::tm> &dayTime : dayTimes) {

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


bool Fence::isInside(std::pair<double, double> &latLng) {
    return( this->isInTime() && this->isInLocation(latLng));
}

// Serialise the fence instance into a JSON element.
web::json::value Fence::serialiseFence() {

    std::cout << "HERE! IN! ACCOUNT!" << std::endl;

    // The list of day names and the fence root element, respectively.
    const std::string days[] = JSON_KEY_FENCE_DAYS;
    char dayTimeBuffer[DAY_TIME_BUFFER_SIZE];

    // The root fence json element.
    web::json::value jsonFence = web::json::value::object();

    // Serialise generic fence attributes.
    jsonFence[U(JSON_KEY_FENCE_SAFE)] = web::json::value::boolean(safe);
    for (auto &day : week) {
        for (int i = 0; i < day.second.size(); ++i) {

            std::cout << "Day: " << days[day.first] << ", Time: " << day.second[i].second.tm_hour << ":" << day.second[i].second.tm_min << std::endl;

            // Format the string that's to be written.
            snprintf(dayTimeBuffer,
                     DAY_TIME_BUFFER_SIZE,
                     DAY_TIME_STRING_FORMAT,
                     day.second[i].first.tm_hour,
                     day.second[i].first.tm_min
            );

            std::cout << "Buffer: " << dayTimeBuffer << std::endl;

            // serialise the from time.
            jsonFence[U(JSON_KEY_FENCE_WEEK)][days[day.first]][i][U(JSON_KEY_FENCE_TIME_FROM)]
                    = web::json::value::string(U(dayTimeBuffer));

            // Format the string that's to be written.
            snprintf(dayTimeBuffer,
                     DAY_TIME_BUFFER_SIZE,
                     DAY_TIME_STRING_FORMAT,
                     day.second[i].second.tm_hour,
                     day.second[i].second.tm_min
            );

            // Serialise the to time.
            jsonFence[U(JSON_KEY_FENCE_WEEK)][days[day.first]][i][U(JSON_KEY_FENCE_TIME_TO)]
                    = web::json::value::string(U(dayTimeBuffer));;
        }
    }

    // Pass to sub-class for additional serialisation.
    return jsonFence;
}