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
    void toggle_search(const QString &text);
    void search();

    void parseSearchLocation(QNetworkReply *reply);
    void parseWeatherDetails(QNetworkReply *reply);
    void getIcon(QNetworkReply *reply);

private:    
    QNetworkAccessManager *searchLocation;
    QNetworkAccessManager *requestWeatherDetails;    
    QNetworkAccessManager *requestIcon;

    void init_fields();
    void showResultList(QDomNodeList nodeList);
    void parseDom(QDomDocument &dom);

};

#endif // PANAHON_H
