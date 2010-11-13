#ifndef GOOGLE_MAP_H
#define GOOGLE_MAP_H

#include <QWebView>
class GoogleMap : public QWebView
{
    Q_OBJECT

public:
    GoogleMap(QWidget *parent = 0);    
    void loadMap(const QString &url);
private:    

};

#endif // GOOGLE_MAP_H
