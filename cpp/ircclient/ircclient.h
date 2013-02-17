#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <QString>

class IRCClientInterface
{
public:
    static IRCClientInterface *New();
    static void Delete(IRCClientInterface *);
    virtual void cmd(const QString &cmd) = 0;
    virtual bool connect(const QString &host, const qint16 port, const QString &nick, const QString &user) = 0;
    virtual void getData(QString &data) = 0;
};

class UserIRCCMDProcessorInterface
{
public:
    static UserIRCCMDProcessorInterface *New(IRCClientInterface *ircClient);
    static void Delete(UserIRCCMDProcessorInterface *);
    virtual void processUserCMD(const QString &cmd) = 0;
};

#endif // IRCCLIENT_H
