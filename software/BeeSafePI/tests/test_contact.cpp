#include <iostream>
using namespace std;
#include "../src/contact/Contact.cpp"

int main () {
    //-------setup-------
    std::string failures = "";

    Contact contact("John", "Doe", "01234123456", "somekey");
    Contact contact2(contact);


    //-----test serialiseContact()-----
    web::json::value testJC = web::json::value::object();
    testJC[utility::string_t(U(JSON_KEY_CONTACT_FORENAME))] = web::json::value::string(U(contact.getForename()));
    testJC[utility::string_t(U(JSON_KEY_CONTACT_SURNAME))] = web::json::value::string(U(contact.getSurname()));
    testJC[utility::string_t(U(JSON_KEY_CONTACT_NUMBER))] = web::json::value::string(U(contact.getNumber()));
    testJC[utility::string_t(U(JSON_KEY_CONTACT_KEY))] = web::json::value::string(U(contact.getKey()));

    web::json::value targetJC = contact.serialiseContact();
    if(targetJC.operator!=(testJC)){ failures+="serialiser;"; }


    //----test copy constructor, as well as serialiser------
    //note: technically no need to test the constructor as there's no logic in the body,
    //but it helps verify the operations of the serialiser

    web::json::value copyJC = contact2.serialiseContact();
    if(targetJC.operator!=(copyJC)){ failures+="copy;"; }

    if(failures != ""){
        cout << "Test failed" << endl;
        cout << failures <<endl;
    }
    else { cout << "Test passed";}
}
