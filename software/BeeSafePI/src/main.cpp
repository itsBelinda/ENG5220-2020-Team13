#include "device/Account.h"
#include "contact/Contact.h"
#include "geo/RoundFence.h"
#include "geo/PolyFence.h"

#include <iostream>


// TODO: Mark constructor with explicit if time related information is not included.
// TODO: Implement time related information.
// TODO: Check something to do with the contact constructors using strings.
// TODO: Use a const iterator by reference in fence isPresent code.
// TODO: Check if a null ptr can be returned for a reference.
// TODO: Implement the code for serialising the RoundFence lat, lng, rds.

int main()
{

    std::map<int, std::vector<std::pair<std::tm, std::tm>>> week;
    

    std::vector<std::pair<double, double>> coordinates;
    coordinates.push_back(std::pair(30, 40));
    coordinates.push_back(std::pair(50, 60));

    Fence* polyFence = new PolyFence(false, week, coordinates);

    Fence* roundFence = new RoundFence(true, week, 10, 20, 10);

    std::vector<Fence*> fences;
    fences.push_back(roundFence);
    fences.push_back(polyFence);

    Contact* contact1 = new Contact("Daniels", "Vasiljevs", "0121DO1", "Coke");
    Contact* contact2 = new Contact("Ben", "Smith", "124232", "Pepsi");

    std::vector<Contact*> contacts;
    contacts.push_back(contact1);
    contacts.push_back(contact2);

    Account account(contacts, fences);

    return 0;
}