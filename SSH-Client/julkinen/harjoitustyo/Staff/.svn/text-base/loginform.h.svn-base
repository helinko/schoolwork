#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <memory>
#include <QWidget>
#include <DirectoryInterface.h>
#include "ConnectionInterface.h"
#include "sshconnection.h"
namespace Ui {
class LoginForm;
}

namespace Staff {
    class LoginForm : public QWidget
    {
        Q_OBJECT

    public:
        explicit LoginForm(QWidget *parent = 0);
        ~LoginForm();
        bool connected() const;
        bool connect();
        void disconnect();
        QString serverName() const;
        std::shared_ptr<FSInterface::ConnectionInterface> directory() const;

    private slots:
        void on_connectButton_clicked();

    private:
        Ui::LoginForm *ui;
        std::unique_ptr<Staff::SshConnection> connection;
    };
}

#endif // LOGINFORM_H
