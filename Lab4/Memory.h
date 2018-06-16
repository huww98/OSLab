#if !defined(MEMORY_H)
#define MEMORY_H

#include <memory>
#include <array>
#include "ReplaceAlgorithm.h"
class Memory
{
  public:
    static constexpr size_t PageSize = 10;
    static constexpr size_t VirtualPageCount = 32;
    static constexpr size_t AddressSpace = VirtualPageCount * PageSize;
    static constexpr size_t GetPageIndex(size_t index)
    {
        return index / PageSize;
    }

    Memory(size_t memoryPageCount, std::unique_ptr<ReplaceAlgorithm> &&);
    void Access(size_t index);
    double HitRate()
    {
        return (double)hitCount / accessCount;
    }

  private:
    size_t memoryPageCount;
    std::array<bool, VirtualPageCount> pageLoaded;
    size_t accessCount = 0;
    size_t hitCount = 0;
    size_t usedMemoryPageCount = 0;

    std::unique_ptr<ReplaceAlgorithm> replaceAlgorithm;
};

#endif // MEMORY_H
