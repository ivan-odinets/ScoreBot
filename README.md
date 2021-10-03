# ScoreBot

ScoreBot - is a very simple "game" bot for Telegram group chats.

You can play in this game each day by entering /roll in the group chat. After this your score increases/decreases by random value.
Top scores: /top
Exit the game (progress will be saved): /unroll.

Bot is not working in private chats, only in groups.

## Compiling & usage

First of all - create bot in Telegram. Check telegram docs [here](https://core.telegram.org/bots#6-botfather).

```bash
qmake
make
./ScoreBot -t API_TOKEN -d /path/to/database/file 
```

## License
[MIT](https://choosealicense.com/licenses/mit/)
