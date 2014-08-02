#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#define VERSION "1.0.1"

#include <QDialog>
#include <QDebug>
#include <QFile>
#include "ui_about.h"

class AboutWidget : public QDialog
{
    Q_OBJECT
public:
    explicit AboutWidget(QWidget *parent = 0);

private:
    Ui::Dialog *ui;

};

#endif // ABOUTWIDGET_H
