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
}

void Panahon::showResultList(QDomNodeList nodeList)
{
    if(nodeList.isEmpty()) return;

    for (uint i = 0; i < nodeList.length(); ++i) {
        const QDomNode node = nodeList.item(i);
        QDomNode child = node.firstChild();
        while (!child.isNull()) {
            const QString tag = child.toElement().tagName();
            const QString data = child.toElement().attribute("data");
            if (tag == "condition")
                conditionData->setText(data);
            if (tag == "temp_f") {
                QString s = QString("Temperature: %1%2").arg(data).arg("(F)");
                fahrenheit->setText(s);
            }
            if (tag == "temp_c") {
                QString s = QString("%1%2").arg(data).arg("(C)");
                centigrade->setText(s);
            }
            if (tag == "humidity")
                humidity->setText(data);
            if (tag == "wind_condition")
                windCondition->setText(data);
            if (tag == "icon") {
                QString url = QString("http://www.google.com%1").arg(data);
                requestIcon->get(QNetworkRequest(QUrl(url)));
            }
            child = child.nextSibling();
        }
    }

    const QString s = searchLineEdit->text();
    QString url = QString("http://maps.google.com/maps/api/staticmap?center=%1&sensor=false&mapType=roadMap&size=511x161").arg(s);
    mapProgressBar->show();
    gmap->loadMap(url);
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
