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

#include "ScoreBot.h"

#include "UI.h"
#include "SuperRandomGenerator.h"

#include <QCoreApplication>

ScoreBot::ScoreBot(QObject* parent)
    : QObject(parent),p_botApi(nullptr),p_db(nullptr),isSuspended(false)
{}
ScoreBot::~ScoreBot()
{}

bool ScoreBot::started()
{
    p_botApi = new Telegram::Bot(m_botApiKey,1000);
    connect(p_botApi,&Telegram::Bot::message,this,&ScoreBot::messageRecieved);

    m_botUser = p_botApi->getMe();
    qInfo() << "Staring process using API key for following bot: "<<m_botUser;

    return true;
}

/*
 *********************************************************************************************************************
 * General message handlers
 */

void ScoreBot::messageRecieved(const Telegram::Message& message)
{
    qDebug() << message;
    //Handle all private messages
    if (message.chat().id() == message.from().id()) {
        handlePrivateMessage(message);
        return;
    }

    if (Q_LIKELY(message.text().startsWith(QChar('/')))) {
        handleChatCommand(message);
        return;
    }

    //Bot was added to the new chat
    if (message.type() == Telegram::Message::NewChatParticipantType) {
        if (message.newChatMembers().count() == 0) {
            qDebug() << "This should not happen. Probably bug in QtTelegramBot";
            return;
        }

        for (Telegram::User newMember : message.newChatMembers()) {
            if (newMember.id() == m_botUser.id()) {
                handleNewChat(message);
                return;
            }
        }
        return;
    }

    //Bot was kicked out from a chat
    if (message.type() == Telegram::Message::LeftChatParticipantType) {
        if (message.leftChatMember().id() == m_botUser.id())
            handleChatRemoval(message);
    }
}

void ScoreBot::handlePrivateMessage(const Telegram::Message& message)
{
    if (message.text().startsWith(UI::helpCmd())) {
        handleHelpCommand(message);
        return;
    }

    if (message.from().id() == m_botAdmin) {
        handleBotAdminCommand(message);
        return;
    }

    _sendReply(UI::onlyForChatsMessage(),message);
}

void ScoreBot::handleNewChat(const Telegram::Message& message)
{
    p_db->addChat(message.chat().id());
}

void ScoreBot::handleChatRemoval(const Telegram::Message& message)
{
    p_db->removeChat(message.chat().id());
}

void ScoreBot::handleHelpCommand(const Telegram::Message &message)
{
    _sendReply(UI::helpMessage(),message);
}

void ScoreBot::handleChatCommand(const Telegram::Message &message)
{
    if (Q_UNLIKELY(isSuspended)) {
        _sendReply(UI::suspendedMessage(),message);
        return;
    }

    if (Q_UNLIKELY(!p_db->chatRegistered(message.chat().id()))) {
        qInfo() << "Probably bot was added to the new chat, while process was not running. Adding chat "<< message.chat() <<"to database.";
        p_db->addChat(message.chat().id());
    }

    if (message.text().startsWith(UI::rollCmd())) {
        rollCmd(message);
        return;
    }

    if (message.text().startsWith(UI::unrollCmd())) {
        handleUnrollCommand(message);
        return;
    }

    if (message.text().startsWith(UI::top10Cmd())) {
        handleTopTenCommand(message);
        return;
    }

    if (message.text().startsWith(UI::topCmd())) {
        handleTopCommand(message);
        return;
    }

    if (message.text().startsWith(UI::helpCmd())) {
        handleHelpCommand(message);
        return;
    }

    //Easter eggs
    handleSomeRandomStuff(message);
}

/*
 *********************************************************************************************************************
 * Bot admin commands implementation
 */

void ScoreBot::handleBotAdminCommand(const Telegram::Message& message)
{
    if (message.text().startsWith(UI::sendGlobalMessageCmd())) {
        sendGlobalMessageCmd(message);
        return;
    }

    if (message.text().startsWith(UI::getTotalChatsCountCmd())) {
        getTotalChatsCmd(message);
        return;
    }

    if (message.text().startsWith(UI::getVersionCmd())) {
        getVersionCmd(message);
        return;
    }

    if (message.text().startsWith(UI::backupDatabaseCmd())) {
        backupDatabaseCmd(message);
        return;
    }

    if (message.text().startsWith(UI::suspendBotCmd())) {
        suspendBot(message);
        return;
    }

    if (message.text().startsWith(UI::resumeBotCmd())) {
        resumeBot(message);
        return;
    }

    if (message.text().startsWith(QChar('/'))) {
        _sendReply(UI::unknownCommandMessage()
                   .arg(message.text()),message);
        return;
    }
}

void ScoreBot::sendGlobalMessageCmd(const Telegram::Message &message)
{
    QString messageText = message.text();
    messageText.remove(UI::sendGlobalMessageCmd());
    foreach (qint64 chatId,p_db->getAllChats()) {
        p_botApi->sendMessage(chatId,messageText);
    }
    _sendReply(UI::okMessage(),message);
    return;
}

void ScoreBot::getTotalChatsCmd(const Telegram::Message &message)
{
    QString replyText = QString(UI::getTotalChatsCountCmd())
                                .arg(p_db->getAllChats().size());
    _sendReply(replyText,message);
    return;
}

void ScoreBot::backupDatabaseCmd(const Telegram::Message& message)
{
    QFile dbFile(p_db->getFileName());
    if (!dbFile.exists()) {
        qWarning() << "Error database file not found! p_db->getFileName()="<<p_db->getFileName();
        _sendReply(UI::errorOpeningDbMessage(),message);
    }

    p_botApi->sendDocument(message.from().id(),&dbFile,message.id());
    return;
}

void ScoreBot::suspendBot(const Telegram::Message& message)
{
    isSuspended = true;
    _sendReply(UI::okMessage(),message);
}

void ScoreBot::resumeBot(const Telegram::Message& message)
{
    isSuspended = false;
    _sendReply(UI::okMessage(),message);
}

void ScoreBot::getVersionCmd(const Telegram::Message &message)
{
    _sendReply(qApp->applicationVersion(),message);
}

/*
 *********************************************************************************************************************
 * /roll and /unroll commands
 */

void ScoreBot::rollCmd(const Telegram::Message &message)
{
    if (!p_db->userIsRegistered(message.chat().id(),message.from().id())) {
        p_db->registerUser(message.chat().id(),message.from().id());
        _sendReply(UI::userRegisteredMessage().arg(message.from().firstName()),message);
    }

    if (!p_db->userIsActive(message.chat().id(),message.from().id())) {
        p_db->activateUser(message.chat().id(),message.from().id());
        _sendReply(UI::userNotRegisteredMessage().arg(message.from().firstName()),message);
    }

#ifndef ENDLESS_ROLL
    if (!userCanRoll(message)) {
        _sendReply(UI::noGameTodayMessage().arg(message.from().firstName()),message);
        return;
    }
#endif //ENDLESS_ROLL

    //Handle registered user
    int currentScore = p_db->getScore(message.chat().id(),message.from().id());

    int change = SuperRandomGenerator::generateScoreChange(currentScore);
    currentScore += change;

    p_db->updateScore(message.chat().id(),message.from().id(),currentScore);
    //DB contains data about time in UTC, so we need to convert from local clock to UTC
    p_db->updateRollTime(message.chat().id(),message.from().id(),QDateTime::currentDateTime().toUTC());

    QString messageText = QString(change > 0 ? UI::updateIncreaseMessage() : UI::updateDecreaseMessage())
            .arg(message.from().firstName()).arg(std::abs(change)).arg(currentScore);

    _sendReply(messageText,message);
}

bool ScoreBot::userCanRoll(const Telegram::Message &message)
{
    //DB contains data about time in UTC, so we need to convert it to specified timezone
    QDateTime lastRolled = p_db->lastRolled(message.chat().id(),message.from().id());
    if (m_timeZone.isValid())
        lastRolled = lastRolled.toTimeZone(m_timeZone);

    return lastRolled.date() < QDate::currentDate();
}

void ScoreBot::handleUnrollCommand(const Telegram::Message &message)
{
    if (!p_db->userIsRegistered(message.chat().id(),message.from().id())) {
        _sendReply(UI::userNotRegisteredMessage().arg(message.from().firstName()),message);
        return;
    }

    p_db->deactivateUser(message.chat().id(),message.from().id());
    _sendReply(UI::userUnregisteredMessage().arg(message.from().firstName()),message);
}

/*
 *********************************************************************************************************************
 * Managing /top and /top10 commands
 */

void ScoreBot::handleTopCommand(const Telegram::Message &message)
{
    QList<UserData> users = p_db->getTopUsers(message.chat().id());

    QString result = QString();
    int resultCount = 0;
    for (auto i = users.constBegin(); i != users.constEnd(); i++) {
        Telegram::ChatMember user = p_botApi->getChatMember(message.chat().id(),i->first);
        //If user has left the chat - omit his/her result
        if (user.user().id() == 0)
            continue;

        resultCount++;
        QString userFullName(user.user().firstName() + " " + user.user().lastName());
        result.append(QString("%1. %2 - %3\r\n").arg(resultCount).arg(userFullName).arg(i->second));
    }

    _sendReply(UI::topMessage()+result,message);
}

void ScoreBot::handleTopTenCommand(const Telegram::Message &message)
{
    QList<UserData> users = p_db->getTopUsers(message.chat().id());

    QString result = QString();
    int resultCount = 0;
    for (auto i = users.constBegin(); i != users.constEnd(); i++) {
        Telegram::ChatMember user = p_botApi->getChatMember(message.chat().id(),i->first);
        //If user has left the chat - omit his/her result
        if (user.user().id() == 0)
            continue;

        resultCount++;
        QString userFullName(user.user().firstName() + " " + user.user().lastName());
        result.append(QString("%1. %2 - %3\r\n").arg(resultCount).arg(userFullName).arg(i->second));
        if (resultCount >= 10) //If we have already top10 results - time to finish loop
            break;
    }

    _sendReply(UI::topTenMessage()+result,message);
}

/*
 *********************************************************************************************************************
 * Easter egg
 */

void ScoreBot::handleSomeRandomStuff(const Telegram::Message &message)
{
    if (message.text().startsWith(UI::fakeDropCmd())) {
        _sendReply(UI::fakeDropReply(),message);
        return;
    }
}

/*
 *********************************************************************************************************************
 * Some service stuff...
 */

void ScoreBot::_sendReply(const QString &text, const Telegram::Message &message)
{
    p_botApi->sendMessage(message.chat().id(),text,message.id());
}
