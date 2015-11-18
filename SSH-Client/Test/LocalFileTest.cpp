#include <QString>
#include <QtTest>
#include <QDebug>
#include <memory>

#include "LocalFile.h"
#include "VirtualDirectory.h"
#include "VirtualFile.h"

class LocalFileTest : public QObject
{
    Q_OBJECT

public:
    LocalFileTest();

private Q_SLOTS:

    // FileInterface interface
    void OpenRead();
    void OpenWrite();
    void OpenClose();

    // DirectoryEntryInterface interface
    void NamePathSize();
    void RenameRemove();


};

LocalFileTest::LocalFileTest()
{
}

void
LocalFileTest::OpenRead(){

}

void
LocalFileTest::OpenWrite(){

}

void
LocalFileTest::OpenClose(){

}

void
LocalFileTest::NamePathSize(){

}

void
LocalFileTest::RenameRemove(){

}

QTEST_MAIN(LocalFileTest)
#include "LocalFileTest.moc"
