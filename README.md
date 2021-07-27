# skribbl-client
## Messages (send to server)
Set your username. First thing you have to do.
```json
{
  "type": "username",
  "username": "s1mple"
}
```
Make a request to start the game. The server will notify you if the game has actually started.
```json
{
  "type": "startGame"
}
```
Notify others of the new point in the line you are drawing. You can only do this if you are the one drawing.
```json
{
  "type": "line",
  "x": 123,
  "y": 123,
  "r": 444,
  "g": 555,
  "b": 666,
  "a": 777
}
```
Notify others that the line has ended. You can only do this if you are the one drawing.
```json
{
  "type": "endLine"
}
```
Guess the word. You can only do this if you are the one guessing.
```json
{
  "type": "guess",
  "word": "ananas"
}
```

## Messages (receive from server)
A new list of players on the server.
```json
{
  "type": "usernameList",
  "usernames": [
    "s1mple",
    "tyler1",
    "dendi"
  ]
}
```
Notifies players that the game has started, what the word is and who is drawing.
```json
{
  "type": "gameStarted",
  "word": "brontosaurus",
  "drawer": "s1mple"
}
```
Notifies others of the new point in the line being drawn.
```json
{
  "type": "line",
  "x": 123,
  "y": 123,
  "r": 444,
  "g": 555,
  "b": 666,
  "a": 777
}
```
Notifies others that the line has ended.
```json
{
  "type": "endLine"
}
```
Notifies everyone who guessed what incorrectly.
```json
{
  "type": "incorrectGuess",
  "username": "tyler1",
  "word": "nosorog"
}
```
Notifies everyone who guessed correctly and what.
```json
{
  "type": "correctGuess",
  "username": "tyler1",
  "word": "nosorog"
}
```
Notifies everyone that the game was aborted (because someone disconnected and not enough players remain) and sends the new username list.
```json
{
  "type": "gameAborted",
  "usernames": [
    "s1mple",
    "tyler1"
  ]
}
```
