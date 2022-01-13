/*
 **********************************************************************************************************************
 *
 * MIT License
 * Copyright (c) 2021 Ivan Odinets
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef SCOREBOT_H
#define SCOREBOT_H

#include <QObject>
#include <QTimeZone>
#include "qttelegrambot.h"

#include "Database.h"
#include "SuperRandomGenerator.h"

class ScoreBot : public QObject
{
    Q_OBJECT
public:
    explicit ScoreBot(QObject* parent = nullptr);
    ~ScoreBot();

    bool started();
    void setDatabase(Database* db)
        { p_db = db; }
    void setApiKey(const QString& key)
        { m_botApiKey = key; }
    void setBotAdmin(qint32 userId)
        { m_botAdmin = userId; }
    void setTimeZone(const QTimeZone& timeZone)
        { m_timeZone = timeZone; }

public slots:
    void messageRecieved(const Telegram::Message& message);

private:
    void handleChatCommand(const Telegram::Message& message);
    void handlePrivateMessage(const Telegram::Message& message);

    void handleNewChat(const Telegram::Message& message);
    void handleChatRemoval(const Telegram::Message& message);

    void rollCmd(const Telegram::Message& message);
    bool userCanRoll(const Telegram::Message& message);
    void handleUnrollCommand(const Telegram::Message& message);

    void handleTopCommand(const Telegram::Message& message);
    void handleTopTenCommand(const Telegram::Message& message);

    void handleHelpCommand(const Telegram::Message& message);
    void handleBotAdminCommand(const Telegram::Message& message);
    void sendGlobalMessageCmd(const Telegram::Message& message);
    void getTotalChatsCmd(const Telegram::Message& message);
    void getVersionCmd(const Telegram::Message& message);

    void handleSomeRandomStuff(const Telegram::Message& message);

    Telegram::Bot*   p_botApi;
    Telegram::User   m_botUser;
    QString          m_botApiKey;
    qint64           m_botAdmin;
    QTimeZone        m_timeZone;
    Database*        p_db;

    void _sendReply(const QString& text,const Telegram::Message& message);
};

#endif // SCOREBOT_H
