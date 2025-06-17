#include "ProcessCreator.h"

#include <array>
#include <functional>

#include "PrPickPoint.h"
#include "PrCreatePoint.h"

namespace 
{
// мб стоит сделать базовый класс PrCreationParams с enumом наследников, чтобы принимать 
// его в фабричные функции. А эти функции бы кастили к нужным параметрам. 
// Но это на случай, если процессы будут иметь разные конструкторы  

IProcess* CreateMainProc(PrIds id, IProcess* parent, ProcessManager& prManager)
{
    return nullptr;
}

IProcess* CreatePrPickPoint(PrIds id, IProcess* parent, ProcessManager& prManager)
{
    return new PrPickPoint(id, parent, prManager);
}

IProcess* CreatePrCreatePoint(PrIds id, IProcess* parent, ProcessManager& prManager)
{
    return new PrCreatePoint(id, parent, prManager);
}

std::array<std::function<IProcess*(PrIds, IProcess*, ProcessManager&)>, static_cast<size_t>(PrIds::count)> PrIdsToCreateFuncs =
{
    CreateMainProc,
    CreatePrPickPoint,
    CreatePrCreatePoint
};

}

IProcess* CreateProc(PrIds id, IProcess* parent, ProcessManager& prManager)
{
    return PrIdsToCreateFuncs[static_cast<size_t>(id)](id, parent, prManager);
}