#include "ProcessCreator.h"

#include <array>
#include <functional>
#include <memory>

#include "PrCreateEdge.h"
#include "PrPickPoint.h"
#include "PrCreatePoint.h"
#include "PrCreateEdge.h"

namespace 
{
// мб стоит сделать базовый класс PrCreationParams с enumом наследников, чтобы принимать 
// его в фабричные функции. А эти функции бы кастили к нужным параметрам. 
// Но это на случай, если процессы будут иметь разные конструкторы  

std::shared_ptr<IProcess> CreateMainProc(PrIds id, IProcess* parent, Editor3D& editor)
{
    return nullptr;
}

std::shared_ptr<IProcess> CreatePrPickPoint(PrIds id, IProcess* parent, Editor3D& editor)
{
    return std::make_shared<PrPickPoint>(id, parent, editor);
}

std::shared_ptr<IProcess> CreatePrCreatePoint(PrIds id, IProcess* parent, Editor3D& editor)
{
    return std::make_shared<PrCreatePoint>(id, parent, editor);
}

std::shared_ptr<IProcess> CreatePrCreateEdge(PrIds id, IProcess* parent, Editor3D& editor) {
   return std::make_shared<PrCreateEdge>(id, parent, editor);
}

std::array<std::function<std::shared_ptr<IProcess>(PrIds, IProcess*, Editor3D&)>, static_cast<size_t>(PrIds::count)> PrIdsToCreateFuncs =
{
    CreateMainProc,
    CreatePrPickPoint,
    CreatePrCreatePoint,
    CreatePrCreateEdge
};

}

std::shared_ptr<IProcess> CreateProc(PrIds id, IProcess* parent, Editor3D& editor)
{
    return PrIdsToCreateFuncs[static_cast<size_t>(id)](id, parent, editor);
}