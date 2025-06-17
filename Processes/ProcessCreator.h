#pragma once

#include "IProcess.h"

class ProcessManager;

IProcess* CreateProc(PrIds id, IProcess* parent, ProcessManager& prManager);
