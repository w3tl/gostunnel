#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    {
        QSharedMemory shmem("GOSTunnel-gui");
        shmem.attach();
    }

    shmem = new QSharedMemory("GOSTunnel-gui");
    if (shmem->attach()) {
        QMessageBox::warning(this,
                             tr("Ошибка открытия приложения"),
                             tr("Приложение уже запущенно!"));
        exit(1);
    } else {
        shmem->create(1);
    }

    ui->setupUi(this);

    isShowMsgTray = false;
    mainSettings = NULL;
    setWindowTitle("GOSTunnel-gui");
    setWindowFlags(Qt::WindowMinimizeButtonHint |
                   Qt::WindowCloseButtonHint |
                   Qt::MSWindowsFixedSizeDialogHint);
    loadSettings(QCoreApplication::applicationDirPath() + "/config.txt");
    loadConfigs(programConfig);
    this->setTrayIconActions();
    this->showTrayIcon();

    ui->lwGroups->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lwGroups, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenuForGroupsList(QPoint)));
    QAction *renameAction = new QAction(tr("Переименовать"), this);
    QAction *deleteAction = new QAction(tr("Удалить"), this);
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(on_btnDeletegroup_clicked()));
    connect(renameAction, SIGNAL(triggered()),
            this, SLOT(triggeredRenameAction()));
    lwMenu = new QMenu(tr("Context menu"), this);
    lwMenu->addAction(renameAction);
    lwMenu->addAction(deleteAction);
}

MainWindow::~MainWindow()
{
    //sema->release();
    delete shmem;
    //delete sema;

    delete lwMenu;
    delete aboutAction;
    delete showAction;
    delete settingsAction;
    delete quitAction;
    delete trayIconMenu;
    delete trayIcon;
    delete mainSettings;
    delete ui;
}

void MainWindow::loadSettings(QString path)
{
    if (!QFile(path).exists()) {
        QMessageBox::warning(this, tr("Ошибка открытия файла"),
                             tr("Не найден файл настроек GOSTunnel GUI:\n") +
                             path);
        return;
    }
    QSettings st(path, QSettings::IniFormat);
    programConfig = QDir::toNativeSeparators(st.value("Files/config").toString());
    programPath = QDir::toNativeSeparators(st.value("Files/program").toString());
    if (!QFile(programConfig).exists() ||
        !QFile(programPath).exists()) {
        QMessageBox::warning(this, tr("Не найдены файлы"),
                             tr("Не удалось найти конфигурационный файл программы "
                                "GOSTunnel, либо файл программы gostunnel."));
    }
}

void MainWindow::loadConfigs(QString path)
{
    if (!QFile(path).exists()) {
        QMessageBox::warning(this, tr("Ошибка открытия файла"),
                             tr("Не найден файл настроек gostunnel:\n") + path);
        return;
    }
    ui->lwGroups->clear();
    connections.clear();

    QStringList groups;
    QSettings st(path, QSettings::IniFormat);
    groups << st.childGroups();

    for (int i = 0; i < groups.count(); i++) {
        QProcess *proc = new QProcess(this);
        proc->setObjectName(groups.at(i));
        connections.insert(groups.at(i), proc);
        QListWidgetItem *item = new QListWidgetItem(groups.at(i),
                                                    ui->lwGroups);
    }
    if (!groups.isEmpty()) {
        ui->lwGroups->item(0)->setSelected(true);
        loadConfig(groups.first());
    }
}

void MainWindow::loadConfig(QString group)
{
    QSettings st(programConfig, QSettings::IniFormat);
    ui->leRemoteHost->setText(st.value(group + "/remote-host").toString());
    ui->leRemotePort->setText(st.value(group + "/remote-port").toString());
    ui->leLocalHost->setText(st.value(group + "/local-host").toString());
    ui->leLocalPort->setText(st.value(group + "/local-port").toString());;
    QString logPath = st.value(group + "/log-file").toString();
    ui->leCommonName->setText(st.value(group + "/cn").toString());
    ui->leLogFile->setText(logPath);
    ui->cbLogLevel->setCurrentIndex(st.value(group + "/log-level").toInt() - 1);
    if (connections.value(group)->pid() <= 0) { // is proc off
        ui->lblConnState->setText(ui->lblConnState->statusTip());
    } else {
        ui->lblConnState->setText(ui->lblConnState->whatsThis());
    }
}

void MainWindow::saveConfig(QString section)
{
    if (!QFile(programConfig).exists()) {
        int res = QMessageBox::warning(
                    this, tr("Не найдены файлы"),
                     tr("Не удалось найти конфигурационный файл программы, "
                     "либо файл программы gostunnel.\n"
                     "Показать окно настроек для выбора файлов?"),
                     QMessageBox::No, QMessageBox::Yes);
        if (res == QMessageBox::Yes) {
            showMainSettings();
        }
    }
    if (connections.value(section)->isOpen()) {
        QMessageBox::information(this, tr("Подключение активно"),
                                 tr("Настройки вступят в силу после остановки "
                                    "соединения"));
    }
    QSettings st(programConfig, QSettings::IniFormat);
    st.setIniCodec("UTF-8");
    st.beginGroup(section);
    st.setValue("remote-host", ui->leRemoteHost->text());
    st.setValue("remote-port", ui->leRemotePort->text());
    st.setValue("local-host", ui->leLocalHost->text());
    st.setValue("local-port", ui->leLocalPort->text());
    st.setValue("cn", ui->leCommonName->text());
    st.setValue("log-file", ui->leLogFile->text());
    st.setValue("log-level", ui->cbLogLevel->currentIndex() + 1);
    st.endGroup();
}

void MainWindow::deleteConfig(QString section)
{
    QSettings st(programConfig, QSettings::IniFormat);
    st.remove(section);
}

QString MainWindow::searchFreeNumber(QList<QString> sl, QString word)
{
    int number = 0;
    QStringList::iterator elem = sl.begin();
    while (elem != sl.end()) {
        QStringRef l = elem->leftRef(word.size());
        if (l == word) {
         QStringRef r = elem->rightRef(elem->size() - word.size());
         number = r.toInt();
        }
        elem++;
    }
    return QString::number(number + 1);
}

void MainWindow::on_lwGroups_clicked(const QModelIndex &index)
{
    QListWidgetItem *item = ui->lwGroups->currentItem();
    item->setIcon(QIcon(""));
    loadConfig(item->text());
}

void MainWindow::on_btnSave_clicked()
{
    if (ui->lwGroups->count() == 0) {
        return;
    }
    saveConfig(ui->lwGroups->currentItem()->text());
}

void MainWindow::on_btnChangeLogFile_clicked()
{
    QString dir = QCoreApplication::applicationDirPath();
    if (!ui->leLogFile->text().isEmpty()) {
        if (QFile(ui->leLogFile->text()).exists()) {
            dir = QFileInfo(ui->leLogFile->text()).dir().path();
        }
    }
    QString path = QFileDialog::getSaveFileName(
        this,
        tr("Создайте файл для записи лога"),
        dir,
        tr(" Файлы лога (*.log);;Все файлы (*.*)"));
    if (!path.isEmpty()) {
        ui->leLogFile->setText(path);
        QFile file(path);
        if (!file.exists()) {
            file.open(QIODevice::WriteOnly);
            file.setPermissions(QFileDevice::WriteOther);
            file.close();
        }
    }
}

void MainWindow::on_btnAddGroup_clicked()
{
    QString name = "Connection" + searchFreeNumber(connections.keys(),
                                                   QString("Connection"));
    QString label = tr("Введите название нового подключения");
    bool ok = false;
    QList<QListWidgetItem *> list = ui->lwGroups->findItems(name, Qt::MatchWrap);
    while (true) {
        name = QInputDialog::getText(
                    this,
                    tr("Ввод названия нового подключения"),
                    label, QLineEdit::Normal, name, &ok);
        list = ui->lwGroups->findItems(name, Qt::MatchWrap);
        if (ok && list.count() > 0) {
            label = tr("Подключение с таким названием уже существует!\n"
                       "Введите другое название");
            name = "Connection" + searchFreeNumber(connections.keys(),
                                                   QString("Connection"));
        } else if (ok) {
            break;
        } else {
            return;
        }
    }
    QProcess *proc = new QProcess(this);
    proc->setObjectName(name);
    connections.insert(name, proc);
    QListWidgetItem *item = new QListWidgetItem(name);
    ui->lwGroups->addItem(item);
    ui->lwGroups->setCurrentItem(item);
    loadConfig(name);
}

void MainWindow::on_btnDeletegroup_clicked()
{
    QListWidgetItem *item = ui->lwGroups->currentItem();
    if (connections.value(ui->lwGroups->currentItem()->text())->isOpen()) {
        QMessageBox::information(this, tr("Невозможно удалить подключение"),
                                 tr("Подключение активно, остановите подключение "
                                    "для его удаления"));
        return;
    }
    connections.remove(item->text());
    deleteConfig(item->text());
    int row = ui->lwGroups->currentRow();
    delete item;
    ui->lwGroups->setCurrentRow(row == 0 ? row : row - 1);
    loadConfig(ui->lwGroups->currentItem()->text());
}

void MainWindow::setTrayIconActions()
{
    aboutAction = new QAction(tr("О Программе..."), this);
    showAction = new QAction(tr("Показать"), this);
    settingsAction = new QAction(tr("Настройки"), this);
    quitAction = new QAction(tr("Выход"), this);

    connect (settingsAction, SIGNAL(triggered()),
             this, SLOT(showMainSettings()));
    connect(showAction, SIGNAL(triggered()), this, SLOT(trayActionShow()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addAction(settingsAction);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(quitAction);
}

void MainWindow::showTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/img/images/gostunnel.png"));
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}

void MainWindow::trayActionShow()
{
    if (mainSettings == NULL || !mainSettings->isVisible()) {
        showNormal();
        setFocus();
    }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::DoubleClick:
            trayActionShow();
            break;
        default:
            break;
    }
}

void MainWindow::freeMsgIndicator()
{
    isShowMsgTray = false;
}

void MainWindow::showTrayIconMessage()
{
    if (isShowMsgTray) {
        QTimer::singleShot(TRAY_MESSAGE_TIME, this, SLOT(showTrayIconMessage()));
        return;
    }
    QString title = msgStack.first();
    msgStack.removeFirst();
    QString msg = msgStack.first();
    msgStack.removeFirst();
    trayIcon->showMessage(title,
                          msg,
                          QSystemTrayIcon::Warning,
                          TRAY_MESSAGE_TIME);
    isShowMsgTray = true;
    QTimer::singleShot(TRAY_MESSAGE_TIME, this, SLOT(freeMsgIndicator()));
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
            this->hide();
        }
    }
}

void MainWindow::showMainSettings()
{
    if (mainSettings == NULL) {
        mainSettings = new QMainSettings(
                QCoreApplication::applicationDirPath() + "/config.txt",
                QCoreApplication::applicationFilePath());
        mainSettings->setModal(true);
    }
    mainSettings->show();
}

void MainWindow::on_btnOpenLogFile_clicked()
{
    QString logPath = ui->leLogFile->text();
    if (!QFile(logPath).exists()) {
        if (QFile(programPath).exists()) {
            QDir programDir = QDir(programPath);
            programDir.cdUp();
            logPath = programDir.path();
        } else {
            logPath = QApplication::applicationDirPath();
        }
        logPath += "/logs/" + ui->lwGroups->currentItem()->text() + ".log";
        qDebug() << logPath;
    }
    qDebug() << logPath;
    if (QFile(logPath).exists()) {
        QDesktopServices::openUrl(QUrl("file:///" + logPath));
    } else {
        QMessageBox::warning(this, tr("Файл не найден"),
                             tr("Файл по адресу ") + ui->leLogFile->text() +
                             tr(" не нйден"));
    }
}

void MainWindow::on_lwGroups_doubleClicked(const QModelIndex &index)
{
    if (connections.value(index.data().toString())->pid() > 0) {

        QMessageBox::information(this,
                                 tr("Невозможно изменить имя подключения"),
                                 tr("В настоящее время подключение активно, "
                                    "остановите подключение для изменения настроек"));
    } else {
        QString oldName = index.data().toString();
        QString newName = oldName;
        QString label = tr("Введите новое название подключения:");
        bool ok = false;
        QList<QListWidgetItem *> list;
        while (true) {
            newName = QInputDialog::getText(
                        this,
                        tr("Изменение названия подключения"),
                        label, QLineEdit::Normal, newName, &ok);
            list = ui->lwGroups->findItems(newName, Qt::MatchWrap);
            if (ok && list.count() > 0) {
                if (newName == oldName) {
                    return;
                }
                label = tr("Подключение с таким названием уже существует!\n"
                           "Введите другое название:");
                newName = oldName;
            } else if (newName.contains(QRegExp("\\w*[\\.\\s\\,]+\\w*"))) {
                label = tr("Некорректное название подключение!\n"
                           "Строка содержит неподдерживаемые символы\n"
                           "Введите другое название:");
                newName = oldName;
            } else if (ok) {
                break;
            } else {
                return;
            }
        }
        ui->lwGroups->currentItem()->setText(newName);
        QProcess *proc = connections.value(oldName);
        proc->setObjectName(newName);
        connections.insert(newName, proc);
        connections.remove(oldName);
        deleteConfig(oldName);
        saveConfig(newName);
    }
}


void MainWindow::on_btnStart_clicked()
{
    if (!QFile(programPath).exists()) {
        QMessageBox::warning(this,
                             tr("Невозможно запустить процесс"),
                             tr("Не найден файл программы.\n"
                                "Возможно, не указан путь к файлу gostunnel.exe"));
        return;
    }
    if (ui->lwGroups->count() == 0) {
        return;
    }
    QString el = ui->lwGroups->currentItem()->text();
    QProcess *proc = connections.value(el);
    if (proc->pid() <= 0) {
        QDir wd = QDir(programPath);
        wd.cdUp();
        proc->setWorkingDirectory(wd.path());
        QStringList args;
        args << "--config" << programConfig
             << "-s" << el;
        connect(proc, SIGNAL(error(QProcess::ProcessError)),
                this, SLOT(processError(QProcess::ProcessError)));
        connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)),
                this, SLOT(processFinished(int,QProcess::ExitStatus)));
        connect(proc, SIGNAL(stateChanged(QProcess::ProcessState)),
                this, SLOT(processStateChenged(QProcess::ProcessState)));
        connect(proc, SIGNAL(readyReadStandardError()),
                this, SLOT(processReadyReadError()));
        connect(proc, SIGNAL(started()),
                this, SLOT(processStarted()));
        ui->lwGroups->currentItem()->setBackgroundColor(QColor::fromRgb(255, 233, 113));
        proc->start(programPath, args);
    }
}

void MainWindow::on_btnStop_clicked()
{
    if (ui->lwGroups->count() == 0) {
        return;
    }
    QString el = ui->lwGroups->currentItem()->text();
    QProcess *proc = connections.value(el);
    if (proc->pid() > 0) {
        ui->lwGroups->currentItem()->setBackgroundColor(QColor::fromRgb(255, 233, 113));
        proc->close();
    }
}

void MainWindow::processError(QProcess::ProcessError error)
{
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QProcess *p = (QProcess *)sender();

    msgStack.push_back(tr("Подключение остановлено"));
    msgStack.push_back(tr("Подключение ") + p->objectName() + tr(" было остановлено"));
    showTrayIconMessage();

    QList<QListWidgetItem *> list = ui->lwGroups->findItems(p->objectName(),
                                                            Qt::MatchWrap);
    if (list.at(0) != ui->lwGroups->currentItem()) {
        list.at(0)->setIcon(QIcon(":/img/images/error.png"));
    }
}

void MainWindow::processStateChenged(QProcess::ProcessState newState)
{
    QProcess *p = (QProcess *)sender();
    QList<QListWidgetItem *> list = ui->lwGroups->findItems(p->objectName(),
                                                            Qt::MatchWrap);
    switch (newState) {
        case QProcess::Running :
            if (list.at(0) == ui->lwGroups->currentItem()) {
                ui->lblConnState->setText(ui->lblConnState->whatsThis());
                ui->lblConnState->setToolTip(tr("Подключение активно"));
            }
            list.at(0)->setBackgroundColor(QColor::fromRgb(113, 255, 129));
            trayIcon->showMessage(
                        tr("Подключение запущено"),
                        tr("Подключение ") + p->objectName() + tr(" было запущено"),
                        QSystemTrayIcon::Information,
                        TRAY_MESSAGE_TIME);

            break;
        case QProcess::NotRunning :
            if (list.at(0) == ui->lwGroups->currentItem()) {
                ui->lblConnState->setText(ui->lblConnState->statusTip());
                ui->lblConnState->setToolTip(tr("Подключение остановлено"));
            }
            list.at(0)->setBackgroundColor(Qt::white);
    }
}

void MainWindow::processStarted()
{

}

void MainWindow::processReadyReadError()
{
    QProcess *p = (QProcess *)sender();
    QByteArray buf = p->readAllStandardError();

    qDebug() << QString::fromLocal8Bit(buf);
    qDebug() << buf.length();
    if (buf.size() == 0) {
        return;
    }
    QFile f("log");
    f.open(QIODevice::Append);
    f.write(buf, buf.size());
    f.close();

    msgStack.push_back(tr("Ошибка"));
    msgStack.push_back(QString::fromLocal8Bit(buf));
    showTrayIconMessage();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool isAllStoped = true;
    QHash<QString, QProcess *>::iterator elem = connections.begin();
    while (elem != connections.end()) {
        if (elem.value()->pid() > 0) {
            isAllStoped = false;
            break;
        }
        elem++;
    }
    if (isAllStoped) {
        event->accept();
    } else {
        int ret = QMessageBox::warning(
                    this, tr("Подтвердите выход"),
                    tr("Остались запущенные подключения, остановите все "
                       "подключения и выйти?"),
                    QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::No) {
            event->ignore();
        } else {
            elem = connections.begin();
            while (elem != connections.end()) {
                if (elem.value()->pid() > 0) {
                    elem.value()->close();
                    delete elem.value();
                }
                elem++;
            }
        }
    }
}

void MainWindow::on_btnRestart_clicked()
{
    on_btnStop_clicked();
    on_btnStart_clicked();
}

void MainWindow::showContextMenuForGroupsList(const QPoint &point)
{
    lwMenu->exec(mapToGlobal(point));
}

void MainWindow::triggeredRenameAction()
{
    on_lwGroups_doubleClicked(ui->lwGroups->currentIndex());
}

void MainWindow::showAbout()
{
    AboutWidget *aw = new AboutWidget(this);
    aw->setWindowTitle(tr("О Программе"));
    aw->setModal(true);
    aw->show();
}
