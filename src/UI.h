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

#ifndef UI_H
#define UI_H

#include <QObject>
#include <QString>

class UI
{
public:
    static bool exampleFileGenerated(const QString& fileName);
    static bool fileLoaded(const QString& fileName);

    // Yes, code of this class is possible to rewrite using enum/switch combination or enum QMap<enum,QString> combination
    // Method-based approach was used because it should work faster (simple method call vs method call & jump
    // vs method call & QMap internal stuff. And this methods will be invoked very, VERY often. Probably this can be changed
    // in future

    // Command tokens (executable by user)
    static QString   rollCmd()         { return m_rollCmd; }
    static QString   unrollCmd()       { return m_unrollCmd; }
    static QString   helpCmd()         { return m_helpCmd; }
    static QString   topCmd()          { return m_topCmd; }
    static QString   top10Cmd()        { return m_top10Cmd; }
    static QString   fakeDropCmd()     { return m_fakeDropCmd; }

    // Command tokens (executable by admin)
    static QString   sendGlobalMessageCmd()      { return m_sendGlobalMessageCmd; }
    static QString   getTotalChatsCountCmd()     { return m_getTotalChatsCountCmd; }
    static QString   getVersionCmd()             { return m_getVersionCmd; }
    static QString   backupDatabaseCmd()         { return m_backupDatabaseCmd; }
    static QString   suspendBotCmd()             { return m_suspendBotCmd; }
    static QString   resumeBotCmd()              { return m_resumeBotCmd; }

    // Message tokens, can be seen by users in private chats
    static QString   onlyForChatsMessage()  { return m_onlyForChatsMessage; }

    // Messages tokens, can be seen by users in group chats
    static QString   suspendedMessage()          { return m_suspendedMessage; }

    static QString   userRegisteredMessage()     { return m_userRegisteredMessage; }
    static QString   userNotRegisteredMessage()  { return m_userNotRegisteredMessage; };
    static QString   userUnregisteredMessage()   { return m_userUnregisteredMessage; };

    static QString   noGameTodayMessage()        { return m_noGameTodayMessage; }
    static QString   updateIncreaseMessage()     { return m_updateIncreaseMessage; }
    static QString   updateDecreaseMessage()     { return m_updateDecreaseMessage; }

    static QString   topMessage()                { return m_topMessage; };
    static QString   topTenMessage()             { return m_topTenMessage; }

    static QString   helpMessage()               { return m_helpMessage; }

    static QString   fakeDropReply()             { return m_fakeDropReply; }

    // Message tokens, can be seen by bot administrator
    static QString   okMessage()                 { return m_okMessage; }
    static QString   unknownCommandMessage()     { return m_unknownCommandMessage; }
    static QString   errorOpeningDbMessage()     { return m_errorOpeningDb; }
    static QString   totalChatsCountMessage()    { return m_totalChatsCountMsg; }

private:
    UI()=delete;
    Q_DISABLE_COPY(UI);

    // Command tokens (executable by user)
    static QString m_rollCmd;
    static QString m_unrollCmd;
    static QString m_helpCmd;
    static QString m_topCmd;
    static QString m_top10Cmd;
    static QString m_fakeDropCmd;

    // Command tokens (executable by admin)
    static QString m_sendGlobalMessageCmd;
    static QString m_getTotalChatsCountCmd;
    static QString m_getVersionCmd;
    static QString m_backupDatabaseCmd;
    static QString m_suspendBotCmd;
    static QString m_resumeBotCmd;

    // Messages
    static QString m_onlyForChatsMessage;

    // Messages tokens, can be seen by users in group chats
    static QString m_suspendedMessage;

    static QString m_userRegisteredMessage;
    static QString m_userNotRegisteredMessage;
    static QString m_userUnregisteredMessage;

    static QString m_noGameTodayMessage;
    static QString m_updateIncreaseMessage;
    static QString m_updateDecreaseMessage;

    static QString m_topMessage;
    static QString m_topTenMessage;

    static QString m_helpMessage;

    static QString m_fakeDropReply;

    // Message tokens, can be seen by bot administrator
    static QString m_okMessage;
    static QString m_unknownCommandMessage;
    static QString m_errorOpeningDb;
    static QString m_totalChatsCountMsg;

    // Service methods
    static inline QString _loadValue(const QJsonObject& sourceObject, const QString& valueName, const QString& defaultValue);
};

#endif // UI_H
