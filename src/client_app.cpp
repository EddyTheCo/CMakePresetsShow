#include <cstdio>

#include "amarula/foo/foo.hpp"

#include <QCoreApplication>
#include <QTextStream>

auto main(int argc, char *argv[]) -> int {

  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("ClientApp");

  Amarula::Foo::QParser parser;
  parser.setApplicationDescription("Client App to connect to server");
  parser.process(app);

  if (parser.getCode()) {
    const auto identifier = Amarula::Foo::fooFunction();
    QTextStream out(stdout);
    out << QString::fromStdString(identifier) << "\n";
    return 0;
  }
  return app.exec();
}
