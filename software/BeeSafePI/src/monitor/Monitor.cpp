#include "Monitor.h"

// Constructor is used to initialise the Monitor thread.
Monitor::Monitor()
{
    monitorThreadRunning = false;
    monitorThread = nullptr;
}

// Function starts the thread, stopping the previous thread if it is monitorThreadRunning.
bool Monitor::start()
{

    // TODO: Check that the relevant variables are present.

    // If the thread is currently monitorThreadRunning, stop it.
    if (monitorThreadRunning && monitorThread != nullptr) {
        stop();
    }

    // Start the new thread.
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

    // TODO: Handles data cleanup.


}

// The main loop that is being executed.
void Monitor::run()
{

    // Update the monitorThreadRunning flag.
    monitorThreadRunning = true;

    // The main game loop.
    while (monitorThreadRunning) {

        // TODO: HERE!

    }
}