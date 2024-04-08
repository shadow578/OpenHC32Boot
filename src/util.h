#pragma once

#define _CONCAT(a, b) a##b
#define CONCAT(a, b) _CONCAT(a, b)

#define minimum(a,b) ((a)<(b)?(a):(b))

#ifdef __cplusplus
  template<typename T, int N>
  constexpr int countof(T(&)[N]) { return N; }
#endif

#define STRINGIFY(x) #x
