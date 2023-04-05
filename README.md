# ScoreBot

ScoreBot - is a very simple "game" bot for Telegram group chats.

You can play in this game each day by entering /roll in the group chat. After this your score increases/decreases by random value.
Top scores: /top
Exit the game (progress will be saved): /unroll.

Bot is not working in private chats, only in groups.

## Compiling & usage

First of all - create bot in Telegram. Check telegram docs [here](https://core.telegram.org/bots#6-botfather).

For  Debian:
```
git clone git@github.com:ivan-odinets/ScoreBot.git
cd ScoreBot
git submodule init
git submodule update
sudo  apt-get  install  qtbase5-dev  qtbase5-dev-tools  qt5-qmake  libqt5core5a  libqt5network5 libqt5sql5
debuild  -b  -uc  -us
```

## Customization

If you want to use your custom bot replies - edit file ui-messages.json and feed edited version to the bot (using command line option -u / --strings ).

## License
[GNU General Public License v3.0](https://choosealicense.com/licenses/gpl-3.0/)
