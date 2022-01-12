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

#include "CommandLineParser.h"

#include <QDir>

CommandLineParser::CommandLineParser() :
    m_dbFileOption(QStringList() << "d" << "database",QCoreApplication::translate("main","Set database file.")),
    m_botTokenOption(QStringList() << "t" << "token",QCoreApplication::translate("main", "Set bot token.")),
    m_botAdminOption(QStringList() << "a" << "admin",QCoreApplication::translate("main", "Set bot administrator userId.")),
    m_setTimeZoneOption(QStringList() << "s" << "set-timezone",QCoreApplication::translate("main","Set timzone for bot")),
    m_showTimeZonesOption(QStringList() << "z" <<"available-timezones",QCoreApplication::translate("main","Show available timezones"))
{
    // -h/ --help
    addHelpOption();
    // -v/ --version
    addVersionOption();

    // -d/ --database
    m_dbFileOption.setValueName("database");
    m_dbFileOption.setDefaultValue("database.db");
    addOption(m_dbFileOption);

    // -t/ --token
    m_botTokenOption.setValueName("token");
    m_botTokenOption.setDefaultValue("");
    addOption(m_botTokenOption);

    // -a/ --admin
    m_botAdminOption.setValueName("admin");
    m_botAdminOption.setDefaultValue("-1");
    addOption(m_botAdminOption);

    // -s// --set-timezone
    m_setTimeZoneOption.setValueName("timezone");
    addOption(m_setTimeZoneOption);

    // // --available-timezones
    addOption(m_showTimeZonesOption);
}
