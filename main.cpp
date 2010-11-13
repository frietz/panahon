#include <QApplication>
#include <QtGui>

#include "main_panahon.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(app.translate("main","Pana-panahon"));
    app.setOrganizationName("****");
    app.setOrganizationDomain("Pasig");    

    app.setQuitOnLastWindowClosed(false);

    MainPanahon *mainPanahon = new MainPanahon;
    mainPanahon->show();

    return app.exec();
}
