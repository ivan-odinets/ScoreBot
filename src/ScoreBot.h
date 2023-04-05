/*
 **********************************************************************************************************************
 *
 * This file is part of ScoreBot project
 *
 * Copyright (c) 2021-2023 Ivan Odinets <i_odinets@protonmail.com>
 *
 * ScoreBot is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * ScoreBot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ScoreBot; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef SCOREBOT_H
#define SCOREBOT_H

#include <QObject>
#include <QTimeZone>
#include "QtTelegramBot.h"

#include "Database.h"

class ScoreBot : public QObject
{
    Q_OBJECT
public:
    explicit ScoreBot(QObject* parent = nullptr);
    virtual ~ScoreBot();

    bool started();
    void setDatabase(Database* db)               { p_db = db; }
    void setApiKey(const QString& key)           { m_botApiKey = key; }
    void setBotAdmin(qint32 userId)              { m_botAdmin = userId; }
    void setTimeZone(const QTimeZone& timeZone)  { m_timeZone = timeZone; }

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
    void backupDatabaseCmd(const Telegram::Message& message);
    void suspendBot(const Telegram::Message& message);
    void resumeBot(const Telegram::Message& message);
    void getVersionCmd(const Telegram::Message& message);

    void handleSomeRandomStuff(const Telegram::Message& message);

    Telegram::Bot*   p_botApi;
    Telegram::User   m_botUser;
    QString          m_botApiKey;
    qint64           m_botAdmin;
    QTimeZone        m_timeZone;
    Database*        p_db;
    bool             isSuspended;

    void _sendReply(const QString& text,const Telegram::Message& message);
};

#endif // SCOREBOT_H
