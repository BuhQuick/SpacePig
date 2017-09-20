#ifndef SPACEPIG_WAVE_H
#define SPACEPIG_WAVE_H

#include <set>
#include <memory>
#include <random>
#include "Projectile.h"

namespace spacePig {

/**
 * A wave stores projectiles for one "round" of play.
 * The projectiles are separated into those that are waiting
 * to be released, and those that have been released.
 * When there are no projectiles left in the wave, a new round
 * is ready to be started.
 */

class Wave {
public:
    /** Construct a wave. Because of the static wave id, we use no params */
    Wave();

    /**
     * All of the projectiles in this wave waiting to be released.
     * @return the projectiles waiting for release
     */
    std::vector<Projectile> getWaiting() const noexcept;

    /**
     * All of the projectiles that have been released and are not yet
     * off screen.
     * @return the projectiles that have been released
     */
    std::vector<Projectile> getReleased() const noexcept;

    /** The number of released projectiles so far 
     *
     * @return the number of projectiled that have been released
     */
    int getReleasedCount() const noexcept;

    /*
     * Get the wave number
     * @return the wave number
     */	
    int getWave() const noexcept;

    /**
     * resets the static wave count
     */
    void resetWaveCount() noexcept;

    /**
     * release a number of projectiles from the waiting vector.
     * if the count is greater than the number of waiting projectiles,
     * release all that are left.
     */	
    void release(/** number of projectiles to release */ int count = 1) noexcept;

    /**
     * move each projectile that has been released by velocity * delta
     */
    void onTick(/** time */ double delta = 0.01) noexcept;

private:
    /* static variable to store wave number */
    static int nextWave_;

    /* the wave number of this wave */	
    int wave_ = 0;

    /* vector of projectiles waiting to be released */
    std::vector<Projectile> waiting_;

    /* vector of projectiles that have been released */
    std::vector<Projectile> released_;

    /* a uniform int distribution for random numbers */
    std::uniform_int_distribution<unsigned int> dist_;

    /* mersenne twister engine for random numbers */
    std::mt19937 engine_;
};

}

#endif
