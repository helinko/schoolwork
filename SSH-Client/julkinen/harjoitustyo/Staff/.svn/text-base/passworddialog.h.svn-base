#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

namespace Staff {
    class PasswordDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit PasswordDialog(QString username, QWidget *parent = 0);
        ~PasswordDialog();
        void getPass(char * chars, int max);
    private:
        Ui::PasswordDialog *ui;
    };
}

#endif // PASSWORDDIALOG_H
