# TODO

#### Features
- **Two Player Mode:** Add a two player mode, where the players can compete against each other.
- **Drop Ghost:** Add a drop ghost to show where the piece will land.

#### Improvements
- **Center of Rotation Check:** Verify the center of rotation for Z and S tetrominoes and adjust accordingly.
- **Pre-Solidification Movement:** To improve the feel of the game, the player should be able to move the tetromino left/right after it has solidified, this is related with the previous item.
- **Save Game:** Save hiscores and settings in a save file.
- **Options Menu:** Add an options menu instead of having all the options in the main menu.

#### Bugs
- **Game over when a piece is solidified in y 0:** When a piece is solidified in the top row, the games ends, but should not unless the piece is above the game grid.

#### Code
- `drawNextTetromino`: Doing a monkey patch to show as centered as posible I and T tetrominoes, maybe we can find a better way to do this.

#### Other
 If you want to see the bugs that have been fixed, check the [RELEASES](https://github.com/danisc23/genesis-megatetris/releases) section.