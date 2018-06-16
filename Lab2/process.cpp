#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

void throwIfError(int ret, string errorMessage)
{
    if (ret != 0)
        throw new runtime_error(errorMessage);
}

void output(string msg)
{
    throwIfError(lockf(STDOUT_FILENO, F_LOCK, 0), "lockf failed");
    for (char c : msg)
    {
        cout << c << flush;
        this_thread::yield();
    }
    cout << endl;
    throwIfError(lockf(STDOUT_FILENO, F_ULOCK, 0), "lockf unlock failed");
}

void childProcess(string msg)
{
    pid_t pid = fork();
    if (pid > 0)
    {
        return;
    }
    else if (pid == 0)
    {
        output(msg);
        exit(0);
    }
    else
    {
        cerr << "error while fork" << endl;
        exit(1);
    }
}

int main()
{
    childProcess("this is from process B. A very loooooooooooooooog sentence.");
    childProcess("this is from process C. A very loooooooooooooooog sentence.");
    output("this is from process A. A very loooooooooooooooog sentence.");
    return 0;
}
