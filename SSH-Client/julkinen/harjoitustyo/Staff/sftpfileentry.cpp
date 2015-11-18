#include "sftpfileentry.h"
#include <libgen.h>
#include <time.h>

using namespace Staff;
using namespace FSInterface;

SftpFileEntry::SftpFileEntry(SftpConnection *session, char *name):
    session(session), namestr(strdup(name))
{
}

QString SftpFileEntry::name() const
{
    char * dup = strdup(namestr);
    char* dirn = basename(dup);
    QString res(dirn);
    free(dup);
    return res;
}

QString SftpFileEntry::fullPath() const
{
    return namestr;
}

void SftpFileEntry::handleError(int res, const QString &operation, bool pointer) const
{
    if ((pointer && res) || (res == LIBSSH2_ERROR_SFTP_PROTOCOL))
    {
        switch (libssh2_sftp_last_error(session->sftp))
        {
        case LIBSSH2_FX_WRITE_PROTECT:
        case LIBSSH2_FX_OP_UNSUPPORTED:
        case LIBSSH2_FX_PERMISSION_DENIED:
            throw AccessDenied();
        case LIBSSH2_FX_DIR_NOT_EMPTY:
            throw DirNotEmpty(QString(namestr));
        case LIBSSH2_FX_FILE_ALREADY_EXISTS:
            throw FileExists(QString(namestr));
        case LIBSSH2_FX_INVALID_FILENAME:
        case LIBSSH2_FX_INVALID_HANDLE:
            throw FileDoesNotExist(QString(namestr));
        case LIBSSH2_FX_NOT_A_DIRECTORY:
            throw NotADir(QString(namestr));
        default:
            throw UnknownError("Can't perform "+operation+" on remote file: "+namestr);
        }
    } else if (res < 0)
        throw UnknownError(QString("Error in ssh connection %1").arg(res));
}
void SftpFileEntry::stat(LIBSSH2_SFTP_ATTRIBUTES *buf) const
{
    int res = libssh2_sftp_stat(session->sftp, namestr, buf);
    handleError(res,"stat");
}

int SftpFileEntry::permissions() const
{
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    stat(&attrs);
    if (attrs.flags & LIBSSH2_SFTP_ATTR_PERMISSIONS)
        return 0777 & attrs.permissions;
    else
        throw OperationNotSupported("File permissions");
}

DateTime SftpFileEntry::time() const
{
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    stat(&attrs);
    if (attrs.flags & LIBSSH2_SFTP_ATTR_ACMODTIME)
    {
        struct tm time;
        const time_t unix = attrs.mtime;
        if (localtime_r(&unix, &time))
            return DateTime(time.tm_year+1900, (time.tm_mon+1), time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

    }
    throw OperationNotSupported("File modification time");
}

void SftpFileEntry::changePermissions(int newPermissions)
{
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    stat(&attrs);
    if (attrs.flags & LIBSSH2_SFTP_ATTR_PERMISSIONS)
    {
        attrs.permissions &= ~0777;
        attrs.permissions |= 0777&newPermissions;
        attrs.flags = LIBSSH2_SFTP_ATTR_PERMISSIONS;
        int res = libssh2_sftp_setstat(session->sftp, namestr, &attrs);
        handleError(res,"setstat");
    } else
        throw OperationNotSupported("File permissions");
}

void SftpFileEntry::rename(QString newName)
{
    if (newName.contains('/'))
        throw UnknownError("Invalid filename");
    char * dup = strdup(namestr);
    QString fullnameQStr(QString(dirname(dup))+"/"+newName);
    free(dup);
    QByteArray fullnameByteArray = fullnameQStr.toUtf8();
    char * fullname = fullnameByteArray.data();
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    if (!libssh2_sftp_stat(session->sftp, fullname, &attrs))
        throw FileExists(fullnameQStr);
    int ret = libssh2_sftp_rename(session->sftp, namestr, fullname);
    if (ret)
        handleError(ret, "rename");
    else
    {
        free(namestr);
        namestr = strdup(fullname);
    }
}

void SftpFileEntry::remove()
{
    handleError(libssh2_sftp_unlink(session->sftp, namestr), "unlink");
}
