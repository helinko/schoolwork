#include "connectingdialog.h"
#include "ui_connectingdialog.h"
#include "passworddialog.h"
#include <QMessageBox>
using namespace Staff;
ConnectingDialog::ConnectingDialog(QString user, QString host, int port, QWidget *parent)
    : QDialog(parent),connection(new SshConnection(user, host, port, 3, this)), user(user),
      ui(new Ui::ConnectingDialog())
{
    ui->setupUi(this);
    ui->label->setText("Connecting to "+host);
    this->setWindowTitle("Connecting to "+host);
    connect(connection.get(), SIGNAL(connected()), this, SLOT(connected()));
    connect(connection.get(), SIGNAL(failed(QString)), this, SLOT(error(QString)));
}

ConnectingDialog::~ConnectingDialog()
{
    delete ui;
}

bool ConnectingDialog::getPassword(char *pass, int maxlength)
{
    PasswordDialog dialog(user);
    if (dialog.exec())
    {
        dialog.getPass(pass, maxlength);
        return true;
    }
    else
        return false;
}

bool ConnectingDialog::connectToHost()
{
    show();
    connection->connectToServer();
    return successed;
}

std::unique_ptr<SshConnection> ConnectingDialog::result()
{
    return std::move(connection);
}

void ConnectingDialog::error(QString error)
{
    QMessageBox box(this);
    box.setText(error);
    box.exec();
    successed = false;
    close();
}

void ConnectingDialog::connected()
{
    successed = true;
    close();
}
