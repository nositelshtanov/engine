#pragma once

#include "ProcessBase.h"

class PrMain : public ProcessBase
{
public:
    PrMain()
       : ProcessBase(static_cast<PrId>(PrIds::Main), nullptr) 
    {}
};