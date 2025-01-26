class Error
{
   public:
    using ErrorCallback = void (*)(int);  // Callback function pointer

   private:
    int lastErrorCode;            // Stores the last error code
    ErrorCallback errorCallback;  // Callback function

   public:
    void triggerError(int errorCode);

   public:
    Error() : lastErrorCode(0), errorCallback(nullptr) {}
    virtual ~Error() {}

    void setErrorCallback(ErrorCallback callback);
    int getLastError() const;
    void clearError();
};
