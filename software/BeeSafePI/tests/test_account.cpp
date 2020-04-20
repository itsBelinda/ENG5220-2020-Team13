#include <iostream>
#include "../src/device/Account.cpp"
#include "../src/device/Account.h"
#include "../src/contact/Contact.cpp"
#include "../src/geo/RoundFence.h"
#include "../src/geo/PolyFence.h"
#include "../src/geo/Fence.h"
#include "../src/device/AccountBuilder.h"

#define ACCOUNT_PATH "../../build/data/TestAccount.json"

Account* createAccount(void);

Account* createAccount()
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
    Contact* contact1 = new Contact("Daniels", "Vasiljevs", "+447455787051", "Coke");

    std::vector<Contact*> contacts;
    contacts.push_back(contact1);

    std::string name = "AccountNameTest";
    return new Account(name, contacts, fences);
}

/**
 * By definition, if we create an instance of account, serialise it, and then successfully
 * re-construct it, the account was successfully serialised and built, thus testing both Account
 * and AccountBuilder.
 *
 * @return EXIT_SUCCESS if all the tests have passed, EXIT_FAILURE otherwise.
 */
int main ()
{
    // Test the creation of the account.
    Account *account = createAccount();
    if (account == nullptr) {
        std::cerr << "Failed to create an instance of account." << std::endl;
        return EXIT_FAILURE;
    }

    // Serialise the account.
    if (!account->saveSerialisedAccount(ACCOUNT_PATH)) {
        std::cerr << "Failed to serialise the account instance." << std::endl;
        return EXIT_FAILURE;
    }

    // Using the account builder, create an instance of the account.
    Account *serialisedAccount = nullptr;
    try {

        // Attempt to read the file.
        utility::ifstream_t ifStream;
        ifStream.open(ACCOUNT_PATH);
        if (ifStream.fail()) {
            std::cerr << "Failed to load account file: " << ACCOUNT_PATH
                      << ". Maybe doesn't exist yet?" << std::endl;
            return EXIT_FAILURE;
        } else if (!ifStream.is_open()) {
            std::cerr << "Failed to open account file: " << ACCOUNT_PATH << "." << std::endl;
            return EXIT_FAILURE;
        }

        // Stream the file.
        utility::stringstream_t sStream;
        sStream << ifStream.rdbuf();

        // Attempt to build the JSON file.
        std::error_code ec;
        AccountBuilder accountBuilder(sStream, ec);
        serialisedAccount = accountBuilder.build();

        if (serialisedAccount == nullptr) {
            std::cerr << "Failed to build account file: " << ACCOUNT_PATH
                      << ". Error code: " << ec.value() << ", message: " << ec.message() << std::endl;
            return EXIT_FAILURE;
        }

        // Close the file stream.
        ifStream.close();


    } catch (web::json::json_exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Check if the two accounts are the same.
    if (strcmp(account->serialiseAccount().as_string().c_str(), serialisedAccount->serialiseAccount().as_string().c_str()) != 0) {
        std::cerr << "Failed to yield matching account serialisations." << std::endl;
        return EXIT_FAILURE;
    }

    // Check if the accounts have matching contacts.
    if (account->getContacts().size() != serialisedAccount->getContacts().size()) {
        std::cerr << "Failed to serialise matching contacts." << std::endl;
        return EXIT_FAILURE;
    }

    // Check if the accounts have matching fences.
    if (account->getFences().size() != serialisedAccount->getContacts().size()) {
        std::cerr << "Failed to serialise all fences." << std::endl;
        return EXIT_FAILURE;
    }

    // Check that the names have been serialised correctly.
    if (strcmp(account->getName().c_str(), serialisedAccount->getName().c_str()) != 0) {
        std::cerr << "Failed to serialise account name." << std::endl;
        return EXIT_FAILURE;
    }

    delete account;
    delete serialisedAccount;

    // Account and account builder tests have successfully been completed.
    std::cout << "Test: " << __FILE__ << " has successfully passed." << std::endl;
    return EXIT_SUCCESS;
}
