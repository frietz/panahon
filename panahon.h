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
