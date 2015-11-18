#include "sftpconnection.h"
#include "sftpdirectory.h"
#include "sftpfile.h"
#include "sftplink.h"
#include "sftpunknown.h"
using namespace FSInterface;
using namespace std;
using namespace Staff;

std::shared_ptr<DirectoryInterface> SftpConnection::directory()
{
    return shared_ptr<SftpDirectory>(new SftpDirectory(this, pathStr.toUtf8().data()));
}

unique_ptr<DirectoryEntryInterface> SftpConnection::open(char *newName)
{
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    int res = libssh2_sftp_lstat(this->sftp, newName, &attrs);
    std::unique_ptr<DirectoryEntryInterface> result;
    if (res)
        throw FileDoesNotExist(newName);
    else if (LIBSSH2_SFTP_S_ISREG(attrs.permissions))
        result.reset(new SftpFile(this, newName));
    else if (LIBSSH2_SFTP_S_ISDIR(attrs.permissions))
        result.reset(new SftpDirectory(this, newName));
    else if (LIBSSH2_SFTP_S_ISLNK(attrs.permissions))
        result.reset(new SftpLink(this, newName));
    else
        result.reset(new SftpUnknown(this, newName));
    return result;
}
