#include "loginform.h"
#include "ui_loginform.h"
#include <QMessageBox>
#include "connectingdialog.h"
#include "sftpdirectory.h"
#include <libssh2_sftp.h>
using namespace Staff;

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    ui->userName->setText(name);
}

LoginForm::~LoginForm()
{
    delete ui;
}

QString LoginForm::serverName() const
{
    return ui->hostName->text();
}

std::shared_ptr<FSInterface::ConnectionInterface> LoginForm::directory() const
{
    QString path;
    if (ui->directory->text().isEmpty())
    {
        QByteArray dir;
        // Read the default dir
        int size = 256;
        dir.resize(size);
        int res = LIBSSH2_ERROR_EAGAIN;
        while (res == LIBSSH2_ERROR_EAGAIN)
        {
            res = libssh2_sftp_realpath(connection->connection(), ".", dir.data(), size);
            if (res == LIBSSH2_ERROR_BUFFER_TOO_SMALL)
            {
                size = size<<2;
                dir.resize(size);
                res = LIBSSH2_ERROR_EAGAIN;
            }
        }
        if (res < 0)
        {
            // An error. Use "/"
            dir = "/";
        }
        else
        {
            dir.resize(res+1);
        }
        path = QString::fromUtf8(dir.data());
    }
    else
    {
        // given dir
        path = ui->directory->text();
    }
    return std::shared_ptr<FSInterface::ConnectionInterface>(new SftpConnection(ui->hostName->text(), path, connection->connection()));
}

bool LoginForm::connected() const
{
    return connection != nullptr;
}

bool LoginForm::connect()
{
    if (connected())
        return true;

    QString missing;
    if (ui->userName->text().isEmpty())
        missing = "name ";
    if (ui->hostName->text().isEmpty())
        missing += "host ";
    if (ui->port->value() == 0)
        missing += "port ";
    if (!missing.isEmpty())
    {
        QMessageBox box(this);
        box.setText("Following fields are missing: "+missing);
        box.exec();
        return false;
    }
    ConnectingDialog dialog(ui->userName->text(), ui->hostName->text(), ui->port->value(), this);
    bool isconnected;
    if ((isconnected = dialog.connectToHost()))
    {
        connection = dialog.result();
        ui->userName->setEnabled(false);
        ui->hostName->setEnabled(false);
        ui->port->setEnabled(false);
        ui->directory->setEnabled(false);
        ui->connectButton->setText("Disconnect");
    }
    return isconnected;
}

void LoginForm::disconnect()
{
    connection.release();

    ui->userName->setEnabled(true);
    ui->hostName->setEnabled(true);
    ui->port->setEnabled(true);
    ui->directory->setEnabled(true);
    ui->connectButton->setText("Connect");
}

void LoginForm::on_connectButton_clicked()
{
    if (connected())
        disconnect();
    else
        connect();
}
