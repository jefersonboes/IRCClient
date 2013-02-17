#include "ircclient.h"
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QByteArray>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QMap>
#include <QtDebug>

//#az1

class StringUtils
{
public:
    static QString trimLeft(const QString &str)
    {
        int i = 0;

        while (i<str.length() && (str[i] == ' ' ||
                                  str[i] == '\t' ||
                                  str[i] == '\n' ||
                                  str[i] == '\v' ||
                                  str[i] == '\f' ||
                                  str[i] == '\r'))
            i++;

        return str.mid(i, str.length() - i);
    }
};

class CMDProcessor
{
private:
    QString cmd;
    int offset;
public:
    CMDProcessor(const QString &cmd)
    {
        this->cmd = StringUtils::trimLeft(cmd);
        offset = 0;
    }

    bool isUserCMD()
    {
        if (cmd.length() > 0)
            if (cmd[0] == '/')
                return true;

        return false;
    }

    void getUserCMD(QString &cmd)
    {
        if (isUserCMD())
            offset = 1;

        getNextToken(cmd);
        cmd = cmd.toUpper();
    }

    void getNextToken(QString &token)
    {
        token.clear();

        if (cmd[offset] == ' ')
            offset++;

        while (offset < cmd.length() && cmd[offset] != ' ') {
            token += cmd[offset];
            offset++;
        }
    }

    void getString(QString &str)
    {
        str.clear();

        while (offset < cmd.length()) {
            str += cmd[offset];
            offset++;
        }
    }

    bool hasToken()
    {
        return offset < cmd.length();
    }
};

class IRCClient : public QThread, public IRCClientInterface
{
private:
    QAbstractSocket *socket;
    QMutex mutex;
    QQueue<QString> list;
protected:
    void run()
    {
        QString data;

        while (isRunning()) {            
            read(data);

            if (data.length() > 0) {
                mutex.lock();
                list.enqueue(data);
                mutex.unlock();
            }
        }
    }

    void read(QString &data)
    {
        socket->waitForReadyRead(1);
        data = socket->readLine();
    }

public:
    IRCClient()
    {
        socket = new QTcpSocket();
    }

    ~IRCClient()
    {
        terminate();

        wait();

        if (socket->state() == QAbstractSocket::ConnectedState) {
            quit("");
            socket->waitForDisconnected();
        }

        delete socket;
    }

    void cmd(const QString &cmd)
    {
        qDebug() << "send: " + cmd;
        socket->write(QString(cmd + "\n").toUtf8());
    }

    bool connect(const QString &host, const qint16 port, const QString &nick, const QString &user)
    {
        socket->connectToHost(host, port);

        if (!socket->waitForConnected())
            return false;

        start();

        cmd("NICK " + nick);
        cmd("USER " + user + " \"\"" + " \"" + host + "\" :" + nick);

        return true;
    }

    void join(const QString &target)
    {
        cmd("JOIN " + target);
    }

    void part(const QString &target)
    {
        cmd("PART " + target);
    }

    void quit(const QString &msg)
    {
        cmd("QUIT :" + msg);
    }

    void privMSG(const QString &target, const QString &msg)
    {
        cmd("PRIVMSG " + target + " :" + msg);
    }

    void nick(const QString &nick)
    {
        cmd("NICK " + nick);
    }

    void getData(QString &data)
    {
        mutex.lock();
        if (!list.isEmpty())
            data = list.dequeue();
        mutex.unlock();
    }
};

IRCClientInterface *IRCClientInterface::New()
{
    return new IRCClient();
}

void IRCClientInterface::Delete(IRCClientInterface *ircClient)
{
    delete (IRCClient*) ircClient;
}

typedef void (UserIRCCMDProcessorInterface::*CMDProcessMethod) (CMDProcessor &);

class UserIRCCMDProcessor : public UserIRCCMDProcessorInterface
{
private:
    IRCClient *ircClient;
    QMap<QString, CMDProcessMethod> methods;

    void processJOIN(CMDProcessor &cmdProcessor)
    {
        QString target;
        cmdProcessor.getString(target);
        ircClient->join(target);
    }

    void processQUIT(CMDProcessor &cmdProcessor)
    {
        QString msg;
        cmdProcessor.getString(msg);
        ircClient->quit(msg);
    }

    void processPRIVMSG(CMDProcessor &cmdProcessor)
    {
        QString target;
        QString msg;

        cmdProcessor.getNextToken(target);
        cmdProcessor.getString(msg);
        ircClient->privMSG(target, msg);
    }

    void processPART(CMDProcessor &cmdProcessor)
    {
        QString target;

        cmdProcessor.getNextToken(target);
        ircClient->part(target);
    }

    void processNICK(CMDProcessor &cmdProcessor)
    {
        QString nick;

        cmdProcessor.getNextToken(nick);
        ircClient->nick(nick);
    }

    void registerMethods()
    {
        methods.insert("JOIN", (CMDProcessMethod) &UserIRCCMDProcessor::processJOIN);
        methods.insert("QUIT", (CMDProcessMethod) &UserIRCCMDProcessor::processQUIT);
        methods.insert("PRIVMSG", (CMDProcessMethod) &UserIRCCMDProcessor::processPRIVMSG);
        methods.insert("PART", (CMDProcessMethod) &UserIRCCMDProcessor::processPART);
        methods.insert("NICK", (CMDProcessMethod) &UserIRCCMDProcessor::processNICK);
    }

public:
    UserIRCCMDProcessor(IRCClient *ircClient)
    {
        this->ircClient = ircClient;
        registerMethods();
    }

    void processUserCMD(const QString &cmd)
    {
        if (cmd.length() == 0)
            return;

        CMDProcessor cmdProcessor(cmd);

        if (cmdProcessor.isUserCMD()) {
            QString token;

            cmdProcessor.getUserCMD(token);

            if (methods.contains(token))
                ((this)->*methods[token]) (cmdProcessor);
            else
                ircClient->cmd(cmd);
        } else
            ircClient->cmd(cmd);
    }
};

UserIRCCMDProcessorInterface *UserIRCCMDProcessorInterface::New(IRCClientInterface *ircClient)
{
    return new UserIRCCMDProcessor((IRCClient*) ircClient);
}

void UserIRCCMDProcessorInterface::Delete(UserIRCCMDProcessorInterface *userIRCCMDProcessor)
{
    delete (UserIRCCMDProcessor*) userIRCCMDProcessor;
}
