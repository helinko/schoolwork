#ifndef LOCALFILE_H
#define LOCALFILE_H

#include <QObject>
#include <QDir>
#include <QString>
#include <memory>
#include "FileInterface.h"

class LocalFile : public QObject, public FSInterface::FileInterface
{
    Q_OBJECT
public:
    // Olion voi alustaa antamalla rakentajalle tiedostopolun, joka oletuksena on tyhjä merkkijono.
    explicit LocalFile(QString polku = QString(), QObject *parent = 0);

signals:

public slots:


    // FileInterface interface
public:
    virtual void open(Mode mode);
    virtual int read(QByteArray &array);
    virtual int write(const QByteArray &array);
    virtual void close();

    // DirectoryEntryInterface interface
    virtual QString name() const;
    virtual QString fullPath() const;
    virtual ~LocalFile();
    virtual long size() const;
    virtual int permissions() const;
    virtual void changePermissions(int newPermissions);
    virtual void remove();
    virtual void rename(QString newName);
    virtual FSInterface::DateTime lastModified() const;
    virtual FSInterface::ConnectionInterface *connection() const;

private:
    QString muodosta_polku(QString alkion_nimi) const;

    std::shared_ptr<QFile> tiedosto;
    QString path;
};

#endif // LOCALFILE_H
