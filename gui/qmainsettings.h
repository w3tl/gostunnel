#ifndef QMAINSETTINGS_H
#define QMAINSETTINGS_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QEvent>
#include <QCloseEvent>
#include <QCheckBox>

#define REG "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"

class QMainSettings : public QDialog
{
    Q_OBJECT
public:
    explicit QMainSettings(QString path, QString app, QWidget *parent = 0);
    ~QMainSettings();

public slots:

private slots:
    void btn_changeProgramPath_click();
    void btn_changeConfigPath_click();

    void btn_save_click();

private:
    static QString appName;
    QString settingsPath;
    QString appPath;

    QVBoxLayout *vLayout;
    QHBoxLayout *hLayoutConfig;
    QHBoxLayout *hLayoutProgram;

    QLabel *lblConfig;
    QLabel *lblProgram;

    QToolButton *btnConfig;
    QToolButton *btnProgram;
    QPushButton *btnSave;

    QLineEdit *leConfig;
    QLineEdit *leProgram;

    QCheckBox *cbAutoRun;

};

#endif // QMAINSETTINGS_H
