#include <QtGui>
#include "google_map.h"

GoogleMap::GoogleMap(QWidget *parent)
    : QWebView(parent)
{
}

void GoogleMap::loadMap(const QString &url)
{
    if (url.isEmpty()) return;
    load(QUrl(url));
    show();
}
