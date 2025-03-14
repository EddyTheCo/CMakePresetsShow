#include "amarula/foo/foo.hpp"
#undef NDEBUG
#include <assert.h>

int main(int argc, char *argv[]) {
  assert(Amarula::Foo::fooFunction() == "id-1234-5678");
  return 0;
}
