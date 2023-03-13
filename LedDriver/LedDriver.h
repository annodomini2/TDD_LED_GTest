// TEST

#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include "stdint.h"
#include "stdbool.h"

// Catch2 is a C++ test framework, to link a C library you need these tags
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

int LedDriver_Init(uint16_t* Address, bool InvertOutput, bool InvertInput);

int LedDriver_TurnOn(int16_t LedIndex);

int LedDriver_TurnOff(int16_t LedIndex);

int LedDriver_TurnOnAll(void);

int LedDriver_TurnOffAll(void);

bool LedDriver_IsOn(int16_t LedIndex);

bool LedDriver_IsOff(int16_t LedIndex);

// Catch2 is a C++ test framework, to link a C library you need these tags
#ifdef __cplusplus
}
#endif

#endif