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
    QAction *helpAboutQtAction;

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
