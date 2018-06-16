#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <mutex>
#include <random>
#include "CharacterBuffer.h"

using namespace std;

CharacterBuffer buffer;

uniform_real_distribution<> dist;
default_random_engine engine;
auto randFunc = [] { return dist(engine); };

void sleepRand(double maxSec)
{
    double r = randFunc();
    auto duration = chrono::duration<double>(r);
    this_thread::sleep_for(duration * maxSec);
}

void producer(char base)
{
    while (true)
    {
        for (char c = base; c < base + 26; c++)
        {
            sleepRand(1.0);
            buffer.put(c);
        }
    }
}

void consumer(function<char()> &&f)
{
    while (true)
    {
        sleepRand(1.2);
        f();
    }
}

int main(int argc, char const *argv[])
{
    thread p1(producer, 'a');
    thread p2(producer, 'A');
    thread c1(consumer, [] { return buffer.getLower(); });
    thread c2(consumer, [] { return buffer.getUpper(); });
    thread c3(consumer, [] { return buffer.get(); });

    p1.join();
    return 0;
}
