#include <assert.h>

#include "Memory.h"

using namespace std;

Memory::Memory(size_t memoryPageCount, std::unique_ptr<ReplaceAlgorithm> &&algo)
    : memoryPageCount(memoryPageCount), replaceAlgorithm(std::move(algo))
{
    for (auto &l : pageLoaded)
    {
        l = false;
    }
}

void Memory::Access(size_t index)
{
    accessCount++;
    size_t pageIndex = GetPageIndex(index);
    //clog << "accessing page " << pageIndex;
    if (pageLoaded[pageIndex])
    {
        //clog << " hit." << endl;
        hitCount++;
        replaceAlgorithm->AccessPage(pageIndex);
        return;
    }
    if (usedMemoryPageCount < memoryPageCount)
    {
        usedMemoryPageCount++;
    }
    else
    {
        auto pageToReplace = replaceAlgorithm->ReplacePage();
        assert(pageLoaded[pageToReplace]);
        pageLoaded[pageToReplace] = false;
        //clog << " page " << pageToReplace << " replaced.";
    }
    replaceAlgorithm->LoadNewPage(pageIndex);
    pageLoaded[pageIndex] = true;
    //clog << " loaded." << endl;
}
