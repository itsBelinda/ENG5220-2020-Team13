#include "AccountBuilder.h"

// Used for parsing the time format.
#define JSON_VALUE_TIME_FORMAT "%H:%M"

// Initialise the device builder.
AccountBuilder::AccountBuilder(utility::stringstream_t &stream, std::error_code &errorCode)
{
    this->root = web::json::value::parse(stream, errorCode);
}

// Builds an instance of the Account class.
Account* AccountBuilder::build()
{
    // Check whether the element matches the device file.
    if (!hasAccountAttributes(root)) {
        return nullptr;
    }

    // Get the name for the account.
    std::string name = root.at(U(JSON_KEY_ACCOUNT_NAME)).as_string();

    // Create contacts for the device.
    const web::json::array& jsonContacts = root.at(U(JSON_KEY_ACCOUNT_CONTACTS)).as_array();
    std::vector<Contact*> contacts;

    // Parse the list of contacts, creating them in the process.
    if (!buildContacts(jsonContacts, contacts)) {
        clearVector(contacts);
        return nullptr;
    }

    // Create the fences for the device.
    const web::json::array& jsonFences = root.at(U(JSON_KEY_ACCOUNT_FENCES)).as_array();
    std::vector<Fence*> fences;

    // Parse the array of contacts creating them in the process.
    if (!buildFences(jsonFences, fences)) {
        clearVector(contacts);
        clearVector(fences);
        return nullptr;
    }

    // We have fuccessfully created an device instance.
    return new Account(name, contacts, fences);
}

// Check that json has device attributes.
bool AccountBuilder::hasAccountAttributes(const web::json::value &jsonAccountElement)
{
    return !jsonAccountElement.is_null() && jsonAccountElement.is_object()
           && jsonAccountElement.has_string_field(U(JSON_KEY_ACCOUNT_NAME))
           && jsonAccountElement.has_array_field(U(JSON_KEY_ACCOUNT_CONTACTS))
           && jsonAccountElement.has_array_field(U(JSON_KEY_ACCOUNT_FENCES));
}

// Check that JSON element has Contact attributes i.e. structure.
bool AccountBuilder::hasContactAttributes(const web::json::value &jsonElement)
{
    return !jsonElement.is_null() && jsonElement.is_object()
           && jsonElement.has_string_field(U(JSON_KEY_CONTACT_FORENAME))
           && jsonElement.has_string_field(U(JSON_KEY_CONTACT_SURNAME))
           && jsonElement.has_string_field(U(JSON_KEY_CONTACT_NUMBER))
           && jsonElement.has_string_field(U(JSON_KEY_CONTACT_KEY));
}

// Check that the element has general fence attributes.
bool AccountBuilder::hasFenceAttributes(const web::json::value &jsonFenceElement)
{
    return !jsonFenceElement.is_null() && jsonFenceElement.is_object()
           && jsonFenceElement.has_string_field(U(JSON_KEY_FENCE_NAME))
           && jsonFenceElement.has_boolean_field(U(JSON_KEY_FENCE_SAFE))
           && jsonFenceElement.has_object_field(U(JSON_KEY_FENCE_WEEK))
           && jsonFenceElement.has_field(U(JSON_KEY_FENCE_DEFINITION));
}

// Check that json element has round fence attributes.
bool AccountBuilder::hasRoundFenceAttributes(const web::json::value &jsonElement)
{
    bool rc = !jsonElement.is_null() && jsonElement.is_object()
           && jsonElement.has_number_field(U(JSON_KEY_ROUND_FENCE_LATITUDE))
           && jsonElement.has_number_field(U(JSON_KEY_ROUND_FENCE_LONGITUDE))
           && jsonElement.has_number_field(U(JSON_KEY_ROUND_FENCE_RADIUS));
    return rc;
}

// Check whether the element has poly fence structure.
bool AccountBuilder::hasPolyFenceAttributes(const web::json::value &jsonElement)
{
    return !jsonElement.is_null() && jsonElement.is_array();
}

// Generic function for clearing vectors.
template<typename T>
void AccountBuilder::clearVector(std::vector<T>& vector)
{
    for (auto iter = vector.begin(); iter != vector.end(); ++iter) {
        delete *iter;
    }
    vector.clear();
}

// Build a contact vector.
bool AccountBuilder::buildContacts(const web::json::array &jsonContacts, std::vector<Contact *> &contacts)
{
    Contact* contact = nullptr;
    for (auto& element : jsonContacts) {
        if (!hasContactAttributes(element)) {
            return false;
        }
        contact = buildContact(element);
        if (contact == nullptr) {
            return false;
        }
        contacts.push_back(contact);
    }
    return true;
}

// Build a fence vector.
bool AccountBuilder::buildFences(const web::json::array &jsonFences, std::vector<Fence *> &fences)
{
    Fence* fence = nullptr;
    for (auto& element : jsonFences) {
        if (!hasFenceAttributes(element)) {
            std::cout << element << std::endl;
            return false;
        }
        fence = buildFence(element);
        if (fence == nullptr) {
            return false;
        }
        fences.push_back(fence);
    }
    return true;
}

// Used to create an instance of a Contact based on JSON element.
Contact* AccountBuilder::buildContact(const web::json::value &json)
{
    return new Contact(
            json.at(U(JSON_KEY_CONTACT_FORENAME)).as_string(),
            json.at(U(JSON_KEY_CONTACT_SURNAME)).as_string(),
            json.at(U(JSON_KEY_CONTACT_NUMBER)).as_string(),
            json.at(U(JSON_KEY_CONTACT_KEY)).as_string()
    );
}

// Used to create an instance of a fence.
Fence* AccountBuilder::buildFence(const web::json::value &element)
{

    // Get and build general fence attributes.
    std::string name = element.at(U(JSON_KEY_FENCE_NAME)).as_string();
    bool safe = element.at(U(JSON_KEY_FENCE_SAFE)).as_bool();
    auto map = buildWeekMap(element.at(U(JSON_KEY_FENCE_WEEK)));
    const web::json::value& fence = element.at(U(JSON_KEY_FENCE_DEFINITION));

    // Combine general fence attributes with fence specific attributes.
    if (hasRoundFenceAttributes(fence)) {
        return buildRoundFence(name, safe, map, fence);
    } else if (hasPolyFenceAttributes(fence)) {
        return buildPolyFence(name, safe, map, fence);
    } else {
        return nullptr;
    }
}

// Creates a new instance of the timing map for a given fence.
std::map<int, std::vector<std::pair<std::tm, std::tm>>> AccountBuilder::buildWeekMap(const web::json::value &jsonWeek)
{
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> week;
    const std::string days[] = JSON_KEY_FENCE_DAYS;

    // Parse the string representation of time for each week, converting them into the tm (time) structure.
    std::vector<std::pair<std::tm, std::tm>> dayTimes;
    std::pair<std::tm, std::tm> dayTime;
    struct std::tm fromTm = {0};
    struct std::tm toTm = {0};
    for (int i = 0; i < days->length(); ++i) {

        // Check that the week object has a specific day [array].
        if (!jsonWeek.has_array_field(U(days[i]))) {
            continue;
        }

        // Get the array of times for that day.
        const web::json::array& times = jsonWeek.at(U(days[i])).as_array();
        if (times.size() == 0) {
            continue;
        }

        // Parse each of the times, convert the into structures and add.
        for (auto& time : times) {
            strptime(time.at(U(JSON_KEY_FENCE_TIME_FROM)).as_string().c_str(), JSON_VALUE_TIME_FORMAT, &fromTm);
            strptime(time.at(U(JSON_KEY_FENCE_TIME_TO)).as_string().c_str(), JSON_VALUE_TIME_FORMAT, &toTm);
            dayTime.first = fromTm;
            dayTime.second = toTm;
            dayTimes.push_back(dayTime);
        }

        // Finally add the vector of times to the map.
        week.insert(std::pair<int, std::vector<std::pair<std::tm, std::tm>>>(i, dayTimes));
        dayTimes.clear();
    }

    return week;
}

// Create a new instance of round fence.
RoundFence* AccountBuilder::buildRoundFence(std::string &name, bool safe, std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week,
                                            const web::json::value& jsonRoundFence)
{
    return new RoundFence(
            name,
            safe,
            week,
            jsonRoundFence.at(U(JSON_KEY_ROUND_FENCE_LATITUDE)).as_double(),
            jsonRoundFence.at(U(JSON_KEY_ROUND_FENCE_LONGITUDE)).as_double(),
            jsonRoundFence.at(U(JSON_KEY_ROUND_FENCE_RADIUS)).as_double()
    );
}

// Creates an instance of a poly fence.
PolyFence* AccountBuilder::buildPolyFence(std::string &name, bool safe, std::map<int, std::vector<std::pair<std::tm, std::tm>>>& week,
                                          const web::json::value& jsonFence)
{

    const web::json::array& jsonCoordinates = jsonFence.as_array();
    std::vector<std::pair<double, double>> coordinates;

    // Loop through each of the coordinates, creating a vector from them.
    std::pair<double, double> coordinate;
    for (auto& jsonCoordinate : jsonCoordinates) {
        coordinate.first = jsonCoordinate.at(U(JSON_KEY_POLY_FENCE_LATITUDE)).as_double();
        coordinate.second = jsonCoordinate.at(U(JSON_KEY_POLY_FENCE_LONGITUDE)).as_double();
        coordinates.push_back(coordinate);
    }

    // Create an instance of the poly fence.
    return new PolyFence(name, safe, week, coordinates);
}

