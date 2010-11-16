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
