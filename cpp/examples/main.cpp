#include <stdint.h>
#include <iostream>
#include <type_traits> // std::is_enum
#include <navigation/nav.hpp>

using namespace std;


enum Err: int {
  OK = 0,
  BAD = 1,
  REALBAD = 2
};

template<typename T, typename E>
struct ret_t {
  static_assert(std::is_enum<E>::value,"E needs to be an enum");
  T value;
  E error;
  inline explicit operator bool() const noexcept { return error==0 ? true: false; }
};

using retf_t = ret_t<float, Err>;

retf_t test(int val=0) {
  retf_t ret;
  if (val == 0) ret.value = 1.2345f;
  else if (val == 1) ret.error = Err::BAD;
  else ret.error = Err::REALBAD;
  return ret;
}

int main() {
  retf_t ret = test(2);

  if (ret) cout << ret.value << " " << ret.error << endl;
  else cout << "wtf: " << ret.error << endl;

  return 0;
}
