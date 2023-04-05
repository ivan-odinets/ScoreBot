SUBDIRS += src/ScoreBot.pro

TEMPLATE = subdirs
CONFIG += ordered warn_on qt debug_and_release 

DISTFILES += README.md \
    debian/score-bot.dirs \
    debian/score-bot.install \
    debian/score-bot.conf \
    debian/score-bot.service \
    debian/ui-messages.json
