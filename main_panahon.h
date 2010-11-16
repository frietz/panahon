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

#ifndef MAIN_PANAHON_H
#define MAIN_PANAHON_H

#include <QDialog>
#include <QtGui>

#include <QSystemTrayIcon>
#include <QMenuBar>

#include "panahon.h"

class MainPanahon : public QMainWindow
{
    Q_OBJECT

public:
    MainPanahon(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void aboutPanahon();
    void minRestorePanahon(QSystemTrayIcon::ActivationReason reason);

private:
    Panahon *mainWidget;

    QAction *panahonQuitAction;
    QAction *helpAboutPanahonAction;    

    void createActions();
    void createStatusBar();

    void setupMenuBar();
    void setupTrayIcon();
    void setupLocationBox();

    QSystemTrayIcon *panahonTrayIcon;    
    QMenu *panahonTrayIconMenu;

    QMenu *panahonMenu;
    QMenu *helpMenu;
    QMenu *menu;

    QStatusBar *statusbarPanahon;

};

#endif // MAIN_PANAHON_H
