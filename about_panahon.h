#ifndef ABOUT_PANAHON_H
#define ABOUT_PANAHON_H

#include <QDialog>
#include <QtGui>

#include "ui_about_panahon.h"

class AboutPanahon : public QDialog, public Ui::AboutPanahon
{
    Q_OBJECT

public:
    explicit AboutPanahon(QWidget *parent = 0);

private:
};

#endif // ABOUT_PANAHON_H
