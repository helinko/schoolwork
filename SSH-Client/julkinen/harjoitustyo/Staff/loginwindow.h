#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <memory>
#include <QDialog>
#include <QList>
#include <QAbstractButton>
#include "loginform.h"

namespace Ui {
class LoginWindow;
}

namespace Staff {
class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    QList<std::shared_ptr<FSInterface::ConnectionInterface>> connections() const;
    bool hasConnection() const;
private slots:
    void on_conNum_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    bool connectAll();
    Ui::LoginWindow *ui;
    QList<LoginForm*> logins;
    bool connected;
};

}

#endif // LOGINWINDOW_H
