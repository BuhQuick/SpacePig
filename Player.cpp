#include "Player.h"
#include <string>

using namespace spacePig;


Player::Player(int width, int height) :
    posx_(width / 2.0),
    posy_(height - (double(getDiameter()) + 10.0)),
    width_(width),
    height_(height) 
	{}

int Player::getX() const noexcept {
    return int(posx_ - radius_ + 0.5);
}

int Player::getY() const noexcept {
    return int(posy_ - radius_ + 0.5);
}

int Player::getDiameter() const noexcept {
	return int(2.0 * radius_ + 0.5);
}

std::string Player::getFileLoc() const noexcept {
    return fileLocation_;
}

void Player::move(std::string dir, double delta) noexcept {
    // input is left, check bounds against left side
    if (dir.compare("left") == 0 && (posx_ - radius_ >= 0)) {
	posx_ -= (velocity_ * delta);
    }

    // input is right, check bounds against right side
    if (dir.compare("right") == 0 && (posx_ + radius_ < width_)) {
	posx_ += (velocity_ * delta);
    }

    // input is up, check bounds against top of screen	
    if (dir.compare("up") == 0 && (posy_ - radius_ >= (velocity_ * delta))) {
	posy_ -= (velocity_ * delta);
    }

    // input is down, check bounds against bottom of screen
    if (dir.compare("down") == 0 && (posy_ + radius_ < height_)) {
	posy_ += (velocity_ * delta);
    }
}

void Player::move(int x, int y) noexcept {
    // ensure x-coordinate is on screen
    if (x - radius_ > 0 && x + radius_ < width_) {
	posx_ = x;
    }
    // ensure y-coordinate is on screen
    if (y - radius_ > 0 && y + radius_ < height_) {
	posy_ = y;
    }
}

bool Player::hasDied(const Wave& currWave) const noexcept {
    // all of the projectiles released for this wave so far
    std::vector<Projectile> onScreen = currWave.getReleased();

    // check for a collision against each projectile that has been released
    for (auto proj : onScreen) {
	double dx = proj.getX() - posx_;
	double dy = proj.getY() - posy_;
	double dist2 = dx * dx + dy * dy;
	if (dist2 < ((proj.getDiameter() / 2) + radius_) * ((proj.getDiameter() / 2) + radius_)) {
	    return true;
	}
    }
    return false;
}
