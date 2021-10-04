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
#include "StringDefines.h"

ScoreBot*      bot       = nullptr;
Database*      sqliteDb  = nullptr;

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);

    //Parse command line
    CommandLineParser parser;
    parser.process(app);

    if (parser.token().isEmpty()) {
        qCritical() << __FILE__ << ":" << __LINE__ << ". API Token must be specified.";
        return -1;
    }

    //Check SQLITE driver
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        qCritical() << __FILE__ << ":" << __LINE__ << ". Unable to load database - needs the Qt sqlite driver.";
        return -1;
    }

    //Setup database
    sqliteDb = new Database;
    sqliteDb->setFileName(parser.dbFile());
    if (!sqliteDb->databaseInitialized()) {
        qCritical() << __FILE__ << ":" << __LINE__ << ". Some error in initializing database. Check previous messages.";
        return -1;
    }

    //Setup connection to telegram API
    bot = new ScoreBot;
    bot->setApiKey(parser.token());
    bot->setDatabase(sqliteDb);

    //Start Bot
    if (!bot->started()) {
        qCritical() << __FILE__ << ":" << __LINE__ << ". Can not start bot due to some errors.";
        return -1;
    }

    return app.exec();
}
