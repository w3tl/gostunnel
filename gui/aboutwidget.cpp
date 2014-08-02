#include "aboutwidget.h"

AboutWidget::AboutWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QString fName = QApplication::applicationDirPath() + "/license";
    fName += QLocale::system().name() == "ru_RU" ? "_ru.txt" : ".txt";
    QFile file(fName);
    file.open(QIODevice::ReadOnly);
    qDebug() << fName;
    ui->textBrowser->setText(QString::fromLocal8Bit(file.readAll()));
    file.close();
//    ui->label_5->setText(QLocale::system() == QLocale::Russia ? "yes" : "no");
    ui->label->setText(ui->label->text() + tr("версия ") + VERSION);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(hide()));
    ui->label_3->setOpenExternalLinks(true);
}
