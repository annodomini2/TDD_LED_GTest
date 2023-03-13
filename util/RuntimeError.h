#ifndef _RUNTIME_ERROR_H_
#define _RUNTIME_ERROR_H_

extern void RuntimeError(const char * m, int p, const char * f, int l);

#define RUNTIME_ERROR(description, parameter) RuntimeError(description, parameter, __FILE__, __LINE__)

#endif