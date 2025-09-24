/**
 * @file   machine_state.c
 * @author Greg Anderson
 * @date   2025-07-22
 * @brief  Global state for the OVM.
 */

#include "stack.h"
#include "types.h"

/// @brief The main value stack of the OVM.
stack* st;
/// @brief The current environment of the OVM.
object* current_env;
