#ifndef SPACEPIG_PROJECTILE_H
#define SPACEPIG_PROJECTILE_H

#include <random>
#include <string>

namespace spacePig {
/**
 * Represents one single projectile.
 * Each projectile has an (x, y) position and a speed that
 * is determined randomly based on the wave the projectile 
 * belongs to. 
 *
 * If a player hits a projectile, they die. 
 */

class Projectile {
public:

    Projectile(/** Random number generator */ std::mt19937& engine,
		/** wave of the projectile */ int wave,
		/** width of the screen */ int width = 450,
		/** height of the screen */ int height = 800);

    /**
     * The x coordinate of the center of the projectile
     * @return the x coordinate of the projectile.
     */
    int getX() const noexcept;

    /**
     * The y coordinate of the center of the projectile
     * @return the y coordinate of the projectile.
     */
    int getY() const noexcept;

    /**
     * The diameter of the projectile
     * @return the diameter of the projectile.
     */
    int getDiameter() const noexcept;    

    /**
     * The file location of the projectile image.
     * @return the location of the projectile image
     */
    std::string getFileLoc() const noexcept;

    /**
     * Determines whether or not the projectile has fallen off
     * the bottom of the screen and needs to be removed.
     */
    bool offScreen() const noexcept;

    /**
     * Move the projectile over delta time
     */
    void move(/** The interval of time during which the sprite moves. */ double delta) noexcept; 

    /**
     * set the projectile from waiting to being released. 
     * Projectiles are released as the wave progresses.
     */
    void allowMove() noexcept;

private:
    /** a real distribution for RNG */
    std::uniform_real_distribution<double> realDistribution_;

    /** the radius of the projectile image */
    double radius_ = 5.0;

    /** width of the game display */
    int width_ = 450;

    /** height of the game display */
    int height_ = 800;

    /** x-coordinate of this projectile */
    double posx_ = 0.0;

    /** y-coordinate of this projectile */
    double posy_ = -10.0;

    /** x velocity of this projectile */
    double vx_ = 0.0;

    /** y velocity of this projectile */
    double vy_ = 1.0;

    /** whether or not this projectile can move */
    bool canMove_ = false;

    /** whether or not this projectile is on screen */
    bool offScreen_ = false;

    /** the file location of the projectile image */
    std::string fileLocation_ = "graphics/projectile.png";
};

}

#endif 
