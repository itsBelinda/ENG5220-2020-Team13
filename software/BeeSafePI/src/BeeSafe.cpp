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
        monitor->join();
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
        this->monitor->join();
        delete this->monitor;
    }
    delete this->comms;
    delete this->account;

    // First, attempt to init comms.
    std::cout << "Initialising comms interface..." << std::endl;
    auto comms = initComms();
    if (comms == nullptr) {
        std::cerr << "Failed to initialise comms interface." << std::endl;
        return false;
    }
    std::cout << "... comms interface successfully initialised." << std::endl;

    // Attempt to initialise the monitor thread.
    std::cout << "Initialising monitor thread..." << std::endl;
    auto monitor = initMonitor(comms);
    if (monitor == nullptr) {
        std::cerr << "Failed to initialise monitor thread." << std::endl;
        delete comms;
        return false;
    }
    std::cout << "... monitor thread successfully initialised." << std::endl;

    // The account can be null.
    std::cout << "Initialising account..." << std::endl;
    auto account = initAccount(ACCOUNT_PATH);
    if (account == nullptr) {
        std::cerr << "Failed to initialise account." << std::endl;
    } else {
        account->saveSerialisedAccount("/home/dans/dev/software/BeeSafePI/Account_Verif.json");
        std::cout << "... account successfully initialised." << std::endl;
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
        std::cout << "Comms initialisation attempt " << tries << " / " << INIT_COMMS_TRIES << "..." <<  std::endl;
        init = comms->init();
        if (init) {
            break;
        } else {
            std::cerr << "... comms initialisation attempt " << tries << " / " << INIT_COMMS_TRIES << " failed." << std::endl;
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
            std::cerr << "Failed to load account file: " << ACCOUNT_PATH
                      << ". Maybe doesn't exist yet?" << std::endl;
            return nullptr;
        } else if (!ifStream.is_open()) {
            std::cerr << "Failed to open account file: " << ACCOUNT_PATH << "." << std::endl;
            return nullptr;
        }

        // Stream the file.
        utility::stringstream_t sStream;
        sStream << ifStream.rdbuf();

        // Attempt to build the JSON file.
        std::error_code ec;
        AccountBuilder accountBuilder(sStream, ec);
        auto account = accountBuilder.build();

        if (account == nullptr) {
            std::cerr << "Failed to build account file: " << ACCOUNT_PATH
                      << ". Error code: " << ec.value() << ", message: " << ec.message() << std::endl;
        }

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
        monitor->join();
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

    // Create school coordinates.
    std::vector<std::pair<double, double>> schoolCoords;
    schoolCoords.emplace_back(std::make_pair(55.8295041, -4.2118943));
    schoolCoords.emplace_back(std::make_pair(55.8295026, -4.2118996));
    schoolCoords.emplace_back(std::make_pair(55.8294408, -4.2119104));
    schoolCoords.emplace_back(std::make_pair(55.8294423, -4.2119613));
    schoolCoords.emplace_back(std::make_pair(55.8293232, -4.2119533));
    schoolCoords.emplace_back(std::make_pair(55.8293172, -4.2120498));
    schoolCoords.emplace_back(std::make_pair(55.8292328, -4.2120418));
    schoolCoords.emplace_back(std::make_pair(55.8292252, -4.2121276));
    schoolCoords.emplace_back(std::make_pair(55.8290655, -4.2121169));
    schoolCoords.emplace_back(std::make_pair(55.8290504, -4.2121813));
    schoolCoords.emplace_back(std::make_pair(55.8289449, -4.2121598));
    schoolCoords.emplace_back(std::make_pair(55.8289479, -4.2120928));
    schoolCoords.emplace_back(std::make_pair(55.8289615, -4.2117736));
    schoolCoords.emplace_back(std::make_pair(55.8290112, -4.2111540));
    schoolCoords.emplace_back(std::make_pair(55.8292358, -4.2111620));
    schoolCoords.emplace_back(std::make_pair(55.8292448, -4.2112345));
    schoolCoords.emplace_back(std::make_pair(55.8295086, -4.2112613));
    schoolCoords.emplace_back(std::make_pair(55.8295041, -4.2118943));

    // Create path coordinates.
    std::vector<std::pair<double, double>> pathCoords;
    pathCoords.emplace_back(std::make_pair(55.8289916, -4.2116421));
    pathCoords.emplace_back(std::make_pair(55.8289901, -4.2116529));
    pathCoords.emplace_back(std::make_pair(55.8289765, -4.2117923));
    pathCoords.emplace_back(std::make_pair(55.8289569, -4.2121732));
    pathCoords.emplace_back(std::make_pair(55.8289057, -4.2128813));
    pathCoords.emplace_back(std::make_pair(55.8284158, -4.2128465));
    pathCoords.emplace_back(std::make_pair(55.8284384, -4.2123529));
    pathCoords.emplace_back(std::make_pair(55.8283389, -4.2122912));
    pathCoords.emplace_back(std::make_pair(55.828345, -4.2121169));
    pathCoords.emplace_back(std::make_pair(55.8282877, -4.2121142));
    pathCoords.emplace_back(std::make_pair(55.8282798, -4.2133507));
    pathCoords.emplace_back(std::make_pair(55.8282482, -4.2133668));
    pathCoords.emplace_back(std::make_pair(55.828212, -4.2133829));
    pathCoords.emplace_back(std::make_pair(55.8282108, -4.2130637));
    pathCoords.emplace_back(std::make_pair(55.8282289, -4.2119908));
    pathCoords.emplace_back(std::make_pair(55.8282862, -4.2119855));
    pathCoords.emplace_back(std::make_pair(55.8283947, -4.2120203));
    pathCoords.emplace_back(std::make_pair(55.8283902, -4.2121652));
    pathCoords.emplace_back(std::make_pair(55.8284399, -4.2121625));
    pathCoords.emplace_back(std::make_pair(55.828548, -4.2121491));
    pathCoords.emplace_back(std::make_pair(55.8285273, -4.2127177));
    pathCoords.emplace_back(std::make_pair(55.8287745, -4.2127097));
    pathCoords.emplace_back(std::make_pair(55.8287836, -4.2122403));
    pathCoords.emplace_back(std::make_pair(55.8288559, -4.2122591));
    pathCoords.emplace_back(std::make_pair(55.8288997, -4.2116126));
    pathCoords.emplace_back(std::make_pair(55.8289916, -4.2116421));

    std::string schoolFenceName = "School";
    std::string pathFenceName = "Path";
    std::string homeFenceName = "Home";

    // Create tmp fences.
    Fence *schoolFence = new PolyFence(schoolFenceName, true, week, schoolCoords);
    Fence *pathFence = new PolyFence(pathFenceName, true,week,pathCoords);
    Fence *homeFence = new RoundFence(homeFenceName, true, week, 55.828182, -4.213159, 10);

    std::vector<Fence*> fences;
    fences.emplace_back(schoolFence);
    fences.emplace_back(pathFence);
    fences.emplace_back(homeFence);

    // Create temp contacts.
    Contact* contact1 = new Contact("Daniels", "Vasiljevs", "0121DO1", "Coke");
    Contact* contact2 = new Contact("Ben", "Smith", "124232", "Pepsi");

    std::vector<Contact*> contacts;
    contacts.push_back(contact1);
    contacts.push_back(contact2);

    std::string name = "AccountNameTest";
    Account account(name, contacts, fences);
    account.saveSerialisedAccount(ACCOUNT_PATH);
}

int main()
{

    createAccount();

    // Create an instance of the manager.
    auto beeSafeManager = new BeeSafeManager();

    // Initialise the BeeSafePI manager.
    std::cout << "Initialising manager..." << std::endl;
    if (!beeSafeManager->init()) {
        std::cerr << "Failed to initialise the manager. Stopping." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "... manager successfully initialised." << std::endl;

    // Start the manager loop.
    return beeSafeManager->start()
           ? EXIT_SUCCESS
           : EXIT_FAILURE;
}
