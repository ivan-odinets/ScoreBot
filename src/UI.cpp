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

#include "UI.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "DefaultUIStrings.h"

/*
 *********************************************************************************************************************
 * Names of UI strings in JSON file
 */

const QLatin1String ROLL_CMD_TOKEN(       "RollCmd"      );
const QLatin1String UNROLL_CMD_TOKEN(     "UnrollCmd"    );
const QLatin1String HELP_CMD_TOKEN(       "HelpCmd"      );
const QLatin1String TOP_CMD_TOKEN(        "TopCmd"       );
const QLatin1String TOP10_CMD_TOKEN(      "Top10Cmd"     );
const QLatin1String DROP_CMD_TOKEN(       "DropCmd"      );

const QLatin1String SEND_GLOBAL_MESSAGE_CMD_TOKEN(  "SendGlobalMessageCmd"   );
const QLatin1String GET_TOTAL_CHATS_CMD_TOKEN(      "GetTotalChatsCmd"       );
const QLatin1String GET_VERSION_CMD_TOKEN(          "GetVersionCmd"          );
const QLatin1String BACKUP_DATABASE_CMD_TOKEN(      "BackupDatabaseCmd"      );
const QLatin1String SUSPEND_BOT_CMD_TOKEN(          "SuspendBotCmd"          );
const QLatin1String RESUME_BOT_CMD_TOKEN(           "ResumeBotCmd"           );

const QLatin1String ONLY_FOR_CHATS_MSG_TOKEN(         "OnlyForChatsMessage"         );

// Messages tokens, can be seen by users in group chats
const QLatin1String SUSPENDED_MSG_TOKEN(              "SuspendedMessage"            );

const QLatin1String USER_REGISTERED_MSG_TOKEN(        "UserRegisteredMessage"       );
const QLatin1String USER_NOT_REGISTERED_MSG_TOKEN(    "UserNotRegisteredMessage"    );
const QLatin1String USER_UNREGISTERED_MSG_TOKEN(      "UserUnregisteredMessage"     );

const QLatin1String NO_GAME_TODAY_MSG_TOKEN(          "NoGameTodayMessage"          );
const QLatin1String UPDATE_INCREASE_MSG_TOKEN(        "UpdateIncreaseMessage"       );
const QLatin1String UPDATE_DECREASE_MSG_TOKEN(        "UpdateDecreaseMessage"       );

const QLatin1String TOP_MSG_TOKEN(                    "TopMessage"                  );
const QLatin1String TOP10_MSG_TOKEN(                  "TopTenMessage"               );

const QLatin1String HELP_MSG_TOKEN(                   "HelpMessage"                 );

const QLatin1String FAKE_DROP_MSG_TOKEN(              "DropReplyMessage"            );

// Message tokens, can be seen by bot administrator
const QLatin1String OK_MSG_TOKEN(                     "OkMessage"                   );

const QLatin1String UNKNOWN_COMMAND_MSG_TOKEN(        "UnknownCommandMessage"       );
const QLatin1String ERROR_OPENING_DB_MSG_TOKEN(       "ErrorOpeningDb"              );
const QLatin1String TOTAL_CHATS_COUNT_MSG_TOKEN(      "TotalChatsCountMessage"      );

/*
 *********************************************************************************************************************
 * Working with UI Strings file ( generating )
 */

bool UI::exampleFileGenerated(const QString& fileName)
{
    QFile outputFile(fileName);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Error opening file for writing "<<fileName;
        return false;
    }

    // Save default commands to file
    QJsonObject commandsJsonObject({
        { ROLL_CMD_TOKEN,    DefaultUI::rollCmd       },
        { UNROLL_CMD_TOKEN,  DefaultUI::unrollCmd     },
        { HELP_CMD_TOKEN,    DefaultUI::helpCmd       },
        { TOP_CMD_TOKEN,     DefaultUI::topCmd        },
        { TOP10_CMD_TOKEN,   DefaultUI::top10Cmd      },
        { DROP_CMD_TOKEN,    DefaultUI::fakeDropCmd   },

        { SEND_GLOBAL_MESSAGE_CMD_TOKEN,    DefaultUI::sendGlobalMessageCmd    },
        { GET_TOTAL_CHATS_CMD_TOKEN,        DefaultUI::getTotalChatsCountCmd   },
        { GET_VERSION_CMD_TOKEN,            DefaultUI::getVersionCmd           },
        { BACKUP_DATABASE_CMD_TOKEN,        DefaultUI::backupDatabaseCmd       },
        { SUSPEND_BOT_CMD_TOKEN,            DefaultUI::suspendBotCmd           },
        { RESUME_BOT_CMD_TOKEN,             DefaultUI::resumeBotCmd            }
    });

    // Save default UI messages to file
    QJsonObject messagesJsonObject({
        { ONLY_FOR_CHATS_MSG_TOKEN,         DefaultUI::onlyForChatsMessage          },
        { SUSPENDED_MSG_TOKEN,              DefaultUI::suspendedMessage             },
        { USER_REGISTERED_MSG_TOKEN,        DefaultUI::userRegisteredMessage        },
        { USER_NOT_REGISTERED_MSG_TOKEN,    DefaultUI::userNotRegisteredMessage     },
        { USER_UNREGISTERED_MSG_TOKEN,      DefaultUI::userUnregisteredMessage      },
        { NO_GAME_TODAY_MSG_TOKEN,          DefaultUI::noGameTodayMessage           },
        { UPDATE_INCREASE_MSG_TOKEN,        DefaultUI::updateIncreaseMessage        },
        { UPDATE_DECREASE_MSG_TOKEN,        DefaultUI::updateDecreaseMessage        },
        { TOP_MSG_TOKEN,                    DefaultUI::topMessage                   },
        { TOP10_MSG_TOKEN,                  DefaultUI::top10Message                 },
        { HELP_MSG_TOKEN,                   DefaultUI::helpMessage                  },
        { FAKE_DROP_MSG_TOKEN,              DefaultUI::fakeDropMessage              },
        { OK_MSG_TOKEN,                     DefaultUI::okMessage                    },
        { UNKNOWN_COMMAND_MSG_TOKEN,        DefaultUI::unknownCommandMessage        },
        { ERROR_OPENING_DB_MSG_TOKEN,       DefaultUI::errorOpeningDbMessage        },
        { TOTAL_CHATS_COUNT_MSG_TOKEN,      DefaultUI::totalChatsCountMessage       }
    });

    //Construct final json file
    QJsonObject resultingJsonObject({
        { "commands", commandsJsonObject },
        { "messages", messagesJsonObject }
    });

    QByteArray jsonBytes = QJsonDocument(resultingJsonObject).toJson();

    outputFile.write(jsonBytes);
    outputFile.flush();
    outputFile.close();

    return true;
}

/*
 *********************************************************************************************************************
 * Working with UI Strings file ( parsing )
 */

bool UI::fileLoaded(const QString& fileName)
{
    QFile file(fileName);

    if (!file.exists()) {
        qWarning() << "File "<<fileName<<" is not existing!";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error opening file "<<fileName;
        return false;
    }

    QJsonParseError jsonError;

    QJsonDocument d = QJsonDocument::fromJson(file.readAll(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing json";
        return false;
    }

    QJsonObject rootJsonObject = d.object();

    if (!rootJsonObject.contains("commands")) {
        qWarning() << "File "<<fileName<<" is a valid JSON file. Hovever, no \"commands\" object found";
        return false;
    }

    if (!rootJsonObject.contains("messages")) {
        qWarning() << "File "<<fileName<<" is a valid JSON file. Hovever, no \"messages\" object found";
        return false;
    }

    QJsonObject commandsJsonObject = rootJsonObject["commands"].toObject();
    m_rollCmd =         _loadValue(commandsJsonObject,ROLL_CMD_TOKEN,     DefaultUI::rollCmd);
    m_unrollCmd =       _loadValue(commandsJsonObject,UNROLL_CMD_TOKEN,   DefaultUI::unrollCmd);
    m_helpCmd =         _loadValue(commandsJsonObject,HELP_CMD_TOKEN,     DefaultUI::helpCmd);
    m_topCmd =          _loadValue(commandsJsonObject,TOP_CMD_TOKEN,      DefaultUI::topCmd);
    m_top10Cmd =        _loadValue(commandsJsonObject,TOP10_CMD_TOKEN,    DefaultUI::top10Cmd);
    m_fakeDropCmd =     _loadValue(commandsJsonObject,DROP_CMD_TOKEN,     DefaultUI::fakeDropCmd);

    m_sendGlobalMessageCmd =      _loadValue(commandsJsonObject,SEND_GLOBAL_MESSAGE_CMD_TOKEN,     DefaultUI::sendGlobalMessageCmd);
    m_getTotalChatsCountCmd =     _loadValue(commandsJsonObject,GET_TOTAL_CHATS_CMD_TOKEN,         DefaultUI::getTotalChatsCountCmd);
    m_getVersionCmd =             _loadValue(commandsJsonObject,GET_VERSION_CMD_TOKEN,             DefaultUI::getVersionCmd);
    m_backupDatabaseCmd =         _loadValue(commandsJsonObject,BACKUP_DATABASE_CMD_TOKEN,         DefaultUI::backupDatabaseCmd);
    m_suspendBotCmd =             _loadValue(commandsJsonObject,SUSPEND_BOT_CMD_TOKEN,             DefaultUI::suspendBotCmd);
    m_resumeBotCmd =              _loadValue(commandsJsonObject,RESUME_BOT_CMD_TOKEN,              DefaultUI::resumeBotCmd);

    QJsonObject messagesJsonObject = rootJsonObject["messages"].toObject();
    m_onlyForChatsMessage =       _loadValue(messagesJsonObject,ONLY_FOR_CHATS_MSG_TOKEN,          DefaultUI::onlyForChatsMessage);

    m_suspendedMessage =          _loadValue(messagesJsonObject,SUSPENDED_MSG_TOKEN,               DefaultUI::suspendedMessage);

    m_userRegisteredMessage =     _loadValue(messagesJsonObject,USER_REGISTERED_MSG_TOKEN,         DefaultUI::userRegisteredMessage);
    m_userNotRegisteredMessage =  _loadValue(messagesJsonObject,USER_NOT_REGISTERED_MSG_TOKEN,     DefaultUI::userNotRegisteredMessage);
    m_userUnregisteredMessage =   _loadValue(messagesJsonObject,USER_UNREGISTERED_MSG_TOKEN,       DefaultUI::userUnregisteredMessage);

    m_noGameTodayMessage =        _loadValue(messagesJsonObject,NO_GAME_TODAY_MSG_TOKEN,           DefaultUI::noGameTodayMessage);
    m_updateIncreaseMessage =     _loadValue(messagesJsonObject,UPDATE_INCREASE_MSG_TOKEN,         DefaultUI::updateIncreaseMessage);
    m_updateDecreaseMessage =     _loadValue(messagesJsonObject,UPDATE_DECREASE_MSG_TOKEN,         DefaultUI::updateDecreaseMessage);

    m_topMessage =                _loadValue(messagesJsonObject,TOP_MSG_TOKEN,                     DefaultUI::topMessage);
    m_topTenMessage =             _loadValue(messagesJsonObject,TOP10_MSG_TOKEN,                   DefaultUI::top10Message);

    m_helpMessage =               _loadValue(messagesJsonObject,HELP_MSG_TOKEN,                    DefaultUI::helpMessage);

    m_fakeDropReply =             _loadValue(messagesJsonObject,FAKE_DROP_MSG_TOKEN,               DefaultUI::fakeDropMessage);

    return true;
}

QString UI::_loadValue(const QJsonObject& sourceObject, const QString& valueName, const QString& defaultValue)
{
    if (sourceObject.contains(valueName))
        return sourceObject.value(valueName).toString();

    qWarning() << "No value "<<valueName<<" found. Using default one.";
    return defaultValue;
}


/*
 *********************************************************************************************************************
 * Static fields initialization
 */

// Command tokens (executable by user)
QString UI::m_rollCmd;
QString UI::m_unrollCmd;
QString UI::m_helpCmd;
QString UI::m_top10Cmd;
QString UI::m_topCmd;
QString UI::m_fakeDropCmd;

// Command tokens (executable by admin)
QString UI::m_sendGlobalMessageCmd;
QString UI::m_getTotalChatsCountCmd;
QString UI::m_getVersionCmd;
QString UI::m_backupDatabaseCmd;
QString UI::m_suspendBotCmd;
QString UI::m_resumeBotCmd;

// Messages
QString UI::m_onlyForChatsMessage;

// Messages tokens, can be seen by users in group chats
QString UI::m_suspendedMessage;

QString UI::m_userRegisteredMessage;
QString UI::m_userNotRegisteredMessage;
QString UI::m_userUnregisteredMessage;

QString UI::m_noGameTodayMessage;
QString UI::m_updateIncreaseMessage;
QString UI::m_updateDecreaseMessage;

QString UI::m_topMessage;
QString UI::m_topTenMessage;

QString UI::m_helpMessage;

QString UI::m_fakeDropReply;

// Message tokens, can be seen by bot administrator
QString UI::m_okMessage;
QString UI::m_unknownCommandMessage;
QString UI::m_errorOpeningDb;
QString UI::m_totalChatsCountMsg;
