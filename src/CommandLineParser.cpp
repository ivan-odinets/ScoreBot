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

#include "CommandLineParser.h"

CommandLineParser::CommandLineParser() :
    m_configFileOption(      QStringList{ "c", "config"}),
    m_preserveConfig(        QStringList{ "p", "preserve-config" }),

    m_botTokenOption(        QStringList{ "t", "token"}),
    m_botAdminOption(        QStringList{ "a", "admin"}),

    m_dbFileOption(          QStringList{ "d", "database"}),

    m_printTimeZonesOption(  QStringList{ "z", "available-timezones" }),
    m_timeZoneOption(        QStringList{ "s", "set-timezone"}),

    m_stringsFileOption(     QStringList{ "u", "strings" }),
    m_newStringsFileOption(  QStringList{ "g", "generate-strings"} )
{
    // -h/ --help
    addHelpOption();
    // -v/ --version
    addVersionOption();

    // -c/ --config
    m_configFileOption.setDescription(tr("Specify configuration <file> to use."));
    m_configFileOption.setValueName("file");
    m_configFileOption.setDefaultValue("");
    addOption(m_configFileOption);

    // -p/ --preserve-config
    m_preserveConfig.setDescription(tr("Do not change configuration file."));
    addOption(m_preserveConfig);

    // -t/ --token
    m_botTokenOption.setDescription(tr("Specify Telegram Bot API <token> to use."));
    m_botTokenOption.setValueName("token");
    m_botTokenOption.setDefaultValue("");
    addOption(m_botTokenOption);

    // -a/ --admin
    m_botAdminOption.setDescription(tr("Set bot administrator <user_id>."));
    m_botAdminOption.setValueName("user_id");
    m_botAdminOption.setDefaultValue("");
    addOption(m_botAdminOption);

    // -d/ --database
    m_dbFileOption.setDescription(tr("Specify database <file> to use."));
    m_dbFileOption.setValueName("file");
    m_dbFileOption.setDefaultValue("");
    addOption(m_dbFileOption);

    //   / --available-timezones
    m_printTimeZonesOption.setDescription(tr("Print available timezones and exit"));
    addOption(m_printTimeZonesOption);

    // -s/ --set-timezone
    m_timeZoneOption.setDescription(tr("Specify <timzone> which bot should use."));
    m_timeZoneOption.setValueName("timezone");
    addOption(m_timeZoneOption);

    // -u/ --strings
    m_stringsFileOption.setDescription("Specify <file> containing UI strings, which bot should show to users.");
    m_stringsFileOption.setValueName("file");
    addOption(m_stringsFileOption);

    // -g/ --
    m_newStringsFileOption.setDescription(tr("Generate default UI strings <file> and exit."));
    m_newStringsFileOption.setValueName("file");
    addOption(m_newStringsFileOption);
}
