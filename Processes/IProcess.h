#pragma once

class IProcess
{
public:
    using PrId = size_t;
    
    virtual PrId GetPrId() const = 0;
    virtual bool Run() = 0;
    virtual bool Stop() = 0;
}