#define UNUSED(expr) do { (void)(expr); } while (0)

extern "C"
{    
    int _close(int file)
    {
        UNUSED(file);
        return -1;  // Not implemented, always fails
    }

    int _fstat(int file, struct stat *st)
    {
        UNUSED(file);
        UNUSED(st);

        return 0;
    }

    int _isatty(int file)
    {
        UNUSED(file);

        return 1;
    }

    int _lseek(int file, int *ptr, int dir)
    {
        UNUSED(file);
        UNUSED(ptr);
        UNUSED(dir);

        return 0;  // Always successful as we're not implementing a filesystem
    }

    int _read(int file, char *ptr, int len)
    {
        UNUSED(file);
        UNUSED(ptr);
        UNUSED(len);

        return 0;  // Not implemented, always returns EOF
    }

    int _write(int file, char *ptr, int len)
    {
        UNUSED(file);
        UNUSED(ptr);
        UNUSED(len);

        // Implement your write function here
        // For example, you could write to UART or any other peripheral
        return len;  // Return the number of bytes written
    }

    void _exit(int status)
    {
        UNUSED(status);

        while (1)
        {
        }  // Infinite loop
    }

    void _kill(int pid, int sig)
    {
        UNUSED(pid);
        UNUSED(sig);

        return;  // Not implemented
    }

    int _getpid(void)
    {
        return 1;  // Return a dummy PID
    }

}  // extern "C"