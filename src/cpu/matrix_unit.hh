#ifndef __CPU_MATRIX_UNIT_HH__
#define __CPU_MATRIX_UNIT_HH__

/****************************************************
 * ! TensorCore Extension.
 * ! Implement the matrix unit.
 */

#include <list>

#include "base/types.hh"

class ExecContext;

class MatrixUnit {
public:
  static constexpr int MATRIX_SIZE = 4;
  static constexpr int MATRIX_LEN = MATRIX_SIZE * MATRIX_SIZE;
  using MATRIX_ELEMENT_T = float;
  using MATRIX_T = struct { MATRIX_ELEMENT_T elements[MATRIX_LEN]; };

  void load(ExecContext *xc, Addr addr);
  void multiply();
  uint64_t store(ExecContext *xc, Addr addr);

private:
  std::list<MATRIX_T> loadQueue;
  MATRIX_T accumulateMatrix;
};

#endif
