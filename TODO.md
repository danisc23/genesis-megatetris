# TODO

#### Improvements
- **Center of Rotation Check:** Verify the center of rotation for Z and S tetrominoes and adjust accordingly.
- **Pre-Solidification Movement:** To improve the feel of the game, the player should be able to move the tetromino left/right after it has solidified, this is related with the previous item.

#### Bugs
- **Game over when a piece is solidified in y 0:** When a piece is solidified in the top row, the games ends, but should not unless the piece is above the game grid.

#### Code
- `drawNextTetromino`: Doing a monkey patch to show as centered as posible I and T tetrominoes, maybe we can find a better way to do this.
- `triggerSelectedOptionOnCondition`: This function is used to trigger the selected option in the main menu, but also is drawing (if needed) the performed action. Also I'm not sure about the `OnCondition` part.

#### Other
 If you want to see the bugs that have been fixed, check the [RELEASES](https://github.com/danisc23/genesis-megatetris/releases) section.