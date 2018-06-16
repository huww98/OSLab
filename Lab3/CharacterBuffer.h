#include <deque>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>

class CharacterBuffer
{
    friend std::ostream &operator<<(std::ostream &out, CharacterBuffer &buffer);

  private:
    std::deque<char> chars;
    std::mutex mutex_;
    std::condition_variable cv;
    bool tryGet(std::function<bool(char)> condition, char &c);

  public:
    void put(char c);
    char getLower();
    char getUpper();
    char get(std::function<bool(char)> condition = [](char) { return true; });
};

std::ostream &operator<<(std::ostream &out, CharacterBuffer &buffer);
