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

#include "Database.h"
#include "StringDefines.h"
#include "SuperRandomGenerator.h"

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
        handleCommand(message);
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

    //Easter eggs
    handleSomeRandomStuff(message);
}

void ScoreBot::handlePrivateMessage(const Telegram::Message& message)
{
    if (message.string.startsWith(HELP_CMD)) {
        handleHelpCommand(message);
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

void ScoreBot::handleCommand(const Telegram::Message &message)
{
    if (Q_UNLIKELY(!p_db->chatRegistered(message.chat.id))) {
        qInfo() << __FILE__ << ":" << __LINE__ <<". Probably bot was added to the new chat, while process was not running. Adding chat to database.";
        p_db->addChat(message.chat.id);
    }

    if (message.string.startsWith(ROLL_USER_CMD)) {
        handleRollCommand(message);
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
}

/*
 *********************************************************************************************************************
 *
 * Specific command handlers
 *
 */

void ScoreBot::handleRollCommand(const Telegram::Message &message)
{
    if (!p_db->userIsRegistered(message.chat.id,message.from.id)) {
        p_db->registerUser(message.chat.id,message.from.id);
        _sendReply(QString(USER_REGISTERED_MESSAGE).arg(message.from.firstname),message);
    }

    if (!p_db->userIsActive(message.chat.id,message.from.id)) {
        p_db->activateUser(message.chat.id,message.from.id);
        _sendReply(QString(USER_REGISTERED_MESSAGE).arg(message.from.firstname),message);
    }

    bool canRoll = p_db->lastRolled(message.chat.id,message.from.id).date() < QDate::currentDate();

    if (!canRoll) {
        _sendReply(QString(NO_GAME_TODAY_MESSAGE).arg(message.from.firstname),message);
        return;
    }

    //Handle registered user
    int currentScore = p_db->getScore(message.chat.id,message.from.id);

    int change = SuperRandomGenerator::generateScoreChange(currentScore);
    currentScore += change;
    p_db->updateScore(message.chat.id,message.from.id,currentScore);
    p_db->updateRollTime(message.chat.id,message.from.id,QDateTime::currentDateTime());

    QString messageText = QString(change > 0 ? UPDATE_INCREASE_MESSAGE : UPDATE_DECREASE_MESSAGE)
            .arg(message.from.firstname).arg(std::abs(change)).arg(currentScore);

    _sendReply(messageText,message);
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

void ScoreBot::handleTopCommand(const Telegram::Message &message)
{
    QList<UserData> users = p_db->getTopUsers(message.chat.id);

    QString messageText = TOP_MESSAGE;

    for (int i = 0; i < users.size(); i++) {
        Telegram::ChatMember user = p_botApi->getChatMember(message.chat.id,users.at(i).first);
        QString userFullName(user.user.firstname + " " + user.user.lastname);
        messageText.append(QString("%1. %2 - %3\r\n").arg(i+1).arg(userFullName).arg(users.at(i).second));
    }

    _sendReply(messageText,message);
}

void ScoreBot::handleTopTenCommand(const Telegram::Message &message)
{
    QList<UserData> users = p_db->getTopTenUsers(message.chat.id);

    QString messageText = TOP10_MESSAGE;

    for (int i = 0; i < users.size(); i++) {
        Telegram::ChatMember user = p_botApi->getChatMember(message.chat.id,users.at(i).first);
        QString userFullName(user.user.firstname + " " + user.user.lastname);
        messageText.append(QString("%1. %2 - %3\r\n").arg(i+1).arg(userFullName).arg(users.at(i).second));
    }

    _sendReply(messageText,message);
}

void ScoreBot::handleHelpCommand(const Telegram::Message &message)
{
    _sendReply(HELP_MESSAGE,message);
}

/*
 *********************************************************************************************************************
 *
 * Easter egg
 *
 */

void ScoreBot::handleSomeRandomStuff(const Telegram::Message &message)
{
    if ((message.string == "DROP TABLE") ||
        (message.string.startsWith("/drop"))) {
        _sendReply("Bot database was successfully erased.",message);
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
