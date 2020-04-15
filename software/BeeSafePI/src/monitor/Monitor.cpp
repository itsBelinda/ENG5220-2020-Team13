#include "Monitor.h"

#define CONNECT_PSD_TRIES 3

/**
 * Constrictor explicitly initialises the monitor thread with the
 * necessary parameters.
 *
 * @param comms The communications interface used to obtain the location and send messages.
 * @param account The device account that defines the fences and contacts.
 */
Monitor::Monitor(Comms * const comms, Account * const account)
{
    this->comms = comms;
    this->account = account;

    this->monitorThreadRunning = false;
    this->monitorThread = nullptr;
    this->monitorState = nullptr;
}

/**
 * Constructor initialises the monitor thread only with the communications
 * interface. Note, starting will require the account to be passed to the
 * thread.
 *
 * @param comms The communications interface used to obtain the location of the
 *      device and send messages.
 */
Monitor::Monitor(Comms * const comms)
{
    this->comms = comms;

    this->account = nullptr;
    this->monitorThreadRunning = false;
    this->monitorThread = nullptr;
    this->monitorState = nullptr;
}

/**
 * Start the monitor thread.
 *
 * Attempts to start the thread with the current account. If the thread
 * is already running, calling this function is the equivalent of restarting
 * the thread.
 *
 * @return True if the thread was successfully started, false otherwise.
 */
bool Monitor::start()
{
    return start(account);
}

/**
 * Start the monitor thread.
 *
 * Attempts to start the thread with an explicit account. If the thread
 * is already running, calling this function is the equivalent of restarting
 * the thread.
 *
 * @param account The new account instance that should be used. Note,
 *      this will overwrite the existing account instance.
 * @return True if the thread was successfully started, false otherwise.
 */
bool Monitor::start(Account * const account)
{

    // Requisites must be present i.e. communications and account.
    if (comms == nullptr || account == nullptr) {
        return false;
    }

    // If the monitor thread is running, join it.
    if (monitorThreadRunning && monitorThread != nullptr) {
        join();
    }

    // Potentially update the account.
    if (this->account != account) {
        this->account = account;
    }

    monitorThread = new std::thread(Monitor::execute, this);
    return true;
}

void Monitor::stop()
{
    monitorThreadRunning = false;
    join();
}

/**
 * Stop the monitoring thread.
 *
 * Blocks the invoking thread until the monitor thread has finished
 * executing. Moreover, this will take care of cleaning up any resources
 * occupied by the thread.
 */
void Monitor::join()
{
    // Waits for the thread to join.
    monitorThread->join();

    // Handle thread cleanup.
    delete monitorThread;
    monitorThread = nullptr;

    // Clean up the monitor state.
    delete monitorState;
    monitorState = nullptr;
}

/**
 * Run the monitoring thread loop.
 *
 * While the thread is running, it can either be in active or passive state.
 * Each performs different tasks.
 *
 */
void Monitor::run()
{

    // Initialise the thread state.
    monitorState = new PassiveMonitorState(comms, account);
    monitorThreadRunning = true;

    // Generic return code.
    bool rc = false;

    // PSD related variables.
    int psdConnectionTries = 0;
    std::string psdUrc;
    bool psdConnected = false;

    // Pair into which the latitude and longitude shall be stored.
    std::pair<double, double> latLng;

    // The main monitoring thread.
    MonitorState *toMonitorState = nullptr;
    while (monitorThreadRunning) {

        // Check that we have access to the internet.
        rc = comms->hasPSD(psdConnected);
        if (!rc) {
            psdConnectionTries = 0;
            do {

                psdConnectionTries++;
                std::cout << "PSD connection attempt " << psdConnectionTries << " / " << CONNECT_PSD_TRIES << "..." << std::endl;

                // Attempt to reconnect.
                rc = comms->connectPSD(psdConnected, psdUrc);
                if (rc && psdConnected) {
                    std::cout << "... PSD successfully connected." << std::endl;
                    break;
                } else {
                    std::cerr << "... PSD connection attempt " << psdConnectionTries << " / " << CONNECT_PSD_TRIES << " failed." << std::endl;
                }

            } while (psdConnectionTries < CONNECT_PSD_TRIES);
        }

        // If we are not connected try again.
        if (!psdConnected) {
            std::cerr << "Failed to connect PSD." << std::endl;
            continue;
        }

        // Try to get the latitude and longitude.
        std::cout << "Getting device coordinates..." << std::endl;
        rc = comms->getLocation(latLng);
        if (!rc) {
            std::cerr << "Failed to get the location." << std::endl;
            continue;
        }
        std::cout << "... device coordinates (lat: " << latLng.first << ", lng: " << latLng.second << ") successfully obtained." << std::endl;


        // Permit the monitor state to handle the location; update state if necessary.
        /*
        toMonitorState = monitorState->handleLatLng(latLng);
        if (toMonitorState != nullptr) { // if its null --> no change
            delete monitorState;
            monitorState = toMonitorState;
        }
         */
    }
}