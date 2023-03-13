#ifndef _RUNTIME_ERROR_STUB_H_
#define _RUNTIME_ERROR_STUB_H_

// Catch2 is a C++ test framework, to link a C library you need these tags
#ifdef __cplusplus
extern "C" {
#endif

void RuntimeErrorStub_Reset(void);

const char* RuntimeErrorStub_GetLastError(void);

int RuntimeErrorStub_GetLastParameter(void);

// Catch2 is a C++ test framework, to link a C library you need these tags
#ifdef __cplusplus
}
#endif

#endif