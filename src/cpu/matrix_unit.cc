#include "matrix_unit.hh"

#include "exec_context.hh"

#include <sstream>

void MatrixUnit::load(ExecContext *xc, Addr addr) {
  inform("Load matrix at 0x%x.\n", addr);
  MATRIX_T matrix;
  auto &proxy = xc->tcBase()->getVirtProxy();
  proxy.readBlob(addr, (void *)&(matrix.elements[0]),
                 sizeof(MATRIX_ELEMENT_T) * MATRIX_LEN);
  std::stringstream ss;
  for (int i = 0; i < MATRIX_LEN; ++i) {
    ss << matrix.elements[i] << ' ';
  }
  inform("Read in matrix (%d): %s.\n", this->loadQueue.size(),
         ss.str().c_str());
  this->loadQueue.push_back(matrix);
}

void MatrixUnit::multiply() {
  inform("Matrix Multiply.\n");
  assert(this->loadQueue.size() >= 2 &&
         "Must be at least two matrix to be multiplied.");
  auto A = this->loadQueue.front();
  this->loadQueue.pop_front();
  auto B = this->loadQueue.front();
  this->loadQueue.pop_front();
  for (int i = 0; i < MATRIX_SIZE; ++i) {
    for (int j = 0; j < MATRIX_SIZE; ++j) {
      MATRIX_ELEMENT_T sum = 0;
      for (int k = 0; k < MATRIX_SIZE; ++k) {
        sum +=
            A.elements[i * MATRIX_SIZE + k] * B.elements[k * MATRIX_SIZE + j];
      }
      this->accumulateMatrix.elements[i * MATRIX_SIZE + j] += sum;
    }
  }
}

uint64_t MatrixUnit::store(ExecContext *xc, Addr addr) {
  inform("Store matrix to 0x%x.\n", addr);
  auto &proxy = xc->tcBase()->getVirtProxy();
  proxy.writeBlob(addr, (void *)&(this->accumulateMatrix.elements[0]),
                  sizeof(MATRIX_ELEMENT_T) * MATRIX_LEN);

  /**
   * A hack to return the first 64-bit we wrote.
   */
  uint64_t ret = *reinterpret_cast<uint64_t *>(this->accumulateMatrix.elements);

  std::stringstream ss;
  for (int i = 0; i < MATRIX_LEN; ++i) {
    ss << this->accumulateMatrix.elements[i] << ' ';
    // Clear the accumulate matrix.
    this->accumulateMatrix.elements[i] = 0;
  }
  inform("Store in matrix: %s.\n", ss.str().c_str());

  return ret;
}