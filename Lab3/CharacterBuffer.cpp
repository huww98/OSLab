#include <thread>
#include "CharacterBuffer.h"

using namespace std;

void CharacterBuffer::put(char c)
{
    {
        unique_lock<std::mutex> lk(mutex_);
        chars.push_back(c);
        cout << "Producing " << c << endl
             << "  current buffer: " << *this << endl;
    }
    cv.notify_one();
}

bool CharacterBuffer::tryGet(std::function<bool(char)> condition, char &outc)
{
    for (auto it = chars.begin(); it != chars.end(); it++)
    {
        char c = *it;
        if (condition(c))
        {
            outc = c;
            chars.erase(it);
            return true;
        }
    }
    return false;
}

bool isLower(char c)
{
    return c >= 'a' && c <= 'z';
}

bool isUpper(char c)
{
    return c >= 'A' && c <= 'Z';
}

char CharacterBuffer::getLower()
{
    return get(isLower);
}

char CharacterBuffer::getUpper()
{
    return get(isUpper);
}

char CharacterBuffer::get(std::function<bool(char)> condition)
{
    unique_lock<mutex> lk(mutex_);
    char c;
    cv.wait(lk, [this, &c, condition] { return this->tryGet(condition, c); });
    cout << "Consuming " << c << endl
         << "  current buffer: " << *this << endl;
    return c;
}

ostream &operator<<(ostream &out, CharacterBuffer &buffer)
{
    for (auto &&c : buffer.chars)
    {
        out << c << " ";
    }
    return out;
}
