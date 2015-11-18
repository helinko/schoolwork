#include "virtualconnection.h"
#include <VirtualFile.h>
#include <VirtualDirectory.h>

using namespace FSInterface;
using namespace std;

class VirtualConnection : public ConnectionInterface
{
public:
    virtual ~VirtualConnection() = default;
    virtual QString server() const { return name; }
    virtual QString path() const { return ""; }
    virtual shared_ptr<DirectoryInterface> directory()
    {
        return shared_ptr<DirectoryInterface>(new VirtualDirectory(*dir));
    }
    QString name;
    std::unique_ptr<VirtualDirectory> dir;
};

namespace Staff {
    std::unique_ptr<ConnectionInterface> empty()
    {
        VirtualConnection *  connection = new VirtualConnection;
        connection->name = "empty";
        connection->dir.reset(new VirtualDirectory("empty", "", true, connection));
        return unique_ptr<ConnectionInterface>(connection);
    }
    std::unique_ptr<ConnectionInterface> hello()
    {
        VirtualConnection *  connection = new VirtualConnection;
        connection->name = "hello";
        connection->dir.reset(new VirtualDirectory("hello", "", true, connection));
        shared_ptr<FileInterface> file = connection->dir->mkfile("hello");
        file->open(FileInterface::WRITE);
        file->write(QString("Hello, world!\n").toUtf8());
        file->close();
        shared_ptr<DirectoryInterface> dir = connection->dir->mkdir("Directory");
        file = dir->mkfile("subfile");
        file->open(FileInterface::WRITE);
        file->write(QString("This is a subfile\n").toUtf8());
        file->close();
        file = dir->mkfile("bigfile");
        file->open(FileInterface::WRITE);
        for (int i=1; i <= 2048; i++)
            file->write(QString("Hello %1\n").arg(i).toUtf8());
        file->close();
        dir->mkdir("Empty subdirectory");
        return unique_ptr<ConnectionInterface>(connection);
    }
}
