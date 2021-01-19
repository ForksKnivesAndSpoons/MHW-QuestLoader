#pragma once

#include <thread>
#include <vector>

class Threader {
 public:
  template <typename Function, typename... Args>
  void startThread(Function&& func, Args&&... args) {
    threads.emplace_back(std::forward<Function>(func),
                         std::forward<Args>(args)...);
  }

  Threader() = default;
  Threader(const Threader&) = delete;
  Threader& operator=(const Threader&) = delete;
  Threader(Threader&&) = delete;
  Threader& operator=(const Threader&&) = delete;

  ~Threader();

 private:
  std::vector<std::thread> threads;
};
