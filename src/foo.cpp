#include "amarula/foo/foo.hpp"
#include <boost/program_options.hpp>
#include <fstream>

namespace Amarula::Foo {
std::string fooFunction(void) { return "id-1234-5678"; }
namespace po = boost::program_options;
BParser::BParser()
{
  po::options_description desc("Settings");
  desc.add_options()("port", po::value<uint16_t>(&port_), "port");
  po::variables_map vm;
  std::ifstream fstream("config.txt");
  vm = po::variables_map();
  po::store(po::parse_config_file(fstream, desc), vm);
  po::notify(vm);
}
}
