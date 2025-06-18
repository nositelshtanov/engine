#pragma once

#include "IProcess.h"

class Editor3D;

IProcess* CreateProc(PrIds id, IProcess* parent, Editor3D&);
