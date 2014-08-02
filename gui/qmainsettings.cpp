#include "qmainsettings.h"

QString QMainSettings::appName = "GOSTunnel-gui";

QMainSettings::QMainSettings(QString path, QString app, QWidget *parent) :
    settingsPath(path), appPath(app), QDialog(parent)
{
    this->setWindowTitle("Настройки программы gostunnel-gui");
    this->resize(400, 100);
    setWindowFlags(Qt::Tool);

    QSettings st(settingsPath, QSettings::IniFormat);

    vLayout = new QVBoxLayout(this);
    hLayoutProgram = new QHBoxLayout(this);
    hLayoutConfig = new QHBoxLayout(this);
    vLayout->addLayout(hLayoutProgram);
    vLayout->addLayout(hLayoutConfig);

    lblProgram = new QLabel(tr("Путь к программе:"), this);
    leProgram = new QLineEdit(st.value("Files/program").toString(), this);
    btnProgram  = new QToolButton(this);
    btnProgram->setText("...");
    connect(btnProgram, SIGNAL(clicked()),
            this, SLOT(btn_changeProgramPath_click()));
    hLayoutProgram->addWidget(lblProgram);
    hLayoutProgram->addWidget(leProgram);
    hLayoutProgram->addWidget(btnProgram);

    lblConfig = new QLabel(tr("Путь к настройкам:"), this);
    leConfig = new QLineEdit(st.value("Files/config").toString(), this);
    btnConfig = new QToolButton(this);
    btnConfig->setText("...");
    connect(btnConfig, SIGNAL(clicked()),
            this, SLOT(btn_changeConfigPath_click()));
    hLayoutConfig->addWidget(lblConfig);
    hLayoutConfig->addWidget(leConfig);
    hLayoutConfig->addWidget(btnConfig);

    cbAutoRun = new QCheckBox("Запускать при старте системы", this);
    cbAutoRun->setChecked(st.value("Files/autostart").toBool());
    vLayout->addWidget(cbAutoRun);

    btnSave = new QPushButton(tr("Сохранить"), this);
    vLayout->addWidget(btnSave);
    connect(btnSave, SIGNAL(clicked()), this, SLOT(btn_save_click()));

}

QMainSettings::~QMainSettings()
{
    delete vLayout;
}

void QMainSettings::btn_changeProgramPath_click()
{
    QString dir = QDir::rootPath();
    if (!leProgram->text().isEmpty()) {
        if (QDir(QFileInfo(leProgram->text()).dir()).exists()) {
            dir = QFileInfo(leProgram->text()).dir().path();
        }
    }
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Выберите исполнимый файл программы gostunnel"),
                dir, tr("Exe Files (*.exe)"));
    if (!path.isEmpty()) {
        leProgram->setText(path);
    }
}

void QMainSettings::btn_changeConfigPath_click()
{
    QString dir = QDir::rootPath();
    if (!leConfig->text().isEmpty()) {
        if (QDir(QFileInfo(leConfig->text()).dir()).exists()) {
            dir = QFileInfo(leConfig->text()).dir().path();
        }
    }
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Выберите файл настроек программы gostunnel"),
                dir, "");
    if (!path.isEmpty()) {
        leConfig->setText(path);
    }
}

void QMainSettings::btn_save_click()
{
    if (!QFile(leProgram->text()).exists()) {
        QMessageBox::warning(this,
                             tr("Не найден файл программы"),
                             tr("Не найден файл:\n") + leProgram->text());
        return;
    }
    if (leConfig->text().isEmpty()) {
        QMessageBox::warning(this,
                             tr("Не найден файл"),
                             tr("Не введен путь к файлу настроек"));
        return;
    } else {
        if (!QFile(leConfig->text()).exists()) {
            int res = QMessageBox::warning(
                        this,
                        tr("Не найден файл"),
                        tr("Файл ") + leConfig->text() + " не найден.\n"
                        "Создать файл с таким именем?",
                        QMessageBox::Yes, QMessageBox::No);
            if (res == QMessageBox::Yes) {
                QFile file(leConfig->text());
                file.open(QIODevice::WriteOnly);
                file.close();
            }
        }
    }
    QSettings st(settingsPath, QSettings::IniFormat);
    st.setValue("Files/program", leProgram->text());
    st.setValue("Files/config", leConfig->text());
    st.setValue("Files/autostart", cbAutoRun->checkState());
    if (cbAutoRun->checkState()) {
        QSettings settings(REG, QSettings::NativeFormat);
        QString value = appPath;
        QString apostroph = "\"";
        value.replace("/","\\");
        value = apostroph + value + apostroph;
        settings.setValue(appName, value);
    } else {
        QSettings settings(REG, QSettings::NativeFormat);
        settings.remove(appName);
    }
    QMessageBox::information(
                this,
                tr("Сохранение настроек"),
                tr("Новые настройки будут применены после перезапуска пиложения"));
    close();
}
