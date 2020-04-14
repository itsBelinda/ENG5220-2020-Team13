#include "BeeSafe.h"

#include "device/AccountBuilder.h"

#include <utility>
#include <iostream>

#define INIT_COMMS_TRIES 3

#define ACCOUNT_PATH "/home/dans/dev/software/BeeSafePI/Account.json"

/**
 * Constructor used to initialise an instance of the BeeSafeManager class.
 */
BeeSafeManager::BeeSafeManager()
{
    comms = nullptr;
    account = nullptr;
    monitor = nullptr;
}

/**
 * Destructor is responsible for releasing any resources occupied by the BeeSafeManager
 * instance. Note, if the monitor thread is running, it will implicitly be stopped.
 */
BeeSafeManager::~BeeSafeManager()
{
    // Delete monitor first to illegal memory access.
    if (monitor != nullptr) {
        monitor->stop();
        delete monitor;
    }

    // Finally, delete the comms and account.
    delete comms;
    delete account;
}

/**
 * Get the managers communication interface.
 *
 * @return A pointer to the Comms interface instance (if initialised), a nullptr otherwise.
 */
Comms* const BeeSafeManager::getComms()
{
    return comms;
}

/**
 * Get the monitor thread responsible for checking the users location.
 *
 * @return A pointer to the Monitor thread (if initialised), nullptr otherwise.
 */
Monitor* const BeeSafeManager::getMonitor()
{
    return monitor;
}

/**
 * Get the device Account being monitored.
 *
 * @return A pointer to an Account instance (if initialised), nullptr otherwise.
 */
Account* const BeeSafeManager::getAccount()
{
    return account;
}

/**
 * Attempt to initialise the BeeSafeManager instance responsible for
 * managing the monitor thread, comms and accounts. Note, the function will
 * only set the member pointers in the event all necessary sub-systems are
 * initialised.
 *
 * Furthermore, because an account may not be already present on the device,
 * the resulting member account pointer _may be a nullptr_.
 *
 * @return True if all necessary components have been initialised, false
 *      otherwise.
 */
bool BeeSafeManager::init()
{
    // Prevent memory leaks by releasing occupied resources.
    if (this->monitor != nullptr) {
        this->monitor->stop();
        delete this->monitor;
    }
    delete this->comms;
    delete this->account;

    // First, attempt to init comms.
    auto comms = initComms();
    if (comms == nullptr) {
        return false;
    }

    // Attempt to initialise the monitor thread.
    auto monitor = initMonitor(comms);
    if (monitor == nullptr) {
        delete comms;
        return false;
    }

    // The account can be null.
    auto account = initAccount(ACCOUNT_PATH);
    if (account == nullptr) {
        std::cout << "Failed to load account." << std::endl;
    } else {
        std::cout << "Successfully loaded account." << std::endl;
    }

    // We have successfully initialised the manager.
    this->comms = comms;
    this->monitor = monitor;
    this->account = account;

    return true;
}

/**
 * Function is responsible for the creation / initialisation of the Comms
 * interface instance.
 *
 * @return A pointer to an instance of Comms (if successfully initialised / created),
 *      nullptr otherwise.
 */
Comms* BeeSafeManager::initComms()
{
    auto comms = new Comms();

    // Try numerous times to initialise the comms interface.
    bool init = false;
    short tries = 0;
    do {

        // Attempt to initialise the interface.
        tries++;
        init = comms->init();
        if (init) {
            break;
        }

    } while (tries < INIT_COMMS_TRIES);

    // We failed to initialise the interface.
    if (!init) {
        delete comms;
        return nullptr;
    }

    // Successfully return the comms instance.
    return comms;
}

/**
 * Function is responsible for the creation / initialisation of the Monitor
 * thread instance.
 *
 * @param comms The communication interface that's to be utilised.
 * @return A pointer to an instance of Monitor (if successfully initialised / created),
 *      nullptr otherwise.
 */
Monitor* BeeSafeManager::initMonitor(Comms* const comms)
{
    if (comms != nullptr) {
        return new Monitor(comms);
    }
    return nullptr;
}

/**
 * Function is responsible for the initialisation / creation (reading) of the
 * local Account instance.
 *
 * @param path The file path of the JSON file i.e. the path to where the Account.json
 *      file resides.
 * @return A pointer to an instance of Account if it was successfully read, parsed
 *      and created, nullptr otherwise.
 */
Account* BeeSafeManager::initAccount(const char* const path)
{
    try {
        // Attempt to read the file.
        utility::ifstream_t ifStream;
        ifStream.open(path);
        if (ifStream.fail()) {
            std::cerr << "Failed." << std::endl;
            return nullptr;
        } else if (!ifStream.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return nullptr;
        }

        // Stream the file.
        utility::stringstream_t sStream;
        sStream << ifStream.rdbuf();

        // Attempt to build the JSON file.
        std::error_code ec;
        AccountBuilder accountBuilder(sStream, ec);
        auto account = accountBuilder.build();

        std::cout << ec.value() << ", " << ec.message() << std::endl;

        // Close the file stream.
        ifStream.close();

        // Return an account instance, may be null.
        return account;

    } catch (web::json::json_exception &e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}


bool BeeSafeManager::start()
{

    // If the account is not null, start the monitor thread.
    if (account != nullptr) {
        if (!monitor->start(account)) {
            std::cerr << "Failed to start the monitor thread." << std::endl;
            return false;
        }
    }

    // TODO: Implement the manager thread for obtaining the account data file.

    return true;
}

void createAccount()
{
    // Create temp times.
    std::tm fromTime1 = {0};
    fromTime1.tm_hour = 9;
    fromTime1.tm_min = 30;

    std::tm toTime1 = {0};
    toTime1.tm_hour = 20;
    toTime1.tm_min = 10;

    std::tm fromTime2 = {0};
    fromTime2.tm_hour = 9;
    fromTime2.tm_min = 30;

    std::tm toTime2 = {0};
    toTime2.tm_hour = 14;
    toTime2.tm_min = 20;

    std::tm fromTime3 = {0};
    fromTime3.tm_hour = 9;
    fromTime3.tm_min = 30;

    std::tm toTime3 = {0};
    toTime3.tm_hour = 12;
    toTime3.tm_min = 10;

    // Create a temp day.
    std::vector<std::pair<std::tm, std::tm>> day;
    day.emplace_back(std::make_pair(fromTime1, toTime1));
    day.emplace_back(std::make_pair(fromTime2, toTime2));
    day.emplace_back(std::make_pair(fromTime3, toTime3));

    // Create temp week.
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> week;
    week.insert(std::make_pair(0, day));
    week.insert(std::make_pair(2, day));
    week.insert(std::make_pair(3, day));

    // Create tmp coordinates.
    std::vector<std::pair<double, double>> coordinates;
    coordinates.emplace_back(std::make_pair(20, 30));
    coordinates.emplace_back(std::make_pair(30, 20));

    // Create tmp fences.
    Fence *polyFence = new PolyFence(false, week, coordinates);
    Fence *roundFence = new RoundFence(true, week, 10, 20, 10);

    std::vector<Fence*> fences;
    fences.emplace_back(polyFence);
    fences.emplace_back(roundFence);

    // Create temp contacts.
    Contact* contact1 = new Contact("Daniels", "Vasiljevs", "0121DO1", "Coke");
    Contact* contact2 = new Contact("Ben", "Smith", "124232", "Pepsi");

    std::vector<Contact*> contacts;
    contacts.push_back(contact1);
    contacts.push_back(contact2);

    Account account(contacts, fences);
    account.saveSerialisedAccount(ACCOUNT_PATH);
}

int main()
{

    createAccount();

    // Create an instance of the manager.
    auto beeSafeManager = new BeeSafeManager();
    if (!beeSafeManager->init()) {
        return EXIT_FAILURE;
    }

    // Start the manager loop.
    return beeSafeManager->start()
           ? EXIT_SUCCESS
           : EXIT_FAILURE;
}