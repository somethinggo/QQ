#include <QtWidgets/QApplication>
#include <ElaApplication.h>
#include "QQDesktop.h"
// #include <vld.h>

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
	// QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#endif
	initSystem();
	QApplication a(argc, argv);
	ElaApplication::getInstance()->setWindowIcon(QIcon(""));
	ElaApplication::getInstance()->init();
	QQDesktop w;
	w.show();
	return a.exec();
}
