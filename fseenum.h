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
 *
 *              2>  PullFilePassword argument type is QStringList
 *                      - filemd5       QString
 *
 *              3>  PullUserInfoCMD     argument is empty
 *
 *              4>  PushUserInfoCMD     argument type is QStringList
 *                      - realname      QString
 *                      - age           QString
 *                      - address       QString
 *                      - description   QString
 *                      [- password     QString]  new password
 *
 * message return: QList<QVariant>
 *      length + state + [description]
 *
 *      PullUserInfoCMD + ...
 *                      - username      QString
 *                      - realname      QString
 *                      - age           QString
 *                      - address       QString
 *                      - description   QString
 */


enum FSECommandType {
    LoginCMD,
    PushFilePasswordCMD,
    PullFilePasswordCMD,
    PushUserInfoCMD,
    PullUserInfoCMD
};

enum FSEState {
    LoginSuccessState,
    LoginFailedState,
    PushFilePWDSuccessState,
    PushFilePWDFailedState,
    PullFilePWDSuccessState,
    PullFilePWDFailedState,
    PushUserInfoSuccessState,
    PushUserInfoFailState,
    PullUserInfoSuccessState,
    PullUserInfoFailState
};


#endif // FSEENUM_H
