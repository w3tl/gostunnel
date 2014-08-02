#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainsettings.h"
#include "aboutwidget.h"
#include <QMainWindow>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QStringListModel>
#include <QListWidgetItem>
#include <QDir>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QProcess>
#include <QEvent>
#include <QCloseEvent>
#include <QTimer>
#include <QSystemSemaphore>
#include <QSharedMemory>

#define TRAY_MESSAGE_TIME 3000

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void changeEvent(QEvent*);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionShow();
    void setTrayIconActions();
    void showTrayIcon();

    void showTrayIconMessage();
    void freeMsgIndicator();

    void showAbout();

    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError error);
    void processStarted();
    void processReadyReadError();
    void processStateChenged(QProcess::ProcessState newState);

    void showMainSettings();

    void showContextMenuForGroupsList(const QPoint& point);
    void triggeredRenameAction();

    void on_btnSave_clicked();
    void on_btnChangeLogFile_clicked();
    void on_btnAddGroup_clicked();
    void on_btnDeletegroup_clicked();
    void on_btnOpenLogFile_clicked();

    void on_lwGroups_clicked(const QModelIndex &index);
    void on_lwGroups_doubleClicked(const QModelIndex &index);

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnRestart_clicked();

private:
    QSystemSemaphore *sema;
    QSharedMemory *shmem;


    Ui::MainWindow *ui;

    QString configPath;

    QString programConfig;
    QString programPath;

    QHash<QString, QProcess*> connections;

    void loadSettings(QString path);
    void loadConfigs(QString path);

    void loadConfig(QString group);
    void saveConfig(QString section);
    void deleteConfig(QString section);

    QStringList msgStack;
    bool isShowMsgTray;

    QString searchFreeNumber(QList<QString> sl, QString word);

    QMenu *lwMenu;

    QMenu *trayIconMenu;
    QAction *aboutAction;
    QAction *showAction;
    QAction *settingsAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;

    QMainSettings *mainSettings;
};

#endif // MAINWINDOW_H
