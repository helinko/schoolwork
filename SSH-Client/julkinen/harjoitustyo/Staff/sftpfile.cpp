#include "sftpfile.h"

using namespace Staff;
using namespace FSInterface;

SftpFile::SftpFile(SftpConnection *session, char *name)
    : SftpFileEntry(session, name), openHandle(nullptr)
{
}

SftpFile::~SftpFile()
{
    if (openHandle)
        close();
}

long SftpFile::size() const
{
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    stat(&attrs);
    if (attrs.flags & LIBSSH2_SFTP_ATTR_SIZE)
        return attrs.filesize;
    else
        return 0;
}

void SftpFile::open(Mode mode)
{
    openHandle = libssh2_sftp_open(session->sftp,namestr, (mode==WRITE?
                                        LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC :
                                        LIBSSH2_FXF_READ),0600);
    handleError(openHandle == nullptr, "open", true);
}

int SftpFile::read(QByteArray &array)
{
    int res = libssh2_sftp_read(openHandle, array.data(), array.length());
    handleError(res, "read");
    return res;
}

int SftpFile::write(const QByteArray &array)
{
    int res = libssh2_sftp_write(openHandle, array.data(), array.length());
    handleError(res, "write");
    return res;
}

void SftpFile::close()
{
    libssh2_sftp_close(openHandle);
    openHandle = nullptr;
}
