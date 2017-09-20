#include <vector>
#include <random>
#include <stdexcept>
#include "Projectile.h"

using namespace std;
using namespace spacePig;

Projectile::Projectile(std::mt19937& engine, int wave, int width, int height) :
    realDistribution_(0.0, 1.0),

    width_(width),
    height_(height),

    posx_(realDistribution_(engine) * width_),
    posy_(-10.0),
    vy_{realDistribution_(engine) * 500.00 + 150.0}

    {
	int rnd = int(realDistribution_(engine) * 10);
	if (rnd % 2 == 0) {
	    vx_ = realDistribution_(engine) * 600.00 + (wave * 50.0);
	}
	else {
	    vx_ = -realDistribution_(engine) * 600.00 + (wave * 50.0);
	}
}

int Projectile::getX() const noexcept {
    return int(posx_ - radius_ + 0.5);
}

int Projectile::getY() const noexcept {
    return int(posy_ - radius_ + 0.5);
}

int Projectile::getDiameter() const noexcept {
    return int(2.0 * radius_ + 0.5);
}

std::string Projectile::getFileLoc() const noexcept {
    return fileLocation_;
}

bool Projectile::offScreen() const noexcept {
    return offScreen_;
}

void Projectile::move(double delta) noexcept {
    if (canMove_) {
	posx_ += delta * vx_;
	posy_ += delta * vy_;
	// Bounce against walls
	if (posx_ < radius_) {
	    posx_ = 2 * radius_ - posx_;
	    vx_ = -vx_;
	}

	if (posx_ > width_ - radius_) {
	    posx_ = 2 * (width_ - radius_) - posx_;
	    vx_ = -vx_;
        }
    }

    if (posy_ - radius_ > height_) {
	offScreen_ = true;
    }
}

void Projectile::allowMove() noexcept {
    canMove_ = true;
}
