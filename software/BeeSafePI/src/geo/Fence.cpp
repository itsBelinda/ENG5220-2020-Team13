#include "Fence.h"

// Defines the constructor for the fence.
Fence::Fence(bool safe)
{
    this->safe = safe;
}

// Whether or not the fence is defined as a safe virtual fence.
bool Fence::isSafe()
{
    return safe;
}