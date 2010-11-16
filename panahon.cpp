/*
 * Panahon is a simple weather application
 * Copyright (C) 2010 Frietz Osorio
 *
 * This file is part of Panahon.
 *
 * Panahon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Panahon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Panahon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui>
#include "panahon.h"
#include "google_map.h"

Panahon::Panahon(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);    

    searchPushButton->setEnabled(false);    

    requestLocation = new QNetworkAccessManager(this);
    requestIcon = new QNetworkAccessManager(this);

    connect(requestLocation, SIGNAL(finished(QNetworkReply*)),this, SLOT(getLocation(QNetworkReply*)));
    connect(requestIcon, SIGNAL(finished(QNetworkReply*)), SLOT(getIcon(QNetworkReply*)));
    connect(gmap, SIGNAL(loadProgress(int)), mapProgressBar, SLOT(setValue(int)));
    connect(searchLineEdit, SIGNAL(textChanged(QString)),SLOT(toggleSearch(QString)));
    connect(searchLineEdit, SIGNAL(returnPressed()),SLOT(search()));
    connect(searchPushButton, SIGNAL(clicked()), SLOT(search()));

    initWidgets();
}

void Panahon::initWidgets()
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

void Panahon::toggleSearch(const QString &text)
{    
    searchPushButton->setEnabled(!text.isEmpty());
}

void Panahon::search()
{
    QString s = searchLineEdit->text();
    QString url = QString("http://www.google.com/ig/api?weather=%1").arg(s);
    requestLocation->get(QNetworkRequest(QUrl(url)));
}

void Panahon::parseDom(QDomDocument &dom)
{
    QDomElement root = dom.documentElement();
    getCurrentCondition(root.elementsByTagName("current_conditions"));
    getForecast(root.elementsByTagName("forecast_conditions"));
}

void Panahon::getCurrentCondition(QDomNodeList nodeList)
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

    showMap();
}

void Panahon::showMap()
{
    mapProgressBar->show();
    gmap->loadMap(searchLineEdit->text());
}

void Panahon::getForecast(QDomNodeList nodeList)
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

void Panahon::getLocation(QNetworkReply *reply)
{
    QDomDocument doc("doc");
    if(!doc.setContent(reply))
        return;

    QDomElement root = doc.documentElement();
    if (root.tagName() != "xml_api_reply")
        return;

    parseDom(doc);
}
