#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "ConnectionInterface.h"
#include "DirectoryInterface.h"
#include "LocalDirectory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QList<std::shared_ptr<FSInterface::ConnectionInterface>> &connections, QWidget *parent = 0);
    ~MainWindow();

signals:


private slots:

    void naytaEtatiedostot();
    void naytaPaikallisetTiedostot();
    void meneEtakansioon(QListWidgetItem *alkio);
    void menePaikalliseenKansioon(QListWidgetItem *alkio);
    void kopioiEtakansioon();
    void kopioiPaikalliseenKansioon();
    void tulostaError(QString viesti);
    void tulostaFatalError(QString viesti);
    void poistaYhteys();

private:
    Ui::MainWindow *ui;

    QVector<std::shared_ptr<FSInterface::DirectoryInterface>> etakansiot;
    std::shared_ptr<LocalDirectory> paikallinen_kansio;
};

#endif // MAINWINDOW_H
