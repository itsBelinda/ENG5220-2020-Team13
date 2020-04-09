#include "UArt.h"

#include <stdexcept>
#include <sys/ioctl.h>

UArt::UArt()
{
    device = -1;
    conf();
}

/**
 * Destructor is used to close the comms i.e. the Uart
 * serial interface.
 */
UArt::~UArt()
{
    if (device != -1) {
        close(device);
    }
}

int UArt::conf()
{

    // Open the device.
    device = open(DEVICE_PATH, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (device == -1) {
        fprintf(stderr, "Failed to open device\r\n");
        fprintf(stderr, "Error: %s\n",strerror(errno));
        return -1;
    }

    // Get the current device configuration.
    struct termios configuration = {0};
    if (tcgetattr(device, &configuration) != 0) {
        goto err;
    }

    // Reconfigure the device.
    configuration.c_cflag &= ~CSTOPB;
    configuration.c_cflag |= CLOCAL;
    configuration.c_cflag |= CREAD;
    configuration.c_cc[VTIME] = 0;
    configuration.c_cc[VMIN] = 0;
    configuration.c_iflag = 0;
    configuration.c_oflag = 0;
    configuration.c_lflag = 0;

    // Set the speed.
    if (cfsetspeed(&configuration, DEVICE_BAUD_RATE)) {
        goto err;
    }

    // TODO: Sets the terminal?
    cfmakeraw(&configuration);

    // Set the parameters associated with the terminal.
    if (tcsetattr(device, TCSANOW, &configuration) < 0
        || tcsetattr(device, TCSAFLUSH, &configuration) < 0) {
        goto err;
    }

    // Device successfully configured, return.
    return 0;

    // Handle any errors during the configuration process.
err:

    // Display the errors.
    fprintf(stderr, "Failed to configure the device\r\n");
    fprintf(stderr, "Error: %s\n",strerror(errno));

    // Handle clean-up.
    close(device);
    device = -1;

    return -1;
}

/**
 * Check whether or not the device has been opened and configured.
 * 'device' will always be -1 if the latter is not achieved.
 *
 * @return True if the device is open (!=-1), false otherwise.
 */
bool UArt::isDeviceOpen()
{
    return device != -1;
}

int UArt::getDevice()
{
    return device;
}

/**
 * Peeking algorithm for reading bytes from the serial buffer.
 * Note, this method is capable of receiving larger blocks; using
 * peeking + general timeout, issues pertaining to indefinite timeouts
 * are resolved.
 *
 * @param buffer The char array into which the result will be written.
 * @param bytesExpected The number of bytes that are to be returned.
 * @param timeoutMs How long to wait for data before returning.
 * @return The number of bytes that have successfully been read, -1
 *      otherwise i.e. errors.
 */
ssize_t UArt::readExpected(char *buffer, size_t bytesExpected,
                           int timeoutMs)
{

    // Check that the device has been established.
    if (device == -1) {
        return -1;
    }

    // The number of bytes peeked; the last number of bytes peeked.
    size_t bytesPeeked = 0;
    size_t lastBytesPeeked = 0;

    // Timing related variables.
    struct timespec pause = {0};
    pause.tv_sec = timeoutMs / 1000;
    pause.tv_nsec = timeoutMs * 1000;

    // Keep peeking at the buffer until a timeout.
    for(;;) {

        printf("Bytes peeked: %d\n", (int) bytesPeeked);

        // Update the last number of bytes peeked; break if block is met.
        lastBytesPeeked = bytesPeeked;
        if (bytesPeeked >= bytesExpected) {
            printf("Bytes have met.\n");
            break;
        }

        // Sleep the thread until an interrupt.
        int rc = nanosleep(&pause, NULL);
        printf("Rc: %d\n", rc);

        ioctl(device, FIONREAD, &bytesPeeked);

        // Check if the read has timed out.
        if (bytesPeeked == lastBytesPeeked) {
            printf("Timeout has occurred\n");
            break;
        }
    }

    // Set the buffer bytes to null terminators.
    memset(buffer, '\0', bytesExpected);

    // Read the block regardless of what's within.
    if (bytesPeeked >= bytesExpected) {
        bytesPeeked = bytesExpected;
    }

    // Read whatever bytes are present from the buffer.
    return read(device, buffer, bytesPeeked);
}


/**
 * A non blocking means for reading data from the serial buffer up
 * until the first '\n' character is read.
 *
 * @param resultBuffer The buffer into which the result will be read. Note,
 *      results read into the buffer are invalid if the method returns -1.
 * @param resultBufferLen The length of the buffer into which the results are
 *      read.
 * @param timeoutMs The intra-character timeout. Avoided if character present,
 *      blocks the thread for the timeout ms if not.
 * @return The number of characters read (including '\n'), -1 otherwise.
 */
ssize_t UArt::readNext(char * const resultBuffer, const size_t resultBufferLen,
                       const int timeoutMs)
{

    // Check that the device is present.
    if (device == -1) {
        return -1;
    }

    // The number of bytes that have been peeked and read.
    size_t bytesPeeked = 0;
    ssize_t bytesRead = 0;

    // The next and last read indexes.
    size_t nextReadIndex = 0;
    size_t lastReadIndex = 0;

    // The last read character.
    char lastReadChar;

    // Timeout pause.
    struct timespec timeoutPause = {0};
    timeoutPause.tv_sec = timeoutMs / 1000;
    timeoutPause.tv_nsec = timeoutMs * 1000;

    // Keep reading the buffer until crlf.
    for (;;) {

        // If the buffer has been exceeded, return -1.
        lastReadIndex = nextReadIndex;
        if (lastReadIndex >= resultBufferLen) {
            return -1;
        }

        // If there are no characters within the buffer, sleep.
        ioctl(device, FIONREAD, &bytesPeeked);
        if (bytesPeeked <= 0 && nanosleep(&timeoutPause, nullptr)) {
            return -1;
        }

        // Read in a single character from the serial buffer.
        bytesRead = read(device, &lastReadChar, 1);
        if (bytesRead == -1) {
            return -1;
        } else if (bytesRead == 1) {

            // Write the character to the buffer, check if '\n'.
            resultBuffer[nextReadIndex++] = lastReadChar;
            if (lastReadChar == '\n') {
                break;
            }

            // Skip to the next character.
            continue;
        }

        // If no new bytes have been read and we have timed out, return.
        if (nextReadIndex == lastReadIndex) {
            return -1;
        }
    }

    // Return the number of characters that have been read.
    return nextReadIndex;
}

/**
 * Write a string to the device via the UArt
 * serial interface. Note, the string is converted
 * to a character array and only then written.
 *
 * @param cmd The string command sent via the serial interface.
 * @return The number of chars (bytes) that have been successfully
 *      written to the device, -1 otherwise i.e. error.
 */
ssize_t UArt::writeNext(std::string &cmd)
{

    // Convert the string into a char buffer.
    char cmdBuffer[cmd.size() + 1];
    strcpy(cmdBuffer, cmd.c_str());

    // Attempt to write the converted command to the device.
    return writeNext(cmdBuffer);
}

/**
 * Write a character array (C-string) to the device via the Uart
 * serial interface.
 *
 * @param cmdBuffer The C-string command that is to be written to the
 *      device.
 * @return The number of chars (bytes) that have been successfully
 *      written to the device, -1 otherwise i.e. error.
 */
ssize_t UArt::writeNext(const char *cmdBuffer)
{

    // If the device is present, write a command.
    if (device != -1 && tcflush(device, TCIFLUSH) == 0) {
        return write(device, cmdBuffer, strlen(cmdBuffer) + 1);
    }

    // Failed to write to serial.
    return -1;
}

