#include <QtGui>
#include "google_map.h"

GoogleMap::GoogleMap(QWidget *parent)
    : QWebView(parent)
{
}

void GoogleMap::loadMap(const QString &args)
{
    if (args.isEmpty()) return;
    QString url = QString("http://maps.google.com/maps/api/staticmap?center=%1&sensor=false&mapType=roadMap&size=511x161").arg(args);
    load(QUrl(url));
    show();
}
