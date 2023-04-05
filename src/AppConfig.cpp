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

#include "AppConfig.h"

#include <QDebug>
#include <QFile>
#include <QSettings>

#define BOT_TOKEN       "token"
#define BOT_ADMIN       "admin"

#define DATABASE_FILE   "database"
#define STRINGS_FILE    "strings"

#define TIMEZONE        "timezone"

AppConfig::AppConfig(const QString& fileName) :
    p_settings(nullptr)
{
    p_settings = (fileName.isEmpty()) ? new QSettings
            : new QSettings(fileName,QSettings::IniFormat);

    qDebug() << "Using settings file "<<p_settings->fileName();

    _loadValues();
}

AppConfig::~AppConfig()
{
    p_settings->sync();
    delete p_settings;
}

void AppConfig::_loadValues()
{
    m_botToken          = p_settings->value(BOT_TOKEN).toString();
    m_botAdmin          = p_settings->value(BOT_ADMIN).toString().toDouble();
    m_databaseFile      = p_settings->value(DATABASE_FILE).toString();
    m_stringsFile       = p_settings->value(STRINGS_FILE).toString();
    m_timeZone          = p_settings->value(TIMEZONE).toString();
}

void AppConfig::updateConfigFile(const CommandLineParser& parser)
{
    if (parser.botAdmin() != 0)             setBotAdmin(parser.botAdmin());
    if (!parser.token().isEmpty())          setToken(parser.token());
    if (!parser.dbFile().isEmpty())         setDatabaseFile(parser.dbFile());
    if (!parser.timeZone().isEmpty())       setTimeZone(parser.timeZone());
    if (!parser.stringsFile().isEmpty())    setStringsFile(parser.stringsFile());
}

QSettings::Status AppConfig::status() const
{
    return p_settings->status();
}

void AppConfig::setToken(const QString& newToken)
{
    m_botToken = newToken;
    if (!m_preserveConfig)   p_settings->setValue(BOT_TOKEN,newToken);
}

void AppConfig::setBotAdmin(Telegram::User::Id newAdminId)
{
    m_botAdmin = newAdminId;
    if (!m_preserveConfig)   p_settings->setValue(BOT_ADMIN,newAdminId);
}

void AppConfig::setDatabaseFile(const QString& newFileName)
{
    m_databaseFile = newFileName;
    if (!m_preserveConfig)   p_settings->setValue(DATABASE_FILE,newFileName);
}

void AppConfig::setStringsFile(const QString& newFileName)
{
    m_stringsFile = newFileName;
    if (!m_preserveConfig)   p_settings->setValue(STRINGS_FILE,newFileName);
}

void AppConfig::setTimeZone(const QString& newTimeZone)
{
    m_timeZone = newTimeZone;
    if (!m_preserveConfig)   p_settings->setValue(TIMEZONE,newTimeZone);
}
