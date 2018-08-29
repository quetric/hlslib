/// @author    Johannes de Fine Licht (definelicht@inf.ethz.ch)
/// @date      June 2017
/// @copyright This software is copyrighted under the BSD 3-Clause License.

#pragma once

#include "hlslib/Resource.h"
#include <limits>

namespace hlslib {

namespace op {

#ifdef HLSLIB_OPERATOR_ADD_RESOURCE
#define HLSLIB_OPERATOR_ADD_RESOURCE_PRAGMA(var)                                 \
  HLSLIB_RESOURCE_PRAGMA(var, HLSLIB_OPERATOR_ADD_RESOURCE)
#else
#define HLSLIB_OPERATOR_ADD_RESOURCE_PRAGMA(var)
#endif

template <typename T>
struct Add {
  static T Apply(T const &a, T const &b) {
    #pragma HLS INLINE
    const T res = a + b;
    HLSLIB_OPERATOR_ADD_RESOURCE_PRAGMA(res);
    return res;
  }
  static constexpr T identity() { return 0; }
private:
  Add() = delete;
  ~Add() = delete;
};

#ifdef HLSLIB_OPERATOR_MULTIPLY_RESOURCE
#define HLSLIB_OPERATOR_MULTIPLY_RESOURCE_PRAGMA(var)                                 \
  HLSLIB_RESOURCE_PRAGMA(var, HLSLIB_OPERATOR_MULTIPLY_RESOURCE)
#else
#define HLSLIB_OPERATOR_MULTIPLY_RESOURCE_PRAGMA(var)
#endif

template <typename T>
struct Multiply {
  static T Apply(T const &a, T const &b) {
    #pragma HLS INLINE
    const T res = a * b;
    HLSLIB_OPERATOR_MULTIPLY_RESOURCE_PRAGMA(res);
    return res;
  }
  static constexpr T identity() { return 1; }
private:
  Multiply() = delete;
  ~Multiply() = delete;
};

template <typename T>
struct And {
  static T Apply(T const &a, T const &b) { return a && b; }
  static constexpr T identity() { return true; }
private:
  And() = delete;
  ~And() = delete;
};

template <typename T>
struct Min {
  static T Apply(T const &a, T const &b) { return (a < b) ? a : b; }
  static constexpr T identity() { return std::numeric_limits<T>::max(); }
private:
  Min() = delete;
  ~Min() = delete;
};

template <typename T>
struct Max {
  static T Apply(T const &a, T const &b) { return (a > b) ? a : b; }
  static constexpr T identity() { return std::numeric_limits<T>::min(); }
private:
  Max() = delete;
  ~Max() = delete;
};

// TODO: should this be decoupled from DataPack? It could rely on the index
//       operator and have T be the vector class.
template <class Operator, typename T, int width>
struct Wide {
  static T Apply(T const &a, T const &b) {
    #pragma HLS INLINE
    T res;
    for (int w = 0; w < width; ++w) {
      #pragma HLS UNROLL
      res[w] = Operator::Apply(a[w], b[w]);
    }
    return res;
  }
  static T identity() {
    #pragma HLS INLINE
    T result;
    for (int w = 0; w < width; ++w) {
      #pragma HLS UNROLL
      result[w] = Operator::identity();
    }
    return result;
  }
private:
  Wide() = delete;
  ~Wide() = delete;
};

} // End namespace op

} // End namespace hlslib
