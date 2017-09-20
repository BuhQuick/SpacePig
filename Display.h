#ifndef SPACEPIG_DISPLAY_H
#define SPACEPIG_DISPLAY_H

#include <vector>
#include "Player.h"
#include "Wave.h"
#include "Projectile.h"

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

namespace spacePig {

/**
 * Extension of Kenneth Baclawski's SDL display.
 * Encapsulates SDL functionality into one single class.
 * Supports certain game mechanics
 */
class GameDisplay {
public:
    /**
     * Create a game display with specified size and a player
     * @throw domain_error if the display could not
     * be created and initialized.
     */
    GameDisplay(/** Player for game */ Player player,
        /** Display width. */ int width = 450,
	    /** Display height. */ int height = 800);

    /**
     * Destruct the game display.  This closes
     * the display.
     */
     ~GameDisplay();

    /**
    * Close the graphical display and release the
    * resources.
    */
    void close() noexcept;

    /**
     * whether or not the window has been closed.
     * If the display is closed, stop running the game.
     * @return true if the game was closed
     */
    bool wasClosed() const noexcept;

    /**
     * begin the next wave and release a projectile
     */
    void startNextWave() noexcept;   
    
    /**
     * handles user events including keyboard and mouse use.
     */
    void checkForKeyEvent() noexcept;

    /**
     * plays through the game. Responsible for moving projectiles,
     * checking for player movement, changes in the game state... 
     * The hub for activity.
     */	
    void runGame() noexcept;

    /**
     * Refresh the display.
     * @throw domain_error if the display could not
     * be refreshed.
     */
    void refresh();

    /**
     * What wave the game is on
     * @return the wave number
     */
    int getWaveCount() const noexcept;
private:
    /** The display window. */
    SDL_Window* window_ = nullptr;

    /** The display rendering tool. */
    SDL_Renderer* renderer_ = nullptr;

    /** The collection of images. */
    std::vector<SDL_Texture*> images_;

    /** The width of the window. */
    const int width_ = 0;

    /** The height of the window. */
    const int height_ = 0;

    /** Whether or not the display was closed */
    bool wasClosed_ = false;

    /** Whether or not the player can move using the mouse */
    bool allowMouseMovement_ = false;

    /** The wave the player is on */
    Wave wave_;

    /** The wave number the player is on */
    int waveCount_ = 0; 

    /** The player for the game */
    Player player_;

    /**
     * Add an image to the collection.
     */
    void addImage(/** The location of the file. */
		const std::string& fileLocation) noexcept;

    /**
     * Clear the background to opaque white.
     */
    void clearBackground();
};

}

#endif
