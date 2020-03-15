#include "Monitor.h"

// Explicit constructor is used to define the account and the communications link.
Monitor::Monitor(Communication *communication, Account *account)
{
    this->communication = communication;
    this->account = account;

    this->monitorThreadRunning = false;
    this->monitorThread = nullptr;
    this->monitorState = nullptr;
}

// Constructor simply initialises the monitor thread class with the communications link.
Monitor::Monitor(Communication *communication)
{
    this->communication = communication;

    this->account = nullptr;
    this->monitorThreadRunning = false;
    this->monitorThread = nullptr;
    this->monitorState = nullptr;
}

// Starts the thread with the existent account details.
bool Monitor::start()
{
    return start(account);
}

// Starts the thread with a new account.
bool Monitor::start(Account *account)
{

    // Requisites must be present i.e. communications and account.
    if (communication == nullptr || account == nullptr) {
        return false;
    }

    // Check if the thread is currently running, stop if so.
    if (monitorThreadRunning && monitorThread != nullptr) {
        stop();
    }

    // Update the account pointer if necessary.
    if (this->account != account) {
        this->account = account;
    }

    // Finally, start the thread and return.
    monitorThread = new std::thread(Monitor::execute, this);
    return true;
}

// Stops the monitor thread.
void Monitor::stop()
{
    // Stop the thread.
    monitorThreadRunning = false;
    monitorThread->join();

    // Handle thread cleanup.
    delete monitorThread;
    monitorThread = nullptr;
}

// The main loop that is being executed.
void Monitor::run()
{

    // TODO: Initialise the required resources.

    // Update the monitorThreadRunning flag.
    monitorThreadRunning = true;

    // The main game loop.
    while (monitorThreadRunning) {

        // TODO: Detection code resides in here.

    }
}