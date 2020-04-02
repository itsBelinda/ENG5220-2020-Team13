
#include "comms/UBlox.h"
#include "device/AccountBuilder.h"
#include "device/Account.h"
#include "contact/Contact.h"
#include "geo/RoundFence.h"
#include "geo/PolyFence.h"
#include "monitor/Monitor.h"

#include <iostream>


// TODO: Mark constructor with explicit if time related information is not included.
// TODO: Implement time related information.
// TODO: Check something to do with the contact constructors using strings.
// TODO: Use a const iterator by reference in fence isPresent code.
// TODO: Check if a null ptr can be returned for a reference.
// TODO: Implement the code for serialising the RoundFence lat, lng, rds.

int main()
{

    double lat = 0.0;
    double lng = 0.0;
    std::string imei;
    UBlox uBlox;
    uBlox.getModelNumber(imei);
    uBlox.getIMEI(imei);
    std::cout << imei << std::endl;

    std::string phoneNbr("+447747329169");
    std::string txtMsg("This was sent from the beeSafe PI program."); // avoid \r for simplicity.
    //uBlox.sendMsg(phoneNbr, txtMsg);
    uBlox.tempGetLoc(&lat, &lng);
    std::cout.precision(17);
    std::cout << "Position found: " << lat << " , " << lng << std::endl;


    std::tm fromTime = {0};
    fromTime.tm_hour = 20;
    fromTime.tm_min = 30;

    std::tm toTime = {0};
    toTime.tm_hour = 22;
    toTime.tm_min = 30;

    std::vector<std::pair<std::tm, std::tm>> day;
    day.push_back(std::pair<std::tm, std::tm>(fromTime, toTime));
    day.push_back(std::pair<std::tm, std::tm>(fromTime, toTime));

    std::map<int, std::vector<std::pair<std::tm, std::tm>>> week;
    week.insert(std::pair<int, std::vector<std::pair<std::tm, std::tm>>>(0, day));
    week.insert(std::pair<int, std::vector<std::pair<std::tm, std::tm>>>(1, day));
    week.insert(std::pair<int, std::vector<std::pair<std::tm, std::tm>>>(4, day));


    // TODO: TEST TEST TEST

    std::vector<std::pair<double, double>> coordinates;
    coordinates.push_back(std::pair<double, double>(30, 40));
    coordinates.push_back(std::pair<double, double>(50, 60));

    Fence *polyFence = new PolyFence(false, week, coordinates);

    Fence *roundFence = new RoundFence(true, week, 10, 20, 10);

    std::vector<Fence *> fences;
    fences.push_back(roundFence);
    fences.push_back(polyFence);

    Contact *contact1 = new Contact("Daniels", "Vasiljevs", "0121DO1", "Coke");
    Contact *contact2 = new Contact("Ben", "Smith", "124232", "Pepsi");

    std::vector<Contact *> contacts;
    contacts.push_back(contact1);
    contacts.push_back(contact2);

    Account account(contacts, fences);
    account.saveSerialisedAccount("output.json");

    std::cout << "Finished monitorThreadRunning the code." << std::endl;

    return 0;
}
