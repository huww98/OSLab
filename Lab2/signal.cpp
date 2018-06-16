#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;
using namespace std::chrono_literals;

void throwIfError(int ret, string errorMessage)
{
    if (ret != 0)
        throw new runtime_error(errorMessage);
}

vector<pid_t> childPids;

void handler(int signum)
{
    for (auto &pid : childPids)
    {
        kill(pid, SIGUSR1);
    }
    wait(nullptr);
    throwIfError(lockf(STDERR_FILENO, F_LOCK, 0), "lockf failed");
    cerr << "Parent Process is Killed! signum: " << signum << endl;
    throwIfError(lockf(STDERR_FILENO, F_LOCK, 0), "lockf unlock failed");
}

template <int num>
void childHandler(int signum)
{
    throwIfError(lockf(STDERR_FILENO, F_LOCK, 0), "lockf failed");
    cerr << "Child Process " << num << " is Killed by parent! signum: " << signum << endl;
    throwIfError(lockf(STDERR_FILENO, F_LOCK, 0), "lockf unlock failed");
}

template <int num>
pid_t childProcess()
{
    pid_t pid = fork();
    if (pid > 0)
    {
        return pid;
    }
    else if (pid == 0)
    {
        struct sigaction act;
        act.sa_handler = childHandler<num>;
        sigaction(SIGUSR1, &act, nullptr);
        struct sigaction actign;
        actign.sa_handler = SIG_IGN;
        sigaction(SIGQUIT, &actign, nullptr);
        sigaction(SIGINT, &actign, nullptr);
        sigaction(SIGTERM, &actign, nullptr);
        pause();
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
    childPids.push_back(childProcess<1>());
    childPids.push_back(childProcess<2>());

    struct sigaction act;
    act.sa_handler = handler;
    sigaction(SIGTERM, &act, nullptr);
    sigaction(SIGINT, &act, nullptr);
    sigaction(SIGQUIT, &act, nullptr);
    pause();
    return 0;
}
