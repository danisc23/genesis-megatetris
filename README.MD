# GENESIS Megatetris

This is a text-character-based Tetris clone developed using SGDK, for Genesis/Megadrive (that's why the horrible name).

![Title Screen](title_screen.png)

## How to Play

1. Download the rom from [out/rom.bin](out/rom.bin).
2. Open the game in a compatible Megadrive emulator such as Kgens.

## Controls

- **UP:** Drop Tetromino
- **DOWN:** Move Down Tetromino
- **LEFT & RIGHT:** Move Tetromino Horizontally
- **A & C:** Rotate Counter-clockwise
- **B:** Rotate Clockwise
- **START:** Pause

![Gameplay](gameplay.gif)

## How to Build

1. Install [SGDK](https://github.com/Stephane-D/SGDK/wiki/SGDK-Installation)
2. Run `%SGDK%\bin\make -f %SGDK%\makelib.gen` at the root of the project. (Replace `%SGDK%` with the path to your SGDK installation)

## Contributing

Feel free to contribute by forking the repository and submitting pull requests. Suggestions, bug fixes, and improvements are welcome!

Also not everything has to be done in code! you can also help by creating sound effects and music for the game.

#### Improvements
- **Center of Rotation Check:** Verify the center of rotation for Z and S tetrominoes and adjust accordingly.
- **Manual Down Movement:** The automatic downward movement counter should be reset when the player moves down.
- **Pre-Solidification Movement:** To improve the feel of the game, the player should be able to move the tetromino left/right after it has solidified, this is related with the previous item.
- **Hold Down for Faster Descent:** Implement faster downward movement when the player holds down (can reuse the code for left/right movement).
- **Don't stop movement when rotating:** When holding left/right, the tetromino should continue moving after rotating. currently it stops due it's current quick implementation. Should be easy to fix by checking when a player releases the button using `state` in `joyPlaying`.

#### Bugs
- **Game stops detecting lines:** I don't know how to reproduce it, but it happens after a long play session. I think it's related to the game grid overflowing, but I'm not sure.
- **I Piece Game Over:** As the previous item, this only happens after a long play session. When the I piece is spawned, the game ends inmediately, Probably related to it's upper initial position.

#### Code
- `drawMainMenuPointer`: This function applies some game logic between it's drawing code, it should be separated.
- `drawNextTetromino`: This function draws the area where the next tetromino will be displayed and the next tetromino itself. It should be separated in two functions to keep single responsibility.
- `drawNextTetromino`: Also it's doing a monkey patch to show as centered as posible I and T tetrominoes, maybe we can find a better way to do this.
- `triggerSelectedOptionOnCondition`: This function is used to trigger the selected option in the main menu, but also is drawing (if needed) the performed action. Also I'm not sure about the `OnCondition` part.
- `prepareNextState`: Some of this code should be moved to a draw function.
- `main`: Should be using constants for the game states.

Along all these TODOs, there are some other things that can be improved, like the code style, the variable names, etc. This is the first project I do in C, so I'm sure there are a lot of things that can be improved. Also this began as a quick project to learn SGDK having everything in a single file, so the scope of a lot of variables and functions should be refactored, that also would help to avoid circular header dependencies.

Also probably I'm not using the best practices for the Genesis CPU and VDP management, so any help in that area would be appreciated.

## License

This project is licensed under the ![License](https://img.shields.io/static/v1?label=license&message=CC-BY-4.0&color=green) - see the [LICENSE](LICENSE) file for details.
TL;DR: You can use this project for non-commercial purposes as long as you give credit to the original author.

## Acknowledgments
- The current music is a YM2612 version of the [Sonic Adventure 2 - Hero Garden](https://www.youtube.com/watch?v=pevjY2Wb8zM) theme, made by [JXChip](https://www.youtube.com/@JXChip).