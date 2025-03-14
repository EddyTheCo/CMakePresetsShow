#include <iostream>

#include "amarula/foo/foo.hpp"

int main(int argc, char *argv[]) {
  const auto bparser= Amarula::Foo::BParser();
  std::cout << "port:" << bparser.getPort() << "\n";
  return 0;
}
