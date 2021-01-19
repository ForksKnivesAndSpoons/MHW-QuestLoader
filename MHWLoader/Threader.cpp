#include "Threader.h"

Threader::~Threader() {
  for (auto& thread : threads) {
    thread.join();
  }
}
