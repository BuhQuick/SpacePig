#include "Wave.h"
#include <chrono>
using namespace std;
using namespace spacePig;

int Wave::nextWave_ = 1;

Wave::Wave() {
    // seed the engine with a time seed
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    engine_.seed(seed);
    wave_ = ++nextWave_;

    std::uniform_int_distribution<unsigned int> dist(1, wave_);
    dist_ = dist;

    int val = dist_(engine_) * wave_;

    // create wave count to wave count squared projectiles for the wave
    for (int ii = 0; ii < val; ii++) {
	waiting_.push_back(Projectile(engine_, wave_));
    } 

}

std::vector<Projectile> Wave::getWaiting() const noexcept {
    return waiting_;
}

std::vector<Projectile> Wave::getReleased() const noexcept {
    return released_;
}

int Wave::getReleasedCount() const noexcept {
    return released_.size();
}

int Wave::getWave() const noexcept {
	return wave_;
}

void Wave::release(int count) noexcept {
	// ensure no out of range errors
	if (count > waiting_.size()) {
		count = waiting_.size();
	}

        // schedule projectiles for release
	vector<Projectile>::iterator iter = waiting_.begin();
	for (int ii = 0; ii < count; ii++) {
		waiting_.at(0).allowMove();
		released_.push_back(waiting_.at(0));
		waiting_.erase(iter);
	}
}

void Wave::resetWaveCount() noexcept {
	nextWave_ = 0;
}

void Wave::onTick(double delta) noexcept {
    // handle any projectiles that have exited the screen area
    for (vector<Projectile>::iterator iter = released_.begin(); iter != released_.end();) {
	if (iter->offScreen()) {
	    iter = released_.erase(iter);
	}
	else {
	    iter++;
	}
    }

    // move any remaining projectiles
    for (int ii = 0; ii < released_.size();ii++) {
	   released_.at(ii).move(delta);
    }

}
