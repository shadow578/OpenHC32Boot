#pragma once
#include <array>
#include <algorithm>

template <typename T, int N>
constexpr bool contains(T (&arr)[N], T &element)
{
  for (auto other : arr) 
  {
    if (other == element) 
    {
      return true;
    }  
  }

  return false;
}

template <typename T, int N, int M>
constexpr bool containsAny(T (&arr1)[N], T (&arr2)[M])
{
  for (auto &other : arr1) 
  {
    if (contains(arr2, other)) 
    {
      return true;
    }  
  }

  return false;
}

template <typename T, int N>
constexpr bool has_duplicates(T (&arr)[N])
{
  for (size_t i = 0; i < N; ++i) 
  {
    for (size_t j = i + 1; j < N; ++j) 
    {
      if (arr[i] == arr[j]) 
      {
        return true;
      }
    }
  }

  return false;
}
