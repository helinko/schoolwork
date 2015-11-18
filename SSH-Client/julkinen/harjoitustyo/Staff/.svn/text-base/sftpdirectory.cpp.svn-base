#include "sftpdirectory.h"
#include "sftpfile.h"
#include "sftplink.h"
using namespace Staff;
using namespace FSInterface;

SftpDirectory::SftpDirectory(SftpConnection *session, char *name)
    : SftpFileEntry(session, name)
{
}

void SftpDirectory::remove()
{
    if (contents().size() != 2)
        throw DirNotEmpty();
    int res = libssh2_sftp_rmdir(session->sftp, namestr);
    handleError(res, "unlink");
}

char * SftpDirectory::childName(const QString &name) const
{
    if (name.contains('/'))
        throw UnknownError("Invalid filename");
    if (name == ".")
        return strdup(namestr);
    else if (name == "..")
    {
	    char * end = rindex(namestr, '/');
        if ((namestr[0] == 0) || (strcmp(namestr, "/") == 0) || (end == namestr) )
        {
            return strdup("/");
        }
        else
        {
            int length = end-namestr;
            char * newName = (char*)malloc(length+1);
            memcpy(newName,namestr, length);
            newName[length] = 0;
            return newName;
        }
    }
    else
    {
        int namelen = strlen(namestr);
        QByteArray nameArray = name.toUtf8();
        char * newName = (char*)malloc(namelen+nameArray.size()+2);
        strcpy(newName, namestr);
        newName[namelen] = '/';
        strcpy(&(newName[namelen+1]), nameArray.data());
        return newName;
    }
}

std::shared_ptr<DirectoryEntryInterface> SftpDirectory::entry(const QString &name) const
{
    char *newName = childName(name);
    std::shared_ptr<DirectoryEntryInterface> result = session->open(newName);
    free(newName);
    return result;
}

bool SftpDirectory::hasChild(const QString &name) const
{
    char * newName = childName(name);
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    bool res = !(libssh2_sftp_stat(session->sftp, newName, &attrs));
    free(newName);
    return res;
}

std::shared_ptr<DirectoryInterface> SftpDirectory::mkdir(const QString &name)
{
    if (hasChild(name))
        throw FileExists(name);
    char * child = childName(name);
    try {
        int res = libssh2_sftp_mkdir(session->sftp, child, 0700);
        handleError(res, "mkdir");
        std::shared_ptr<DirectoryInterface> result(new SftpDirectory(session, child));
        free(child);
        child = nullptr;
        return result;
    } catch (...)
    {
        if (child != nullptr)
            free(child);
        throw;
    }
}

std::shared_ptr<FileInterface> SftpDirectory::mkfile(const QString &name)
{
    if (hasChild(name))
        throw FileExists(name);
    char * child = childName(name);
    std::shared_ptr<FileInterface> result(new SftpFile(session, child));
    free(child);
    child = nullptr;
    return result;
}

std::shared_ptr<SymbolicLinkInterface> SftpDirectory::mksymlink(const QString &name, const QString &target)
{
    if (hasChild(name))
        throw FileExists(name);
    char * child = childName(name);
    try
    {
        int res = libssh2_sftp_symlink(session->sftp, child, target.toUtf8().data());
        handleError(res, "symlink");
        std::shared_ptr<SymbolicLinkInterface> result(new SftpLink(session, child));
        free(child);
        child = nullptr;
        return result;
    } catch (...)
    {
        if (child != nullptr)
            free(child);
        throw;
    }
}

QList<QString> SftpDirectory::contents() const
{
    QList<QString> result;
    LIBSSH2_SFTP_HANDLE * handle = libssh2_sftp_opendir(session->sftp, namestr);
    handleError(handle == 0, "opendir", true);
    int maxSize = 512;
    char * filename = (char*)malloc(maxSize);
    int length;
    LIBSSH2_SFTP_ATTRIBUTES attr;
    while ((length = libssh2_sftp_readdir(handle, filename, maxSize, &attr)) != 0)
    {
        if (length == LIBSSH2_ERROR_BUFFER_TOO_SMALL)
        {
            free(filename);
            maxSize = maxSize << 1;
            filename = (char*)malloc(maxSize);
        } else if (length == LIBSSH2_ERROR_EAGAIN)
        {
            // Try again
        } else if (length < 0)
        {
            free(filename);
            handleError(length, "readdir");
            throw UnknownError();
        } else
        {
            result.append(QString::fromUtf8(filename));
        }
    }
    free(filename);
    handleError(libssh2_sftp_closedir(handle), "closedir");
    return result;
}

