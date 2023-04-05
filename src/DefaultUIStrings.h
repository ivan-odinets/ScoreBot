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

#ifndef DEFAULTUISTRINGS_H
#define DEFAULTUISTRINGS_H

#include <QString>

namespace DefaultUI {

// Command tokens (executable by user)
static const QString rollCmd      ("/roll");
static const QString unrollCmd    ("/unroll");
static const QString helpCmd      ("/help");
static const QString topCmd       ("/top");
static const QString top10Cmd     ("/top10");
static const QString fakeDropCmd  ("/drop");

// Command tokens (executable by admin)
static const QString sendGlobalMessageCmd   ("/sendGlobalMessage");
static const QString getTotalChatsCountCmd  ("/getTotalChats");
static const QString getVersionCmd          ("/version");
static const QString backupDatabaseCmd      ("/backupDB");
static const QString suspendBotCmd          ("/suspend");
static const QString resumeBotCmd           ("/resume");

// Message tokens, can be seen by users in private chats
static const QString onlyForChatsMessage    ("Only for group chats");

// Messages tokens, can be seen by users in group chats
static const QString suspendedMessage            ("Bot was stopped because of reasonable reasons. It will be started again in the nearby future");

static const QString userRegisteredMessage       ("%1 you are registered in the game");
static const QString userNotRegisteredMessage    ("%1 you are not registered in the game");
static const QString userUnregisteredMessage     ("%1 you have unregistered in the game");

static const QString noGameTodayMessage          ("%1 you have rolled today");
static const QString updateIncreaseMessage       ("%1, your score increased by %2. Now your score is: %3.");
static const QString updateDecreaseMessage       ("%1, your score decreased by %2. Now your score is: %3.");

static const QString topMessage                  ("Top players:\r\n\r\n");
static const QString top10Message                ("Top 10 players:\r\n\r\n");

static const QString helpMessage                 ("This is a very simple \"game\" bot for Telegram group chats.\r\n"\
                                                  "You can play in this game each day by entering " + rollCmd + " in the group chat. After this your score increases/decreases by random value.\r\n"\
                                                  "Top scores: "+topCmd+"\r\n"\
                                                  "Exit the game (progress will be saved): "+unrollCmd+"\r\n"\
                                                  "\r\n"\
                                                  "Bot is not working in private chats, only in groups.");

static const QString fakeDropMessage             ("Bot database was successfully erased.");

// Message tokens, can be seen by bot administrator
static const QString okMessage              ("OK");
static const QString unknownCommandMessage  ("Unknown command: %1");
static const QString errorOpeningDbMessage  ("Error in opening database file. Manual backup needed");
static const QString totalChatsCountMessage ("Total chats count: %1");

}

#endif // DEFAULTUISTRINGS_H
