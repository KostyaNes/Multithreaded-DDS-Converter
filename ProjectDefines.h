#pragma once

#include <assert.h>

#define assertm(exp, msg) assert(((void)msg, exp))
