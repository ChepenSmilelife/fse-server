#ifndef FSEENUM_H
#define FSEENUM_H

/* message: length + content
 * description:
 * length: type -> unsigned short (quint16)
 * content: type -> QList<QVariant>
 *   its sequence is:
 *      0   username    QString
 *      1   password    QString
 *      2   command     enum FSECommandType
 *      3   cmd-argument
 *              if command is following:
 *              0>  Login       argument is empty
 *              1>  PushFilePassword argument type is QStringList
 *                      - filename
 *                      - filemd5
 *                      - filepassword(MD5)
 *              2>  PullFilePassword argument type is QStringList
 *                      - filename
 *                      - filemd5
 *
 * message return: QList<QVariant>
 *      length + state + [description]
 */


enum FSECommandType {
    LoginCMD,
    PushFilePasswordCMD,
    PullFilePasswordCMD
};

enum FSEState {
    LoginSuccessState,
    LoginFailedState,
    PushFilePWDSuccessState,
    PushFilePWDFailedState,
    PullFilePWDSuccessState,
    PullFilePWDFailedState
};


#endif // FSEENUM_H
