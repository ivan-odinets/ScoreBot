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

#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QSettings>

#include <QNetworkProxy>

#include "CommandLineParser.h"

class AppConfig
{
public:
    AppConfig(const QString& settingsFile);
    ~AppConfig();

    void setPreserveConfig(bool state)           { m_preserveConfig = state; }

    void updateConfigFile(const CommandLineParser& parser);

    QSettings::Status   status() const;

    QString             token() const            { return m_botToken; }
    void                setToken(const QString& newToken);

    Telegram::User::Id  botAdmin() const         { return m_botAdmin; }
    void                setBotAdmin(Telegram::User::Id newAdminId);

    QString             databaseFile() const     { return m_databaseFile; }
    void                setDatabaseFile(const QString& newFileName);

    QString             stringsFile() const      { return m_stringsFile; }
    void                setStringsFile(const QString& newFileName);

    QString             timeZone() const         { return m_timeZone; }
    void                setTimeZone(const QString& newTimeZone);

    bool                validProxyConfigured() const;
    QNetworkProxy       proxy() const            { return m_proxy; }

private:
    Q_DISABLE_COPY(AppConfig);
    QSettings*     p_settings;
    void           _loadValues();

    bool                m_preserveConfig;
    QString             m_botToken;
    Telegram::User::Id  m_botAdmin;
    QString             m_databaseFile;
    QString             m_stringsFile;
    QString             m_timeZone;
    QNetworkProxy       m_proxy;
};

#endif // APPCONFIG_H
