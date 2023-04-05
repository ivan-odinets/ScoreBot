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

#include "Database.h"

#include <QDebug>
#include <QDateTime>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql>

Database::Database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

bool Database::databaseInitialized()
{
    if (!db.open()) {
        qCritical() << "Error in initializing database - "<<db.lastError();
        return false;
    }

    return true;
}

/*
 *********************************************************************************************************************
 * Retrieving & updating chats information (tables)
 */

bool Database::chatRegistered(qint64 chatId)
{
    return db.tables().contains( QString::number(chatId) );
}

void Database::addChat(qint64 chatId)
{
    if (Q_UNLIKELY(chatRegistered(chatId))) {
        qWarning() << "This should have not happened. Probably database needs to be rechecked. chat_id="<<chatId;
        return;
    }

    QString queryText = QString("CREATE TABLE \"%1\" (\"user_id\" INTEGER, \"score\" INTEGER, \"in_game\" INTEGER, \"last_rolled\" INTEGER)")
            .arg(chatId);
    QSqlQuery query;

    if (Q_UNLIKELY(!query.exec(queryText))) {
        qWarning() << "SQL error: "<<query.lastError();
        return;
    }

}

void Database::removeChat(qint64 chatId)
{
    QSqlQuery query;
    QString queryText = QString("DROP TABLE \"%1\"")
            .arg(chatId);

    if (Q_UNLIKELY(!query.exec(queryText))) {
        qWarning() << "SQL error: "<<query.lastError();
        return;
    }
}

QList<qint64> Database::getAllChats() const
{
    QList<qint64> result;

    foreach (QString tableName,db.tables()) {
        qint64 chatId = tableName.toLongLong();
        if (chatId != 0)
            result.append(chatId);
    }

    return result;
}

/*
 *********************************************************************************************************************
 * Retrieving & updating user resistration information
 */

bool Database::userIsRegistered(qint64 chatId,qint64 userId)
{
    QSqlQuery query;
    QString queryText = QString("SELECT user_id FROM \"%1\" WHERE user_id = %2")
            .arg(chatId).arg(userId);

    if (Q_UNLIKELY(!query.exec(queryText.toLatin1()))) {
        qWarning() << "SQL error: "<<query.lastError();
        return false;
    }

    int rowsCount = 0;
    while (query.next())
        rowsCount++;

    if (rowsCount == 0)
        return false;

    return true;
}

void Database::registerUser(qint64 chatId,qint64 userId)
{
    QSqlQuery query;

    QString queryText = QString("INSERT INTO \"%1\" (user_id,score,in_game,last_rolled) VALUES (%2,0,1,0);")
            .arg(chatId).arg(userId);

    if (Q_UNLIKELY(!query.exec(queryText)))
        qWarning() << "SQL error: "<<query.lastError();

    return;
}

void Database::unregisterUser(qint64 chatId,qint64 userId)
{
    QSqlQuery query;
    QString queryText = QString("UPDATE \"%1\" SET in_game = 0 WHERE user_id = %2")
            .arg(chatId).arg(userId);

    if (Q_UNLIKELY(!query.exec(queryText)))
        qWarning() << "SQL error: "<<query.lastError();
}

/*
 *********************************************************************************************************************
 *
 * Retrieving & updating user activity information
 *
 */

bool Database::userIsActive(qint64 chatId,qint64 userId)
{
    if (Q_UNLIKELY(!userIsRegistered(chatId,userId))) {
        qInfo() << "This should not happen... userId="<<userId<<"; chatId="<<chatId;
        return false;
    }

    QSqlQuery query;
    QString queryText = QString("SELECT user_id,in_game FROM \"%1\" WHERE user_id = %2")
            .arg(chatId).arg(userId);

    if (Q_UNLIKELY(!query.exec(queryText))) {
        qWarning() << "SQL error: "<<query.lastError();
        return false;
    }

    query.next();//Should be executed only once
    bool result = (query.value("in_game").toInt() == 1);

    if (query.next())
        qWarning() << "Duplicates in database?";

    return result;
}

void Database::activateUser(qint64 chatId,qint64 userId)
{
    QSqlQuery query;
    QString queryText = QString("UPDATE \"%1\" SET in_game = 1 WHERE user_id = %2")
            .arg(chatId).arg(userId);

    if (Q_UNLIKELY(!query.exec(queryText)))
        qWarning() << "SQL error: "<<query.lastError();

}

void Database::deactivateUser(qint64 chatId,qint64 userId)
{
    QSqlQuery query;
    QString queryText = QString("UPDATE \"%1\" SET in_game = 0 WHERE user_id = %2")
            .arg(chatId).arg(userId);

    if (Q_UNLIKELY(!query.exec(queryText)))
        qWarning() << "SQL error: "<<query.lastError();

}

/*
 *********************************************************************************************************************
 * Retrieving & updating user paramaters
 */

void Database::updateScore(qint64 chatId,qint64 userId,int score)
{
    QSqlQuery query;
    QString queryText = QString("UPDATE \"%1\" SET score = %3 WHERE user_id = %2")
            .arg(chatId).arg(userId).arg(score);

    if (Q_UNLIKELY(!query.exec(queryText)))
        qWarning() << __FILE__ << ":" << __LINE__ << ". SQL error: "<<query.lastError();
}

int Database::getScore(qint64 chatId,qint64 userId)
{
    QSqlQuery query;
    QString queryText = QString("SELECT user_id,score FROM \"%1\" WHERE user_id = %2")
            .arg(chatId).arg(userId);

    if (!query.exec(queryText)) {
        qWarning() << __FILE__ << ":" << __LINE__ << ". SQL error: "<<query.lastError();
        return -1;
    }

    query.next();
    int score = query.value("score").toInt();

    if (Q_UNLIKELY(!query.exec(queryText)))
        qWarning() << "Seems to have duplicates in database? chatId="<<chatId<<"; userId="<<userId;

    return score;
}

/*
 *********************************************************************************************************************
 * Retrieving & updating roll times
 */

QDateTime Database::lastRolled(qint64 chatId,qint64 userId)
{
    QSqlQuery query;
    QString queryText = QString("SELECT user_id,last_rolled FROM \"%1\" WHERE user_id = %2")
            .arg(chatId).arg(userId);

    if (Q_UNLIKELY(!query.exec(queryText))) {
        qWarning() << "SQL error: "<<query.lastError();
        return QDateTime();
    }

    query.next();
    qint64 secondsSinceEpochLastRoll = query.value("last_rolled").toLongLong();

    QDateTime result = QDateTime::fromSecsSinceEpoch(secondsSinceEpochLastRoll,Qt::UTC);
    if (Q_UNLIKELY(query.next()))
        qWarning() << "Seems to have duplicates in database? chatId="<<chatId<<"; userId="<<userId;

    return result;
}

void Database::updateRollTime(qint64 chatId,qint64 userId, const QDateTime& rollDateTime)
{
    QString queryText(QString("UPDATE \"%1\" SET last_rolled = %3 WHERE user_id = %2")
            .arg(chatId).arg(userId).arg(rollDateTime.toSecsSinceEpoch()));

    QSqlQuery query;
    if (Q_UNLIKELY(!query.exec(queryText))) {
        qWarning() << "SQL error: "<<query.lastError();
        return;
    }
}

/*
 *********************************************************************************************************************
 * Retrieving user top scores
 */

QList<UserData> Database::getTopUsers(qint64 chatId)
{
    QList<UserData> result;

    QSqlQuery query;
    QString queryText = QString("SELECT user_id,score FROM \"%1\"")
            .arg(chatId);

    if (Q_UNLIKELY(!query.exec(queryText))) {
        qWarning() << __FILE__ << ":" << __LINE__ << ". SQL error: "<<query.lastError();
        return result;
    }

    while (query.next()) {
        UserData item(query.value("user_id").toUInt(),query.value("score").toUInt());
        if (item.first == 0)
            continue;

        result.append(item);
    }

    if (result.isEmpty())
        return result;

    std::sort(result.begin(),result.end(),[](const UserData& first,const UserData& second){
        return first.second > second.second;
    });

    return result;
}

QList<UserData> Database::getTopTenUsers(qint64 chatId)
{
    QList<UserData> result;
    QList<UserData> allUsers = getTopUsers(chatId);
    if (allUsers.size() < 10) {
        return allUsers;
    }

    for (int i = 0; (i < 10); i++)
        result.append(allUsers.at(i));

    return result;
}
