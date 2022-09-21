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

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QPair>
#include <QtSql/QSqlDatabase>

#include <QDateTime>

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

    bool userIsRegistered(qint64 chatId,qint64 userId);
    void registerUser(qint64 chatId,qint64 userId);
    void unregisterUser(qint64 chatId,qint64 userId);

    bool userIsActive(qint64 chatId,qint64 userId);
    void activateUser(qint64 chatId,qint64 userId);
    void deactivateUser(qint64 chatId,qint64 userId);

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
