#pragma once
#include <array>
#include <algorithm>

template <typename T, int N>
constexpr bool contains(const T (&arr)[N], const T &element)
{
  // cppcheck-suppress useStlAlgorithm -- std::any_of is only constexpr in C++20 and later
  for (auto &other : arr)
  {
    if (other == element) 
    {
      return true;
    }  
  }

  return false;
}

template <typename T, int N, int M>
constexpr bool containsAny(const T (&arr1)[N], const T (&arr2)[M])
{
  // cppcheck-suppress useStlAlgorithm -- std::any_of is only constexpr in C++20 and later
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
constexpr bool has_duplicates(const T (&arr)[N])
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
