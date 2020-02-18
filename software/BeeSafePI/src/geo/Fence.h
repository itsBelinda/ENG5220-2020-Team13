#ifndef BEESAFEPI_FENCE_H
#define BEESAFEPI_FENCE_H

#include <string>

// Define JSON keys for the fence.
#define FENCE_KEY_SAFE "Safe"

// Fence defines the basic geo fence / path interface.
class Fence
{

public:
    Fence(bool safe);

public:
    bool isSafe();

public:
    virtual bool isValid() = 0;
    virtual bool isInside(double latitude, double longitude) = 0;

private:
    bool safe;
    // todo: define timing related information.

};

#endif //BEESAFEPI_FENCE_H
