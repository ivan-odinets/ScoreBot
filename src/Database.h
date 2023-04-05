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

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QPair>
#include <QtSql/QSqlDatabase>

#include <QDateTime>

#include "QtTelegramBot/src/types/user.h"

typedef QPair<quint64,qint64> UserData;

class Database
{
public:
    Database();

    bool databaseInitialized();
    void setFileName(const QString& fileName)
        { db.setDatabaseName(fileName); }
    QString getFileName() const
        { return db.databaseName(); }

    void addChat(qint64 chatId);
    bool chatRegistered(qint64 chatId);
    void removeChat(qint64 chatId);
    QList<qint64> getAllChats() const;

    bool userIsRegistered(qint64 chatId,Telegram::User::Id userId);
    void registerUser(qint64 chatId,Telegram::User::Id userId);
    void unregisterUser(qint64 chatId,Telegram::User::Id userId);

    bool userIsActive(qint64 chatId,Telegram::User::Id userId);
    void activateUser(qint64 chatId,Telegram::User::Id userId);
    void deactivateUser(qint64 chatId,Telegram::User::Id userId);

    QList<UserData> getTopTenUsers(qint64 chatId);
    QList<UserData> getTopUsers(qint64 chatId);

    QDateTime lastRolled(qint64 chatId,qint64 userId);
    void updateRollTime(qint64 chatId,qint64 userId,const QDateTime& rollDateTime);

    int getScore(qint64 chatId,qint64 userId);
    void updateScore(qint64 chatId,qint64 userId,int score);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
