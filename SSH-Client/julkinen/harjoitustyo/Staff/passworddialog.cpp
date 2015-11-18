#include "passworddialog.h"
#include "ui_passworddialog.h"
#include "string.h"

using namespace Staff;
PasswordDialog::PasswordDialog(QString user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
    ui->label->setText("Password for "+user);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::getPass(char *chars, int max)
{
    QString text = ui->lineEdit->text();
    QByteArray out = text.toUtf8();

    strncpy(chars, out.data(), max);

    // Clear both
    memset(out.data(), 0, out.size());
    for (QChar* c = text.data(); *c != 0; c++)
        *c = 0;
}
