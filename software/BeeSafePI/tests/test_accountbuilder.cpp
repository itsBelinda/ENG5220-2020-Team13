#include <iostream>
using namespace std;

#include "../src/device/AccountBuilder.cpp"
#include "../src/device/Account.cpp"
#include "../src/geo/Fence.cpp"
#include "../src/geo/PolyFence.cpp"
#include "../src/geo/RoundFence.cpp"
#include "../src/contact/Contact.cpp"

int main () {
//-------setup-------
std::string failures = "";

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
    Account testAccount(name, contacts, fences);

    try {
        // Attempt to read the file.
        utility::ifstream_t ifStream;
        ifStream.open("../../data/TestAccount.json");

        utility::stringstream_t stream;
        stream << ifStream.rdbuf();
        std::error_code errorCode;
        AccountBuilder builder(stream, errorCode);


        //-----build()-----
        auto account = builder.build();
        if(account == NULL){failures+="build;";}

        std::vector<Contact*>& acctContVector = account->getContacts();
        std::string acctContVectorData = "";
        for (auto &c : acctContVector){
            web::json::value jsonVal = web::json::value::object();
            jsonVal = c->serialiseContact();
            acctContVectorData = jsonVal.serialize();
        }

        std::vector<Contact*>& testacctContVector = testAccount.getContacts();
        std::string testacctContVectorData = "";
        for (auto &c : testacctContVector){
            web::json::value jsonVal = web::json::value::object();
            jsonVal = c->serialiseContact();
            testacctContVectorData = jsonVal.serialize();
        }

        std::vector<Fence*>& acctFenceVector = account->getFences();
        std::string acctFenceVectorData = "";
        for (auto &c : acctFenceVector) {
            web::json::value jsonVal = web::json::value::object();
            jsonVal = c->serialiseFence();
            acctFenceVectorData = jsonVal.serialize();
        }

        std::vector<Fence*>& testacctFenceVector = testAccount.getFences();
        std::string testacctFenceVectorData = "";
        for (auto &c : testacctFenceVector){
            web::json::value jsonVal = web::json::value::object();
            jsonVal = c->serialiseFence();
            testacctFenceVectorData = jsonVal.serialize();
        }

        if(account->getName()!=testAccount.getName() ||
            acctContVectorData != testacctContVectorData ||
            acctFenceVectorData != testacctFenceVectorData)
        {failures+="buildAccountData;";}

    } catch (web::json::json_exception &e) {
        std::cerr << e.what() << std::endl;
    }

    //TODO FIX: the following gives segfault
//    delete[](schoolFence);
//    delete[](pathFence);
//    delete[](homeFence);
//    delete[](contact1);

    //-----return test state result-----
    if(failures != ""){
        cout << "Test failed" << endl;
        cout << failures <<endl;
    }
    else { cout << "Test passed";}
}
