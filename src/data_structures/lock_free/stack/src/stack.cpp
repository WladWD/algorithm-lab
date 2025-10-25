#include "data_structures/lock_free/stack/stack.h"

#if defined(STACK_SMOKE_TEST)
#include <iostream>

int main() {
  using data_structures::lock_free::stack::LockFreeStack;
  LockFreeStack<int> st;

  const int N = 10000;
  for (int i = 0; i < N; ++i) st.push(i);

  int cnt = 0;
  while (true) {
    auto v = st.pop();
    if (!v) break;
    ++cnt;
  }

  std::cout << "smoke: pushed=" << N << " popped=" << cnt << "\n";
  return (cnt == N) ? 0 : 2;
}
#endif
