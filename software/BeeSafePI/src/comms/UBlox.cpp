#include <boost/compatibility/cpp_c_headers/cstring>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "UBlox.h"

#define  MAX_CMD_LENGTH 544
// Define the AT commands that are used on the U-Blox device.
// Device identification
#define AT_COMMAND_GET_MODEL_NUMBER "ATI\r"
#define AT_COMMAND_GET_IMEI "ATI5\r"

// get location
#define AT_COMMAND_SET_SCAN "AT+ULOCCELL=1\r" // deep scan
#define AT_COMMAND_GET_LOCATION "AT+ULOC=2,2,0,120,500\r"

// write text message
#define AT_COMMAND_MSG_TXT "AT+CMGF=1\r" // Text message mode
#define AT_COMMAND_MSG_PDU "AT+CMGF=0\r" // PDU message mode
#define AT_CMD_MSG_PT1 "AT+CMGS=\""
// here comes the phone number
#define AT_CMD_MSG_PT2 "\"\r"
// here comes the message
#define AT_CMD_MSG_END "\x1A"
#define AT_CMD_MSG_ESC "\x1B"

// setup Internet connection:
#define AT_COMMAND_GET_GPRS_ATTACH "AT+CGATT?\r"
#define AT_COMMAND_GET_PSD_CONNECT "AT+UPSND=0,8\r"
#define AT_COMMAND_ACTIVATE_PSD "AT+UPSDA=0,3\r"


// Define the states.
#define AT_STATUS_OK "OK"
#define AT_STATUS_ERROR "ERROR"
#define AT_STATUS_ABORTED "ABORTED"

//Define expected responses
#define REPLY_MSG "+CMGS"
#define REPLY_LOC "+UULOC"
#define REPLY_GPRS_ATTACH_0 "+CGATT: 1"
#define REPLY_GPRS_ATTACH_1 "+CGATT: 1"
#define REPLY_PSD_0 "+UPSND: 0,8,0"
#define REPLY_PSD_1 "+UPSND: 0,8,1"


// Define different timeouts
#define ECHO_TIMEOUT 1000 // timeout in ms
#define CHECK_TIMEOUT 10 // timeout in ms
#define RX_TIMEOUT 1000 // timeout in ms
// TODO: only temporary
#define LOC_TIMEOUT 1200000 // timeout in ms (!)

/*
 * This UBlox class is currently only for testing purposes. It is to
 * test and show the basic functionality of the u-blox module with the least
 * effort and in one place.
 * A code refactoring to fit in to the final architecture of the software is
 * required.
 * Timeouts should be reduced drastically or (better) removed completely where
 * possible.
 *
 *
 */
UBlox::UBlox()
{
    locationRequested = false;
    conf();
}

UBlox::~UBlox() = default;

int UBlox::conf()
{
    return -1;
}

Uart &UBlox::getUart()
{
    return uart;
}

int UBlox::getDevice()
{
    return uart.getDevice();
}

bool UBlox::isOpen()
{
    return uart.isOpen();
}

int UBlox::getModelNumber(std::string &modelNumber)
{
    return processCmd(AT_COMMAND_GET_MODEL_NUMBER, modelNumber);
    printf("Writing command\n");

    // Write the at command via uart.
    ssize_t rc = uart.writeBuffer(AT_COMMAND_GET_MODEL_NUMBER);
    if (rc == -1) {
        return -1;
    }

    printf("Command written %d\n", (int) rc);

    // Read the echo, imei and status back from the device.
    char modelNumberBuffer[40] = {'\0'};
    rc = uart.readNext(modelNumberBuffer, 40, 1000);
    printf("Rc: %d, Result: %s", (int) rc, modelNumberBuffer);
    rc = uart.readNext(modelNumberBuffer, 40, 1000);
    printf("Rc: %d, Result: %s", (int) rc, modelNumberBuffer);
    rc = uart.readNext(modelNumberBuffer, 40, 1000);
    rc = uart.readNext(modelNumberBuffer, 40, 1000);
    printf("Rc: %d, Result: %s", (int) rc, modelNumberBuffer);

    return rc;
}


int UBlox::getIMEI(std::string &imei)
{
    //TODO: processing of return needed?
    return processCmd(AT_COMMAND_GET_IMEI, imei);
}

// Todo: the following two functions should be removed
bool UBlox::tempGetLoc(double *const lat, double *const lng)
{
    //
    if (!checkConnections()) {
        std::cerr << "Connection setup/check failed" << std::endl;
        return false;
    }
    if (!checkPSD()) {
        std::cerr << "CPSD check failed" << std::endl;
        return false;
    }

    if (requestLocation() == -1) {
        std::cerr << "Request location failed" << std::endl;
        return false;
    }

    double requestTime = getSysTimeMS();
    double currentTime = getSysTimeMS();

    do {
        if (getLocation(lat, lng) == 0) {
            return true;
        }
        currentTime = getSysTimeMS();
    } while ((currentTime - requestTime) < LOC_TIMEOUT);

    std::cout << "Request location timeout" << std::endl;
    return false;
}

double UBlox::getSysTimeMS()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);;
}

bool UBlox::checkConnections()
{
    std::string gprsAttach;
    processCmd(AT_COMMAND_GET_GPRS_ATTACH, gprsAttach);

    // Check GPRS attach status
    // This should be connected automatically.
    if (!findCharArray(REPLY_GPRS_ATTACH_1, rxBuffer)) {
        //TODO: could be handled
        std::cerr << "GPRS not attached" << std::endl;
        return false;
    }

    // Check PSD connection, this should be done regularly when
    // connected to the internet
    if (!checkPSD()) {
        // If not yet connected at startup, try to connect now.
        return activatePSD();
    } else {
        return true;
    }

}

bool UBlox::checkPSD()
{
    std::string pdsState;
    processCmd(AT_COMMAND_GET_PSD_CONNECT, pdsState);

    if (findCharArray(REPLY_PSD_1, rxBuffer)) {
        std::cout << "PSD activated" << std::endl;
        return true;
    } else if (findCharArray(REPLY_PSD_0, rxBuffer)) {
        std::cerr << "PSD not activated" << std::endl;
        return false;
    } else {
        std::cerr << "PSD unexpected result: " << rxBuffer << std::endl;
        return false;
    }
}

bool UBlox::activatePSD()
{
    std::string pdsState;
    processCmd(AT_COMMAND_ACTIVATE_PSD, pdsState);
    //pdsState(rxBuffer);
    if (!findCharArray(REPLY_PSD_1, rxBuffer)) {
        std::cerr << "PSD not activated" << std::endl;
        return false;
    }
    return true;
}

int UBlox::requestLocation()
{
    // It's not necessary to check if a location request is ongoing, if
    // a location request is ongoing and a new one is sent, this will
    // abort the old one, and start a new one.
    // The (unresolved) result from the old request will be printed out.
    //if( locationRequested == true )

    std::string location;
    //TODO: processing of return needed!
    // TODO: still needs testing (probaly setup)
    if (processCmd(AT_COMMAND_GET_LOCATION) != 0) {
        std::cerr << "Location request failed." << std::endl;
        return -1;
    }
    std::cout << "Location request sent, wait for answer..." << std::endl;
    //
    // TODO: check for answer from  monitoring state directly
    // TODO: ideally, signal from UART if new data is available
    locationRequested = true;
    return 0;
}

int UBlox::getLocation(double *const lat, double *const lng)
{
    if (locationRequested == false) {
        std::cerr << "Location request has not been sent." << std::endl;
    }
    size_t nBytes;
    nBytes = uart.readNext(rxBuffer, MAX_BUFFER_LENGTH, CHECK_TIMEOUT);
    // ignore everything small, this is not the requested result
    if (nBytes < 5) {
        return -1;
    }

    // The expected result is (result type = 0)
    //+UULOC: 13/04/2011,09:54:51.000,45.6334520,13.0618620,49,1
    std::vector<std::string> result;
    boost::split(result, rxBuffer, boost::is_any_of(","));
    for (const auto &res: result) {
        std::cout << res << std::endl;
    }

    if (result[0].find(REPLY_LOC) == std::string::npos) {//TODO: use string functions
        std::cerr << "invalid response found: " << rxBuffer << std::endl;
        return -1;;
    }

    locationRequested = false;
    std::cout << "Response found: " << rxBuffer << std::endl;
    if (result.size() == 6) {
        try {
            *lat = std::stod(result[3]);
            *lng = std::stod(result[4]);
        } catch (const std::invalid_argument &) {
            std::cerr << "Argument is invalid." << std::endl;
            *lat = 0.0;
            *lng = 0.0;
            //throw;
        } catch (const std::out_of_range &) {
            std::cerr << "Argument is out of range for a double." << std::endl;
            *lat = 0.0;
            *lng = 0.0;
            //throw;
        }
    } else {
        std::cerr << "Response could not be read." << std::endl;
        return -1;
    }

    return 0;
}


int UBlox::sendMsg(std::string &nbr, std::string &message)
{
    size_t nBytes;
    char cmdBuffer[32];

    // Message mode needs to be configured (default is 0). It is enough to do this once
    // at startup, but better to at least check every time.
    if (processCmd(AT_COMMAND_MSG_TXT) == -1) {
        return -1;
    }

    // Write the phone number into the AT command
    std::stringstream ss;
    ss << AT_CMD_MSG_PT1 << nbr << AT_CMD_MSG_PT2;
    // Extend lifetime of the reference so it can be passed to UART
    const std::string &cmdString = ss.str();
    nBytes = uart.writeBuffer(cmdString);
    if (nBytes == -1) {
        std::cerr << "UART write error" << std::endl;
        return false;
    }

    // Read back the echo
    nBytes = uart.readNext(rxBuffer, MAX_BUFFER_LENGTH, ECHO_TIMEOUT);
    if (nBytes <= 0) {
        std::cerr << "UART read error or timeout" << std::endl;
        return false;
    }
    std::string reply(rxBuffer);
    if (reply.find(cmdString) != std::string::npos) {
        std::cout << "found echo: " << reply << std::endl;
    }

    if (!findCharArray(AT_CMD_MSG_PT1, rxBuffer)) {
        std::cerr << "invalid cmd echo: " << rxBuffer << std::endl;
        uart.writeBuffer(AT_CMD_MSG_ESC);
        return false;
    }

    // Now the text message can be written to the serial interface
    // In case the message has \r, it would be returned separately (echo)
    // avoid \r in message to not handle that, or read back after every \r
    nBytes = uart.writeBuffer(message);
    if (nBytes == -1) {
        std::cerr << "UART write error" << std::endl;
        // send ESC sequence, in case the u-blox device still expects
        // the text message
        uart.writeBuffer(AT_CMD_MSG_ESC);
        return false;
    }

    // Write send sequence
    nBytes = uart.writeBuffer(AT_CMD_MSG_END);
    if (nBytes == -1) {
        std::cerr << "UART write error" << std::endl;
        // send ESC sequence, in case the u-blox device still expects
        // the text message
        uart.writeBuffer(AT_CMD_MSG_ESC);
        return false;
    }

    // Getting the message back could take a while, it is being sent. (no tests have been made as to how long it
    // actually takes.
    nBytes = uart.readNext(rxBuffer, MAX_BUFFER_LENGTH, 5000);
    if (nBytes == -1) {
        std::cerr << "UART read error or timeout" << std::endl;
        return false;
    }

    nBytes = uart.readNext(rxBuffer, MAX_BUFFER_LENGTH, ECHO_TIMEOUT);

    if (nBytes <= 0) {
        std::cerr << "UART read error or timeout" << std::endl;
        return false;
    }

    if (!checkNoError(rxBuffer)) { // TODO: if we are sure that no valid answer can be 2 bytes, this can be changed
        // to also check for OK,
        return -1;
    }

    if (!findCharArray(REPLY_MSG, rxBuffer)) {
        std::cerr << "invalid echo: " << rxBuffer << std::endl;
        return false;
    }

    if (!checkStatusOK()) {
        return -1;
    }

    return 0;
}

//TODO: only have one function with option to get response or not.
int UBlox::processCmd(const char *const cmd)
{

    if (!sendCmd(cmd)) {
        return -1;// TODO: error codes? or handle here? why -1 and not true/false? @dan
    }

    if (!checkStatusOK()) {
        return -1;
    }

    return 0;

}

int UBlox::processCmd(const char *const cmd, std::string &response)
{
    if (!sendCmd(cmd)) {
        return -1;// TODO: error codes? or handle here? why -1 and not true/false? @dan
    }

    size_t nRx = uart.readNext(rxBuffer, MAX_BUFFER_LENGTH, ECHO_TIMEOUT); // TODO: need for length? @dan

    if (nRx <= 0) {
        std::cerr << "UART read error or timeout" << std::endl;
        return -1;
    }

    std::cout << "answer read: " << rxBuffer;
    response.assign(rxBuffer);

    if (!checkNoError(rxBuffer)) { // TODO: if we are sure that no valid answer can be 2 bytes, this can be
        // changed to also check for OK,
        return -1;
    }


    if (!checkStatusOK()) {
        return -1;
    }
    return 0;

}


bool UBlox::sendCmd(const char *const cmdBuffer)
{
    size_t nBytes;
    nBytes = uart.writeBuffer(cmdBuffer);
    if (nBytes == -1) {
        std::cerr << "UART write error" << std::endl;
        return false;
    }

    nBytes = uart.readNext(rxBuffer, MAX_BUFFER_LENGTH, ECHO_TIMEOUT);

    if (nBytes <= 0) {
        std::cerr << "UART read error or timeout" << std::endl;
        return false;
    }

    std::cout << "echo read: " << rxBuffer;
    if (!findCharArray(cmdBuffer, rxBuffer)) {
        std::cerr << "invalid echo: "<< rxBuffer << std::endl;
        return false;
    }
    return true;
}

bool UBlox::checkStatusOK()
{
    // needs separate buffer to not overwrite the response.
    char checkBuffer[MAX_BUFFER_LENGTH];
    ssize_t nRx = uart.readNext(checkBuffer, MAX_BUFFER_LENGTH, RX_TIMEOUT);

    if (nRx <= 0) {
        std::cerr << "UART read error or timeout" << std::endl;
        return false;
    } else {
        // if the result is OK, the device often (not always) sends \r\n first, if only two bytes are received,
        // read again and check for OK
        if (nRx == 2) {
            nRx = uart.readNext(checkBuffer, MAX_BUFFER_LENGTH, RX_TIMEOUT);
        }
        if (findCharArray(AT_STATUS_OK, checkBuffer)) {
            std::cout << "Status OK" << std::endl;
            return true;
        } else if (!checkNoError(checkBuffer)) {
            return false;
        } else {
            std::cerr << "Status unknown: " << checkBuffer << std::endl;
            return false;
        }
    }
}

bool UBlox::checkNoError(const char *const checkBuffer)
{
    if (findCharArray(AT_STATUS_ERROR, checkBuffer)) {
        std::cerr << "Status ERROR" << std::endl;
        // TODO: error status needs to be handled
        return false;
    } else if (findCharArray(AT_STATUS_ABORTED, checkBuffer)) {
        std::cerr << "Status ABORTED" << std::endl;
        // TODO: aborted status needs to be handled
        return false;
    }
    return true;
}

// todo helper
bool UBlox::findCharArray(const char *const needle, const char *const haystack)
{
    //printf("looking for %s in %s\n", needle , haystack);
    std::string hs(haystack);
    std::string nd(needle);
    return hs.find(nd) != std::string::npos;
}
