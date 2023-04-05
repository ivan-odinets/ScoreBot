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

#include <QCoreApplication>

#include <cstdlib>
#include <time.h>

#include <QDebug>
#include <QSqlDatabase>

#include "CommandLineParser.h"
#include "Database.h"
#include "ScoreBot.h"
#include "AppConfig.h"
#include "UI.h"

#include <QTimeZone>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    std::srand(std::time(nullptr));

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setOrganizationName("OdinSoft");
    QCoreApplication::setApplicationVersion(APP_VERSION);

    // Check for SQLITE driver
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        qCritical() << "Qt SQLITE driver required for this bot to work.";
        return -1;
    }

    // Parse command line and do some fast jobs (if requested)
    CommandLineParser parser;
    parser.process(app);

    if (parser.printTimeZones()) {
        qInfo() << "Available timezones:";
        foreach (QByteArray id, QTimeZone::availableTimeZoneIds())
            qInfo() << QTimeZone(id);
        return 0;
    }

    if (!parser.newStringsFile().isEmpty()) {
        if (UI::exampleFileGenerated(parser.newStringsFile()))
            qInfo() << "Sucessfuly generated file "<<parser.newStringsFile();
        else
            qWarning() << "Error generating file "<<parser.newStringsFile();
        return 0;
    }

    AppConfig settings(parser.configFile());

    // Update config file if some extra options were passed through command line
    settings.updateConfigFile(parser);

    // Check if everything, which is needed is configured
    if (settings.token().isEmpty()) {
        qCritical() << "Telegram BotAPI Token must be specified.";
        return -1;
    }

    //Setup database
    Database sqliteDb;
    sqliteDb.setFileName(settings.databaseFile());
    if (!sqliteDb.databaseInitialized()) {
        qCritical() << "Error in initializing database. Check previous messages.";
        return -1;
    }

    //Setup connection to telegram API
    ScoreBot bot;
    bot.setApiKey(settings.token());
    bot.setDatabase(&sqliteDb);

    //Configure UI
    if (!settings.stringsFile().isEmpty()) {
        UI::fileLoaded(settings.stringsFile());
    }

    if (settings.botAdmin() == 0)
        qWarning() << "Bot administrator is not set, so administration commands will not work!";
    else
        bot.setBotAdmin(settings.botAdmin());

    QTimeZone botTimeZone;
    if (settings.timeZone().isEmpty())
        qInfo() << "Timezone not specified, using UTC as default one.";
    else
        botTimeZone = QTimeZone(parser.timeZone().toStdString().c_str());

    if (!botTimeZone.isValid())
        qWarning() << "Timezone "<<settings.timeZone()<<" is invalid, using UTC as default one.";
    else
        qInfo() << "Setting timezone for the bot: "<<botTimeZone;


    bot.setTimeZone(botTimeZone);

    //Start Bot
    if (!bot.started()) {
        qCritical() << "Can not start bot due to some errors.";
        return -1;
    }

    return app.exec();
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    Q_UNUSED(context);
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "[DEBUG]   : %s ( %s )\n", localMsg.constData(), function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "[INFO]    : %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "[WARNING] : %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "[CRITICAL]: %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "[FATAL]   : %s\n", localMsg.constData());
        break;
    }
}
