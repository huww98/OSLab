#include <iostream>
#include <iomanip>

#include "Memory.h"

using namespace std;

constexpr size_t totalInstructionCount = 320;

vector<size_t> generateAccessSequence()
{
    random_device r;
    mt19937 engine(r());
    vector<size_t> seq;

    size_t lastIns = uniform_int_distribution<size_t>(0, Memory::AddressSpace - 1)(engine);
    seq.push_back(lastIns);

    while (true)
    {
        lastIns++;
        lastIns %= Memory::AddressSpace;
        seq.push_back(lastIns);
        if (seq.size() >= totalInstructionCount)
        {
            break;
        }

        lastIns = uniform_int_distribution<size_t>(0, lastIns)(engine);
        lastIns %= Memory::AddressSpace;
        seq.push_back(lastIns);
        if (seq.size() >= totalInstructionCount)
        {
            break;
        }

        lastIns++;
        lastIns %= Memory::AddressSpace;
        seq.push_back(lastIns);
        if (seq.size() >= totalInstructionCount)
        {
            break;
        }

        lastIns = uniform_int_distribution<size_t>(lastIns + 1, Memory::AddressSpace - 1)(engine);
        lastIns %= Memory::AddressSpace;
        seq.push_back(lastIns);
        if (seq.size() >= totalInstructionCount)
        {
            break;
        }
    }
    return seq;
}

void test(int memoryPageCount, vector<size_t> &accessSequence, unique_ptr<ReplaceAlgorithm> argo)
{
    Memory mem(memoryPageCount, move(argo));
    for (auto &s : accessSequence)
    {
        mem.Access(s);
    }
    cout << setw(5) << mem.HitRate() * 100 << "%  ";
}

int main()
{
    vector<size_t> accessSequence = generateAccessSequence();
    cout << "access sequence: ";
    for (auto &s : accessSequence)
    {
        cout << s << " ";
    }
    cout << endl
         << endl;

    vector<size_t> pageAccessSequence(accessSequence);
    for (auto &s : pageAccessSequence)
    {
        s = Memory::GetPageIndex(s);
    }

    constexpr streamsize columnWidth = 8;
    cout << setw(5) << "页框" << setw(columnWidth) << "FIFO" << setw(columnWidth) << "LRU" << setw(columnWidth) << "OPT" << setw(columnWidth) << "NRU" << endl;
    cout << fixed << setprecision(2);
    for (size_t memoryPageCount = 4; memoryPageCount <= Memory::VirtualPageCount; memoryPageCount++)
    {
        cout << setw(5) << memoryPageCount << "  ";
        test(memoryPageCount, accessSequence, make_unique<FIFO>());
        test(memoryPageCount, accessSequence, make_unique<LRU>());
        test(memoryPageCount, accessSequence, make_unique<Optimal>(pageAccessSequence));
        test(memoryPageCount, accessSequence, make_unique<NRU>(memoryPageCount));
        cout << endl;
    }

    return 0;
}
