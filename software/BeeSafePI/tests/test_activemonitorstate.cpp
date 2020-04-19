#include <iostream>
#include "../src/contact/Contact.cpp"
#include "../src/geo/Fence.cpp"
#include "../src/geo/RoundFence.cpp"
#include "../src/device/Account.cpp"
//#include "../src/comms/Comms.h"
//#include "../src/monitor/states/ActiveMonitorState.h"
#include "../src/monitor/states/MonitorState.h"

using namespace std;

int main () {
//-------setup-------
    std::string failures = "";

    //TODO FIX this undef ref error
//    auto comms = new Comms();
//
//    comms->init();
//    comms->init();

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

    std::string homeFenceName = "Home";

    // Create tmp fences.
    Fence *homeFence = new RoundFence(homeFenceName, true, week, 55.828182, -4.213159, 10);

    std::vector<Fence*> fences;
    fences.emplace_back(homeFence);

    // Create temp contacts.
    Contact* contact1 = new Contact("Daniels", "Vasiljevs", "+447455787051", "Coke");

    std::vector<Contact*> contacts;
    contacts.push_back(contact1);

    std::string name = "AccountNameTest";
    Account testAccount(name, contacts, fences);
/*
    MonitorState *monitorState = new ActiveMonitorState(comms, &testAccount);
    //-----test handleLatLng-----
    //TODO Can't test until compiles...
*/
    if(failures != ""){
        cout << "Test failed" << endl;
        cout << failures <<endl;
    }
    else { cout << "Test passed";}
}
