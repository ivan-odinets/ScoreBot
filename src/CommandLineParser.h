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

#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>

#include "QtTelegramBot/src/types/user.h"

class CommandLineParser : public QCommandLineParser
{
    Q_DECLARE_TR_FUNCTIONS(CommandLineParser);
public:
    CommandLineParser();
    ~CommandLineParser() {}

    QString   configFile() const            { return value(m_configFileOption); }
    bool      preserveConfig() const        { return isSet(m_preserveConfig); }

    QString             token() const       { return value(m_botTokenOption); }
    Telegram::User::Id  botAdmin() const    { return value(m_botAdminOption).toLongLong(); }

    QString   dbFile() const                { return value(m_dbFileOption); }

    bool      printTimeZones() const         { return isSet(m_printTimeZonesOption); }
    QString   timeZone() const              { return value(m_timeZoneOption); }

    QString   stringsFile() const           { return value(m_stringsFileOption); }
    QString   newStringsFile() const        { return value(m_newStringsFileOption); }

private:
    QCommandLineOption m_configFileOption;
    QCommandLineOption m_preserveConfig;

    QCommandLineOption m_botTokenOption;
    QCommandLineOption m_botAdminOption;

    QCommandLineOption m_dbFileOption;

    QCommandLineOption m_printTimeZonesOption;
    QCommandLineOption m_timeZoneOption;

    QCommandLineOption m_stringsFileOption;
    QCommandLineOption m_newStringsFileOption;
};

#endif // COMMANDLINEPARSER_H
