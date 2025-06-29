#pragma once

#include <memory>

#include "IProcess.h"

class Editor3D;

std::shared_ptr<IProcess> CreateProc(PrIds id, IProcess* parent, Editor3D&);
