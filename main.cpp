#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

void childProcess(string msg)
{
    int pid = fork();
    if (pid > 0)
    {
        return;
    }
    else if (pid == 0)
    {
        cout << msg << endl;
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
    childProcess("b");
    childProcess("c");
    cout << "a" << endl;
    return 0;
}