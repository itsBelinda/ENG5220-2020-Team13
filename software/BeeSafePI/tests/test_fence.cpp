#include <iostream>
using namespace std;
#include "../src/geo/Fence.cpp"
#include "../src/geo/Fence.h"
#include "../src/geo/RoundFence.h"
#include "../src/geo/PolyFence.h"

//TODO REVIEW: do we need a separate class to check Fence functionality or is incorporating its methods in Poly/Round enough?

int main () {
    //-------setup-------
    std::string failures = "";

    std::vector<std::pair<std::tm, std::tm>> day;
    std::map<int, std::vector<std::pair<std::tm, std::tm>>> week;

    std::tm fromTime = {0};
    fromTime.tm_hour = 12;
    fromTime.tm_min = 30;
    std::tm toTime = {0};
    toTime.tm_hour = 16;
    toTime.tm_min = 30;

    day.push_back(std::pair<std::tm, std::tm>(fromTime, toTime));
    day.push_back(std::pair<std::tm, std::tm>(fromTime, toTime));

    week.insert(std::pair<int, std::vector<std::pair<std::tm, std::tm>>>(0, day));
    week.insert(std::pair<int, std::vector<std::pair<std::tm, std::tm>>>(1, day));
    week.insert(std::pair<int, std::vector<std::pair<std::tm, std::tm>>>(3, day));
    week.insert(std::pair<int, std::vector<std::pair<std::tm, std::tm>>>(6, day));

//    Fence* fence1 = new RoundFence(true, week,0,0,10);
//
//    //-----serialiseFence()-----
//    //tested for in Round/PolyFence I guess
//
//    //-----isInTime()-----
//    //Note: adjust to and fromTimes at the beginning of test for this to pass
//    if(!(fence1->isInTime())){
//        cout << "Test failed";
//    }
//
//    std::tm testTime = {0};
//    testTime.tm_hour = 13;
//    testTime.tm_min = 30;
//
//    //-----isInTime(...)-----
//    if(!(fence1->isInTime(testTime))){
//
//    }

    //----isInside()------

    //-----return test state result-----
    if(failures != ""){
        cout << "Test failed" << endl;
        cout << failures <<endl;
    }
    else { cout << "Test passed";}
}
