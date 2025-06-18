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

IProcess* CreateMainProc(PrIds id, IProcess* parent, Editor3D& editor)
{
    return nullptr;
}

IProcess* CreatePrPickPoint(PrIds id, IProcess* parent, Editor3D& editor)
{
    return new PrPickPoint(id, parent, editor);
}

IProcess* CreatePrCreatePoint(PrIds id, IProcess* parent, Editor3D& editor)
{
    return new PrCreatePoint(id, parent, editor);
}

std::array<std::function<IProcess*(PrIds, IProcess*, Editor3D&)>, static_cast<size_t>(PrIds::count)> PrIdsToCreateFuncs =
{
    CreateMainProc,
    CreatePrPickPoint,
    CreatePrCreatePoint
};

}

IProcess* CreateProc(PrIds id, IProcess* parent, Editor3D& editor)
{
    return PrIdsToCreateFuncs[static_cast<size_t>(id)](id, parent, editor);
}