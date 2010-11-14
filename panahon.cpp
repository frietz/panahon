#include <QtGui>
#include "panahon.h"
#include "google_map.h"

Panahon::Panahon(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);    

    searchPushButton->setEnabled(false);    

    searchLocation = new QNetworkAccessManager(this);
    requestWeatherDetails = new QNetworkAccessManager(this);
    requestIcon = new QNetworkAccessManager(this);

    connect(searchLocation, SIGNAL(finished(QNetworkReply*)),this, SLOT(parseSearchLocation(QNetworkReply*)));
    connect(requestWeatherDetails, SIGNAL(finished(QNetworkReply*)),this, SLOT(parseWeatherDetails(QNetworkReply*)));
    connect(requestIcon, SIGNAL(finished(QNetworkReply*)), SLOT(getIcon(QNetworkReply*)));
    connect(gmap, SIGNAL(loadProgress(int)), mapProgressBar, SLOT(setValue(int)));
    connect(searchLineEdit, SIGNAL(textChanged(QString)),SLOT(toggle_search(QString)));
    connect(searchLineEdit, SIGNAL(returnPressed()),SLOT(search()));
    connect(searchPushButton, SIGNAL(clicked()), SLOT(search()));

    init_fields();
}

void Panahon::init_fields()
{
    conditionData->setText("");
    humidity->setText("");
    fahrenheit->setText("");    
    centigrade->setText("");
    windCondition->setText("");
    iconLabel->setText("");
    mapProgressBar->hide();

    QStringList headers;
    headers << "Day" << "High (F)" << "Low (F)" << "Condition";
    forecastTable->setColumnCount(4);
    forecastTable->setRowCount(4);
    forecastTable->setHorizontalHeaderLabels(headers);
    forecastTable->setWindowTitle("Forecast");
    forecastTable->horizontalHeader()->setStretchLastSection(true);
}

void Panahon::toggle_search(const QString &text)
{    
    searchPushButton->setEnabled(!text.isEmpty());
}

void Panahon::search()
{
    QString s = searchLineEdit->text();
    QString url = QString("http://www.google.com/ig/api?weather=%1").arg(s);
    searchLocation->get(QNetworkRequest(QUrl(url)));
}

void Panahon::parseDom(QDomDocument &dom)
{
    QDomElement root = dom.documentElement();
    showResultList(root.elementsByTagName("current_conditions"));
    forecast(root.elementsByTagName("forecast_conditions"));
}

void Panahon::showResultList(QDomNodeList nodeList)
{
    if(nodeList.isEmpty()) return;

    QMap<QString, QString> map;
    const QDomNode node = nodeList.item(0);
    QDomNode child = node.firstChild();

    while (!child.isNull()) {
        const QString tag = child.toElement().tagName();
        const QString data = child.toElement().attribute("data");

        if (tag == "condition" ||
            tag == "temp_f" ||
            tag == "temp_c" ||
            tag == "humidity" ||
            tag == "wind_condition" ||
            tag == "icon")
            map[tag] = data;

        child = child.nextSibling();
    }

    currentWeatherCondition(map);
    showMap();
}

void Panahon::showMap()
{
    mapProgressBar->show();
    gmap->loadMap(searchLineEdit->text());
}

void Panahon::currentWeatherCondition(const QMap<QString, QString> map)
{
    QMapIterator<QString, QString> i(map);
    while (i.hasNext()) {
        i.next();
        if (i.key() == "condition") conditionData->setText(i.value());
        if (i.key() == "temp_f") fahrenheit->setText(QString("Temperature: %1%2").arg(i.value()).arg("(F)"));
        if (i.key() == "temp_c") centigrade->setText(QString("%1%2").arg(i.value()).arg("(C)"));
        if (i.key() == "humidity") humidity->setText(i.value());
        if (i.key() == "wind_condition") windCondition->setText(i.value());
        if (i.key() == "icon") {
            QString url = QString("http://www.google.com%1").arg(i.value());
            requestIcon->get(QNetworkRequest(QUrl(url)));
        }
    }
}

void Panahon::forecast(QDomNodeList nodeList)
{
    if(nodeList.isEmpty()) return;

    for (uint i = 0; i < nodeList.length(); ++i) {
        const QDomNode node = nodeList.item(i);
        QDomNode child = node.firstChild();

        QMap<QString, QString> map;

        while (!child.isNull()) {
            const QString tag = child.toElement().tagName();
            const QString data = child.toElement().attribute("data");

            if (tag == "day_of_week" ||
                tag == "low"  ||
                tag == "high" ||
                tag == "icon" ||
                tag == "condition")

                map[tag] = data;

            child = child.nextSibling();
        }

        mapForecast[i] = map;
    }

    QMapIterator<int, QMap<QString, QString> > i(mapForecast);
    QTableWidgetItem *item;
    int column;
    while (i.hasNext()) {
        i.next();
        QMap<QString, QString> m = i.value();
        QMapIterator<QString, QString> j(m);
        while(j.hasNext()) {
            j.next();
            if (j.key() == "icon") continue;
            item = new QTableWidgetItem(j.value());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            item->setTextAlignment(Qt::AlignCenter);

            if (j.key() == "day_of_week") column = 0;
            if (j.key() == "high") column = 1;
            if (j.key() == "low") column = 2;
            if (j.key() == "condition") column = 3;

            forecastTable->setItem(i.key(),column,item);
        }
    }
}

void Panahon::getIcon(QNetworkReply *reply)
{
    QByteArray ba(reply->readAll());
    QPixmap pixmap = QPixmap(90, 70);
    if (pixmap.loadFromData(ba)) {
        iconLabel->setPixmap(pixmap);
        iconLabel->sizeHint();
        iconLabel->setWordWrap(true);
    }
}

void Panahon::parseSearchLocation(QNetworkReply *reply)
{
    QDomDocument doc("doc");
    if(!doc.setContent(reply))
        return;

    QDomElement root = doc.documentElement();
    if (root.tagName() != "xml_api_reply")
        return;

    parseDom(doc);
}

void Panahon::parseWeatherDetails(QNetworkReply *reply)
{
    QDomDocument doc("doc");
    if(!doc.setContent(reply))
        return;

    QDomElement root = doc.documentElement();
    if (root.tagName() != "weather")
        return;

}
