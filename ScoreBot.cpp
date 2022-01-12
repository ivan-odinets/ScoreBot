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

#include "ScoreBot.h"

#include "StringDefines.h"
#include "SuperRandomGenerator.h"

#include <QCoreApplication>

ScoreBot::ScoreBot(QObject* parent)
    : QObject(parent),p_botApi(nullptr),p_db(nullptr)
{}
ScoreBot::~ScoreBot()
{}

bool ScoreBot::started()
{
    p_botApi = new Telegram::Bot(m_botApiKey,true,1000);
    connect(p_botApi,&Telegram::Bot::message,this,&ScoreBot::messageRecieved);

    m_botUser = p_botApi->getMe();
    qInfo() << __FILE__ << ":" << __LINE__ << ". Staring process using API key for following bot: "<<m_botUser;

    return true;
}

/*
 *********************************************************************************************************************
 *
 * General message handlers
 *
 */

void ScoreBot::messageRecieved(const Telegram::Message& message)
{
    //Handle all private messages
    if (message.chat.id == message.from.id) {
        handlePrivateMessage(message);
        return;
    }

    if (Q_LIKELY(message.string.startsWith("/"))) {
        handleChatCommand(message);
        return;
    }

    //Bot was added to the new chat
    if ((message.type == Telegram::Message::NewChatParticipantType) &&
        (message.user.id == m_botUser.id)) {
        handleNewChat(message);
        return;
    }

    //Bot was kicked out from a chat
    if ((message.type == Telegram::Message::LeftChatParticipantType) &&
        (message.user.id == m_botUser.id)) {
        handleChatRemoval(message);
        return;
    }
}

void ScoreBot::handlePrivateMessage(const Telegram::Message& message)
{
    if (message.string.startsWith(HELP_CMD)) {
        handleHelpCommand(message);
        return;
    }

    if (message.from.id == m_botAdmin) {
        handleBotAdminCommand(message);
        return;
    }

    _sendReply(ONLY_FOR_CHATS_MESSAGE,message);
}

void ScoreBot::handleNewChat(const Telegram::Message& message)
{
    p_db->addChat(message.chat.id);
}

void ScoreBot::handleChatRemoval(const Telegram::Message& message)
{
    p_db->removeChat(message.chat.id);
}

void ScoreBot::handleHelpCommand(const Telegram::Message &message)
{
    _sendReply(HELP_MESSAGE,message);
}

void ScoreBot::handleChatCommand(const Telegram::Message &message)
{
    if (Q_UNLIKELY(!p_db->chatRegistered(message.chat.id))) {
        qInfo() << __FILE__ << ":" << __LINE__ <<". Probably bot was added to the new chat, while process was not running. Adding chat to database.";
        p_db->addChat(message.chat.id);
    }

    if (message.string.startsWith(ROLL_USER_CMD)) {
        rollCmd(message);
        return;
    }

    if (message.string.startsWith(UNROLL_USER_CMD)) {
        handleUnrollCommand(message);
        return;
    }

    if (message.string.startsWith(TOP_TEN_CMD)) {
        handleTopTenCommand(message);
        return;
    }

    if (message.string.startsWith(TOP_CMD)) {
        handleTopCommand(message);
        return;
    }

    if (message.string.startsWith(HELP_CMD)) {
        handleHelpCommand(message);
        return;
    }

    //Easter eggs
    handleSomeRandomStuff(message);
}

/*
 *********************************************************************************************************************
 *
 * Bot admin commands implementation
 *
 */

void ScoreBot::handleBotAdminCommand(const Telegram::Message& message)
{
    if (message.string.startsWith(SEND_GLOBAL_MESSAGE)) {
        sendGlobalMessageCmd(message);
        return;
    }

    if (message.string.startsWith(GET_TOTAL_CHATS_COUNT)) {
        getTotalChatsCmd(message);
        return;
    }

    if (message.string.startsWith(GET_VERSION)) {
        getVersionCmd(message);
        return;
    }
}

void ScoreBot::sendGlobalMessageCmd(const Telegram::Message &message)
{
    QString messageText = message.string;
    messageText.remove(SEND_GLOBAL_MESSAGE);
    foreach (qint64 chatId,p_db->getAllChats()) {
        p_botApi->sendMessage(chatId,messageText);
    }
    _sendReply(OK_MESSAGE,message);
    return;
}

void ScoreBot::getTotalChatsCmd(const Telegram::Message &message)
{
    QString replyText = QString(TOTAL_CHATS_COUNT_MSG).arg(
            p_db->getAllChats().size());
    _sendReply(replyText,message);
    return;
}

void ScoreBot::getVersionCmd(const Telegram::Message &message)
{
    _sendReply(qApp->applicationVersion(),message);
}

/*
 *********************************************************************************************************************
 *
 * /roll and /unroll commands
 *
 */

void ScoreBot::rollCmd(const Telegram::Message &message)
{
    if (!p_db->userIsRegistered(message.chat.id,message.from.id)) {
        p_db->registerUser(message.chat.id,message.from.id);
        _sendReply(QString(USER_REGISTERED_MESSAGE).arg(message.from.firstname),message);
    }

    if (!p_db->userIsActive(message.chat.id,message.from.id)) {
        p_db->activateUser(message.chat.id,message.from.id);
        _sendReply(QString(USER_REGISTERED_MESSAGE).arg(message.from.firstname),message);
    }

    if (!userCanRoll(message)) {
        _sendReply(QString(NO_GAME_TODAY_MESSAGE).arg(message.from.firstname),message);
        return;
    }

    //Handle registered user
    int currentScore = p_db->getScore(message.chat.id,message.from.id);

    int change = SuperRandomGenerator::generateScoreChange(currentScore);
    currentScore += change;

    p_db->updateScore(message.chat.id,message.from.id,currentScore);
    //DB contains data about time in UTC, so we need to convert from local clock to UTC
    p_db->updateRollTime(message.chat.id,message.from.id,QDateTime::currentDateTime().toUTC());

    QString messageText = QString(change > 0 ? UPDATE_INCREASE_MESSAGE : UPDATE_DECREASE_MESSAGE)
            .arg(message.from.firstname).arg(std::abs(change)).arg(currentScore);

    _sendReply(messageText,message);
}

bool ScoreBot::userCanRoll(const Telegram::Message &message)
{
    //DB contains data about time in UTC, so we need to convert it to specified timezone
    QDateTime lastRolled = p_db->lastRolled(message.chat.id,message.from.id);
    lastRolled = lastRolled.toTimeZone(m_timeZone);

    return lastRolled.date() < QDate::currentDate();
}

void ScoreBot::handleUnrollCommand(const Telegram::Message &message)
{
    if (!p_db->userIsRegistered(message.chat.id,message.from.id)) {
        _sendReply(QString(USER_NOTREGISTERED_MESSAGE).arg(message.from.firstname),message);
        return;
    }

    p_db->deactivateUser(message.chat.id,message.from.id);
    _sendReply(QString(USER_UNREGISTERED_MESSAGE).arg(message.from.firstname),message);
}

/*
 *********************************************************************************************************************
 *
 * Managing /top and /top10 commands
 *
 */

QString ScoreBot::_userListToString(const QList<UserData> &userList,const Telegram::Message& commandMessage)
{
    QString result = QString();
    int resultCount = 0;
    for (auto i = userList.constBegin(); i != userList.constEnd(); i++) {
        Telegram::ChatMember user = p_botApi->getChatMember(commandMessage.chat.id,i->first);
        //If user has left the chat - omit his/her result
        if (user.user.id == 0)
            continue;

        resultCount++;
        QString userFullName(user.user.firstname + " " + user.user.lastname);
        result.append(QString("%1. %2 - %3\r\n").arg(resultCount).arg(userFullName).arg(i->second));
    }

    return result;
}

void ScoreBot::handleTopCommand(const Telegram::Message &message)
{
    QList<UserData> users = p_db->getTopUsers(message.chat.id);

    _sendReply(TOP_MESSAGE+_userListToString(users,message),message);
}

void ScoreBot::handleTopTenCommand(const Telegram::Message &message)
{
    QList<UserData> users = p_db->getTopTenUsers(message.chat.id);

    _sendReply(TOP10_MESSAGE+_userListToString(users,message),message);
}

/*
 *********************************************************************************************************************
 *
 * Easter egg
 *
 */

void ScoreBot::handleSomeRandomStuff(const Telegram::Message &message)
{
    if (message.string.startsWith(DROP_FAKECMD)) {
        _sendReply(DROP_REPLY,message);
        return;
    }
}

/*
 *********************************************************************************************************************
 *
 * Some service stuff...
 *
 */

void ScoreBot::_sendReply(const QString &text, const Telegram::Message &message)
{
    p_botApi->sendMessage(message.chat.id,text,message.id);
}
