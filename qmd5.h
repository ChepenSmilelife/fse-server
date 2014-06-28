#ifndef QMD5_H
#define QMD5_H

#include "md5.h"

#include <QString>
#include <QDebug>

inline QString md5Str(QString str)
{
    unsigned char digest[MD5_DIGEST_SIZE];
    md5_buffer(str.toStdString().c_str(),
               strlen(str.toStdString().c_str()),
               digest);
    char buffer[MD5_DIGEST_SIZE*2+2];
    buffer[0] = '\0';
    char tmpbuffer[3];
    for(int i = 0; i < MD5_DIGEST_SIZE; ++i)
    {
        sprintf(tmpbuffer, "%2x", digest[i]);
        printf("%x", digest[i]);
        strcat(buffer, tmpbuffer);
    }
    putchar('\n');
    printf("md5: %s\n", buffer);
    QString md5 = QString::fromLatin1(buffer);
    qDebug()<<"Qmd5: "<<md5;
    md5.replace(" ", "0");
    return md5;
}

#endif // QMD5_H
