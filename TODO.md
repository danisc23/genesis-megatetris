# TODO

#### Improvements
- **Center of Rotation Check:** Verify the center of rotation for Z and S tetrominoes and adjust accordingly.
- **Manual Down Movement:** The automatic downward movement counter should be reset when the player moves down.
- **Pre-Solidification Movement:** To improve the feel of the game, the player should be able to move the tetromino left/right after it has solidified, this is related with the previous item.
- **Hold Down for Faster Descent:** Implement faster downward movement when the player holds down (can reuse the code for left/right movement).
- **Don't stop movement when rotating:** When holding left/right, the tetromino should continue moving after rotating. currently it stops due it's current quick implementation. Should be easy to fix by checking when a player releases the button using `state` in `joyPlaying`.

#### Bugs
- **Game unpauses while holding any button:** When the game is paused while the player is holding any button the game unpauses when releasing START.

#### Code
- `drawNextTetromino`: Doing a monkey patch to show as centered as posible I and T tetrominoes, maybe we can find a better way to do this.
- `triggerSelectedOptionOnCondition`: This function is used to trigger the selected option in the main menu, but also is drawing (if needed) the performed action. Also I'm not sure about the `OnCondition` part.

#### Other
 If you want to see the bugs that have been fixed, check the [RELEASES](https://github.com/danisc23/genesis-megatetris/releases) section.