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

#include "main_panahon.h"
#include "about_panahon.h"
#include "ui_panahon.h"

MainPanahon::MainPanahon(QWidget *parent)
    : QMainWindow(parent)
{
    mainWidget = new Panahon(this);

    createActions();
    setupMenuBar();
    createStatusBar();
    setupTrayIcon();

    connect(panahonTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(minRestorePanahon(QSystemTrayIcon::ActivationReason)));

    panahonTrayIcon->show();


    setCentralWidget(mainWidget);

    setWindowTitle(tr(":: Panahon"));
    setWindowIcon(QIcon(":/images/panahon.png"));
    setMinimumSize(600,590);
    setMaximumSize(600,590);
    resize(600,590);

    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

void MainPanahon::createActions()
{
    panahonQuitAction = new QAction(tr("&Quit"), this);
    panahonQuitAction->setIcon(QIcon(":/images/exit.png"));
    panahonQuitAction->setShortcut(QKeySequence::Quit);
    panahonQuitAction->setStatusTip(tr("Close this window"));
    connect(panahonQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    helpAboutPanahonAction = new QAction(tr("&About Panahon..."),this);
    helpAboutPanahonAction->setStatusTip(tr("About Panahon"));
    connect(helpAboutPanahonAction, SIGNAL(triggered()), this, SLOT(aboutPanahon()));

}

void MainPanahon::setupMenuBar()
{
    panahonMenu = menuBar()->addMenu(tr("&Panahon"));
    panahonMenu->addAction(panahonQuitAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("Contents"));
    helpMenu->addAction(tr("Index"));
    helpMenu->addAction(tr("Context Help"));
    helpMenu->addSeparator();    
    helpMenu->addAction(helpAboutPanahonAction);    
}

void MainPanahon::createStatusBar()
{
    statusbarPanahon = new QStatusBar;
    statusbarPanahon->showMessage(tr("Ready"));
    setStatusBar(statusbarPanahon);
}

void MainPanahon::aboutPanahon()
{
    AboutPanahon *a = new AboutPanahon(this);
    a->exec();
}

void MainPanahon::closeEvent(QCloseEvent *event)
{
    if (panahonTrayIcon->isVisible()) {
        QMessageBox::information(this, tr("Panahon"),
                                 tr("Closing this program means it will continue to run in the background. "
                                    "To terminate the program, select <b>Quit</b> in the "
                                    "system tray entry context menu."));
        hide();
        event->ignore();
    }
}

void MainPanahon::setupTrayIcon()
{
    panahonTrayIconMenu = new QMenu(this);
    panahonTrayIconMenu->addAction(helpAboutPanahonAction);    
    panahonTrayIconMenu->addSeparator();
    panahonTrayIconMenu->addAction(panahonQuitAction);

    panahonTrayIcon = new QSystemTrayIcon(this);
    panahonTrayIcon->setToolTip(tr("Panahon ver: 1.0"));
    panahonTrayIcon->setIcon(QIcon(":/images/panahon.png"));
    panahonTrayIcon->setContextMenu(panahonTrayIconMenu);
}

void MainPanahon::minRestorePanahon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::Trigger:
        if(isMinimized() || !isVisible())
            this->showNormal();
        else if(isMaximized() || isVisible())
            this->hide();
        break;
    default:
        ;
    }
}

