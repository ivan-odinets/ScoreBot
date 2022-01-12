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

#ifndef TEXTSTRINGS_H
#define TEXTSTRINGS_H

#define APP_NAME                    "ScoreBot"
#define APP_VERSION                 "0.1.1"

/*
 *********************************************************************************************************************
 *
 * Command tokens
 *
 */

#define ROLL_USER_CMD          "/roll"
#define UNROLL_USER_CMD        "/unroll"
#define HELP_CMD               "/help"
#define TOP_TEN_CMD            "/top10"
#define TOP_CMD                "/top"

#define DROP_FAKECMD           "/drop"

#define SEND_GLOBAL_MESSAGE    "/sendGlobalMessage"
#define GET_TOTAL_CHATS_COUNT  "/getTotalChats"
#define GET_VERSION            "/version"

/*
 *********************************************************************************************************************
 *
 * UI Messages
 *
 */

#define OK_MESSAGE                  "OK"

#define ONLY_FOR_CHATS_MESSAGE      "Only for group chats"

#define USER_REGISTERED_MESSAGE     "%1 you are registered in the game"
#define USER_NOTREGISTERED_MESSAGE  "%1 you are not registered in the game"
#define USER_UNREGISTERED_MESSAGE   "%1 you have unregistered in the game"

#define NO_GAME_TODAY_MESSAGE       "%1 you have rolled today"

#define UPDATE_INCREASE_MESSAGE     "%1, your score increased by %2. Now your score is: %3."
#define UPDATE_DECREASE_MESSAGE     "%1, your score decreased by %2. Now your score is: %3."

#define TOP10_MESSAGE               "Top 10 players:\r\n\r\n"
#define TOP_MESSAGE                 "Top players:\r\n\r\n"

#define HELP_MESSAGE                APP_NAME " - is a very simple \"game\" bot for Telegram group chats.\r\n"\
                                    "You can play in this game each day by entering " ROLL_USER_CMD " in the group chat. After this your score increases/decreases by random value.\r\n"\
                                    "Top scores: " TOP_CMD "\r\n"\
                                    "Exit the game (progress will be saved): " UNROLL_USER_CMD "\r\n"\
                                    "\r\n"\
                                    "Bot is not working in private chats, only in groups."

#define DROP_REPLY                  "Bot database was successfully erased."

#define TOTAL_CHATS_COUNT_MSG       "Total chats count: %1"

#endif // TEXTSTRINGS_H
