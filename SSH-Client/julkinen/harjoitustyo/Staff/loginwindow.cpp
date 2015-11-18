#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "virtualconnection.h"
using std::shared_ptr;

namespace Staff
{
    LoginWindow::LoginWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::LoginWindow),
        connected(false)
    {
        ui->setupUi(this);
    }

    LoginWindow::~LoginWindow()
    {
        qDeleteAll(logins);
        delete ui;
    }
    bool LoginWindow::hasConnection() const
    {
        return connected;
    }

    bool LoginWindow::connectAll()
    {
        foreach (LoginForm * form, logins)
        {
            if (!form->connect())
                return false;
        }
        return true;
    }

    QList<std::shared_ptr<FSInterface::ConnectionInterface>> LoginWindow::connections() const
    {
        QList<std::shared_ptr<FSInterface::ConnectionInterface>> result;
        if (ui->debugMode->isChecked())
        {
            result.append(empty());
            result.append(hello());
        }
        else
            foreach (LoginForm * form, logins)
            {
                result.append(form->directory());
            }
        return result;
    }

    void LoginWindow::on_conNum_valueChanged(int count)
    {
        while (logins.size() > count)
        {
            LoginForm * form = logins.last();
            logins.removeLast();
            ui->loginFormLayout->removeWidget(form);
            delete form;
        }
        while (logins.size() < count)
        {
            LoginForm * form = new LoginForm(this);
            ui->loginFormLayout->addWidget(form);
            logins.append(form);
        }
    }

    void LoginWindow::on_buttonBox_accepted()
    {
        if (this->ui->debugMode->isChecked() || connectAll())
        {
            connected = true;
            close();
        }
    }
}


