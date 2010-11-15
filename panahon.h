#ifndef PANAHON_H
#define PANAHON_H

#include "ui_panahon.h"
#include <QUrl>
#include <QDomDocument>
#include <QFile>
#include <QCompleter>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class QDomDocument;
class QUrl;

class Panahon : public QWidget, public Ui::Panahon
{
    Q_OBJECT

public:
    Panahon(QWidget *parent = 0);    

private slots:
    void toggleSearch(const QString &text);
    void search();

    void getLocation(QNetworkReply *reply);
    void getIcon(QNetworkReply *reply);

private:    
    void showMap();

    QNetworkAccessManager *requestLocation;
    QNetworkAccessManager *requestIcon;

    void initWidgets();
    void getCurrentCondition(QDomNodeList nodeList);
    void getForecast(QDomNodeList nodeList);
    void parseDom(QDomDocument &dom);

    QMap<int, QMap<QString, QString> > mapForecast;

};

#endif // PANAHON_H
