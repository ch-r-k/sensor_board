#include "error.hpp"  // Assuming ErrorBase is in this header

void Error::triggerError(
    int errorCode)  // Protected so derived classes can use it
{
    lastErrorCode = errorCode;  // Store the error
    if (errorCallback)          // Trigger callback if registered
    {
        errorCallback(errorCode);
    }
}

void Error::setErrorCallback(ErrorCallback callback)
{
    errorCallback = callback;  // Register the callback
}

int Error::getLastError() const
{
    return lastErrorCode;  // Return the last error code
}

void Error::clearError()
{
    lastErrorCode = 0;  // Clear the last error
}