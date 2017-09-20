#ifndef SPACEPIG_PLAYER_H
#define SPACEPIG_PLAYER_H

#include "Wave.h"

namespace spacePig {

/*
 * A Player is the object the person playing the game has control over.
 * The player has a set velocity it can move by using arrow keys, or can
 * be moved with the cursor. If the player is hit by any of the projectiles
 * in the wave, they lose.
 */

class Player {
public:

    /* Constructs a player. The player must know the bounds
     * of the window so that it does not go off screen.
     */
    Player(/** width of screen */ int width = 450, 
	/** height of screen */ int height = 800);

    /*
     * The x coordinate of the player's location.
     * @return the x-coordinate
     */
    int getX() const noexcept;

    /*
     * The y coordinate of the player's location.
     * @return the y-coordinate
     */
    int getY() const noexcept;

    /*
     * The diameter of the player's character, as defined by 2 * it's radius.
     * @return the diameter of the player's character
     */
    int getDiameter() const noexcept;
	
    /*
     * The string file location storing where the player's image is.
     * @return the file location of the player's image
     */
    std::string getFileLoc() const noexcept;

    /*
     * Moves the player in a certain direction over delta time.
     * Ensures the player is not moving beyond the boundaries of the game.
     */
    void move(/** which side to move player */ std::string dir, 
		/** time */ double delta = 0.01) noexcept;

    /*
     * Moves the player to the given x and y coordinates.
     * Ensures the player is not moving beyond the boundaries of the game.
     */
    void move(/** x-coord destination */ int x, 
		/** y-coord destination */ int y) noexcept; 

    /*
     * Check if the player has been hit by any of the projectiles
     * in the current wave. If the player has been hit, the game is over 
     * and they must restart.
     * @return true if the player has died
     */
    bool hasDied(/** Current wave for game*/ const Wave& currWave) const noexcept;
	
private:
    /** The player's velocity */
    double velocity_ = 750.0;

    /** The player's x-coordinate */
    double posx_ = 0.0;

    /** The player's y-coordinate */
    double posy_ = 0.0;

    /** The radius of the player's character */
    int radius_ = 10;

    /** The width of the game's screen */
    int width_ = 0;

    /** The height of the game's screen */
    int height_ = 0;

    /** Where the player's image is located */
    std::string fileLocation_ = "graphics/player.png";
};
}

#endif 
