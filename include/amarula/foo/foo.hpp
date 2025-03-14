#pragma once
#include <QCommandLineParser>
namespace Amarula::Foo {
std::string fooFunction(void);

class QParser : public QCommandLineParser {
	QCommandLineOption m_getO;
    public:
	using QCommandLineParser::QCommandLineParser;
	QParser():m_getO{QCommandLineOption(QStringList() << "gc" << "getCode",
		    QCoreApplication::translate("main", "get client id"))}
	{
		addHelpOption();
		addOption(m_getO);
	}
	bool getCode()const{
		return isSet(m_getO);
	}

};
class BParser {
  uint16_t port_;
    public:
	BParser();
	uint16_t getPort()const{
		return port_;
	}

};
} // namespace Amarula::Foo
