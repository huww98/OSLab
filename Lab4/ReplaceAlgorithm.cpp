#include <algorithm>

#include "ReplaceAlgorithm.h"

using namespace std;

void FIFO::LoadNewPage(size_t pageIndex)
{
    replaceQueue.push(pageIndex);
}

size_t FIFO::ReplacePage()
{
    auto page = replaceQueue.front();
    replaceQueue.pop();
    return page;
}

void LRU::LoadNewPage(size_t pageIndex)
{
    lruList.push_back(pageIndex);
}

size_t LRU::ReplacePage()
{
    auto page = lruList.front();
    lruList.pop_front();
    return page;
}

void LRU::AccessPage(size_t pageIndex)
{
    auto it = find(lruList.begin(), lruList.end(), pageIndex);
    lruList.erase(it);
    lruList.push_back(pageIndex);
}

Optimal::Optimal(vector<size_t> &accessSequence) : accessSequence(&accessSequence)
{
}

void Optimal::LoadNewPage(size_t pageIndex)
{
    auto offset = findNextAccess(pageIndex);
    replaceQueue.push_back(AccessPrediction{pageIndex, offset});
}

size_t Optimal::ReplacePage()
{
    auto it = max_element(replaceQueue.begin(), replaceQueue.end(),
                          [](AccessPrediction &a, AccessPrediction &b) { return a.NextAccess < b.NextAccess; });
    auto page = it->PageIndex;
    replaceQueue.erase(it);
    return page;
}

void Optimal::AccessPage(size_t pageIndex)
{
    auto it = find_if(replaceQueue.begin(), replaceQueue.end(),
                      [pageIndex](AccessPrediction &acc) { return acc.PageIndex == pageIndex; });
    it->NextAccess = findNextAccess(pageIndex);
}

size_t Optimal::findNextAccess(size_t pageIndex)
{
    nextAccessIndex++;
    auto it = find(accessSequence->begin() + nextAccessIndex, accessSequence->end(), pageIndex);
    return (size_t)(it - accessSequence->begin());
}

void NRU::LoadNewPage(size_t pageIndex)
{
    referencedList.push_back(pageIndex);
    clearIfDesired();
}

size_t NRU::ReplacePage()
{
    if (!notReferencedList.empty())
    {
        return replaceRandom(notReferencedList);
    }
    return replaceRandom(referencedList);
}

void NRU::AccessPage(size_t pageIndex)
{
    auto it = find(notReferencedList.begin(), notReferencedList.end(), pageIndex);
    if (it != notReferencedList.end())
    {
        notReferencedList.erase(it);
        referencedList.push_back(pageIndex);
    }
    clearIfDesired();
}

void NRU::clearIfDesired()
{
    accessSinceLastClear++;
    if (accessSinceLastClear >= clearPeriod)
    {
        accessSinceLastClear = 0;
        notReferencedList.splice(notReferencedList.end(), referencedList);
    }
}

size_t NRU::replaceRandom(std::list<size_t> &list)
{
    uniform_int_distribution<size_t> dist(0, list.size() - 1);
    auto it = next(list.begin(), dist(randomEngine));
    auto page = *it;
    list.erase(it);
    return page;
}
