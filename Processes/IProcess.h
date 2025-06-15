#pragma once

#include <set>
#include <memory>

using PrId = size_t;

enum class PrIds : PrId
{
    Main = 0,
    count
};

class IProcess
{
public:
    enum ProcessFlags
    {
        fActive = 1,
        fRunning = 2
    };
    
    virtual PrId GetPrId() const = 0;

    virtual bool Run() = 0;
    virtual bool Stop() = 0;

    virtual IProcess* GetParent() = 0;
    virtual std::set<std::shared_ptr<IProcess>> GetChilds() = 0;

    virtual size_t GetFlags() const = 0;
    virtual bool GetFlagVal(ProcessFlags flag) = 0;

};