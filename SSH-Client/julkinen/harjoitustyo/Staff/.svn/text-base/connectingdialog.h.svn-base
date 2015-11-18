#ifndef CONNECTINGDIALOG_H
#define CONNECTINGDIALOG_H

#include <memory>
#include <QDialog>
#include "sshconnection.h"

namespace Ui {
class ConnectingDialog;
}

namespace Staff
{
    class ConnectingDialog : public QDialog, public PasswordProvider
    {
        Q_OBJECT

    public:
        explicit ConnectingDialog(QString user, QString host,  int port, QWidget *parent = 0);
        ~ConnectingDialog();
        virtual bool getPassword(char * pass, int maxlength);
        bool connectToHost();
        std::unique_ptr<SshConnection> result();

    private slots:
        void error(QString error);
        void connected();
    private:
        std::unique_ptr<SshConnection> connection;
        QString user;
        Ui::ConnectingDialog *ui;
        bool successed;

    };
}
#endif // CONNECTINGDIALOG_H
