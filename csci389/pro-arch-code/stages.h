#pragma once

#include <stdlib.h>

#include "base.h"

uint32_t fetch(state*);
instruction decode(state*, uint32_t);
result execute(state*, instruction);
result memory(state*, result);
void writeback(state*, result);

void run_program(state*, size_t);
