#if !defined(REPLACE_ALGORITHM_H)
#define REPLACE_ALGORITHM_H

#include <queue>
#include <list>
#include <random>

class ReplaceAlgorithm
{
  public:
    virtual void AccessPage(size_t pageIndex) {}
    virtual void LoadNewPage(size_t pageIndex) = 0;
    virtual size_t ReplacePage() = 0;
};

class FIFO : public ReplaceAlgorithm
{
  public:
    virtual void LoadNewPage(size_t pageIndex) override;
    virtual size_t ReplacePage() override;

  private:
    std::queue<size_t> replaceQueue;
};

class LRU : public ReplaceAlgorithm
{
  public:
    virtual void LoadNewPage(size_t pageIndex) override;
    virtual size_t ReplacePage() override;
    virtual void AccessPage(size_t pageIndex) override;

  private:
    std::list<size_t> lruList;
};

struct AccessPrediction
{
    size_t PageIndex;
    size_t NextAccess;
};

class Optimal : public ReplaceAlgorithm
{
  public:
    Optimal(std::vector<size_t> &accessSequence);
    virtual void LoadNewPage(size_t pageIndex) override;
    virtual size_t ReplacePage() override;
    virtual void AccessPage(size_t pageIndex) override;

  private:
    size_t nextAccessIndex = 0;
    std::vector<size_t> *accessSequence;
    std::list<AccessPrediction> replaceQueue;
    size_t findNextAccess(size_t pageIndex);
};

class NRU : public ReplaceAlgorithm
{
  public:
    NRU(size_t clearPeriod) : clearPeriod(clearPeriod), randomEngine(std::random_device()()) {}
    virtual void LoadNewPage(size_t pageIndex) override;
    virtual size_t ReplacePage() override;
    virtual void AccessPage(size_t pageIndex) override;

  private:
    std::list<size_t> notReferencedList;
    std::list<size_t> referencedList;
    size_t accessSinceLastClear = 0;
    size_t clearPeriod;
    void clearIfDesired();

    std::mt19937 randomEngine;
    size_t replaceRandom(std::list<size_t> &list);
};

#endif // REPLACE_ALGORITHM_H
