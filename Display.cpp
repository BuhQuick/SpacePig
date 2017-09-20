#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include <iostream>

#include "Display.h"

using namespace std;
using namespace spacePig;

GameDisplay::GameDisplay(Player player, int width, int height)
  : player_(player), width_(width), height_(height) {

    // Initialize SDL2

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    	throw domain_error(string("SDL Initialization failed due to: ") + SDL_GetError());
    }

    // Construct the screen window

    window_ = SDL_CreateWindow("Bubble Blaster", SDL_WINDOWPOS_UNDEFINED, 
                             SDL_WINDOWPOS_UNDEFINED, 
                             width_, height_, SDL_WINDOW_SHOWN);
    if (!window_) {
    	close();
    	throw domain_error(string("Unable to create the window due to: ") + SDL_GetError());
    }

    // Construct the renderer

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
    	close();
    	throw domain_error(string("Unable to create the renderer due to: ") + SDL_GetError());
    }
    // add all necessary images
    addImage("graphics/scene.jpg");
    addImage(player_.getFileLoc());
    for (auto proj : wave_.getWaiting()) {
	addImage(proj.getFileLoc());
	break;
    }
    // Clear the window

    clearBackground();
}

GameDisplay::~GameDisplay() {
    close();
}

void GameDisplay::close() noexcept {

    // Delete the SDL2 resources in reverse order of
    // their construction, starting with the images

    for (SDL_Texture* image : images_) {
    	if (image) {
      	SDL_DestroyTexture(image);
        }
    }

    // Clear the collection of images to ensure
    // idempotence

    images_.clear();

    // Destroy the renderer and window, and set the
    // variables to nullptr to ensure idempotence

    if (renderer_) {
    	SDL_DestroyRenderer(renderer_);
    	renderer_ = nullptr;
    }

    if (window_) {
    	SDL_DestroyWindow(window_);
    	window_ = nullptr;
    }

    // The last step is to quit SDL
    wasClosed_ = true;
    SDL_Quit();
}

bool GameDisplay::wasClosed() const noexcept {
    return wasClosed_;
}

void GameDisplay::startNextWave() noexcept {
    // begin a new wave and release one projectile
    Wave* nextWave = new Wave();
    wave_ = *nextWave;
    wave_.release();
}

void GameDisplay::addImage(const string& fileLocation) noexcept {
    if (renderer_) {

    // Load the image from the file

    SDL_Surface* imageSurface = IMG_Load(fileLocation.c_str());
    if (imageSurface) {

        // Convert the image to a texture

      	SDL_Texture* imageTexture = 
	    SDL_CreateTextureFromSurface(renderer_, imageSurface);
      	if (imageTexture) {

	// Add the image to the collection

        images_.push_back(imageTexture);
        } 
	else {
            cerr << "Unable to load the image file at " << fileLocation
             << " due to: " << SDL_GetError() << endl;
        } 

        // The surface is not longer needed

        SDL_FreeSurface(imageSurface);
    } 
    else {
     	 cerr << "Unable to load the image file at " << fileLocation
           << " due to: " << SDL_GetError() << endl;
    }
  }
}

void GameDisplay::checkForKeyEvent() noexcept {

    // Remove all events from the queue

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
	// handle a quit event by closing window
	if (event.type == SDL_QUIT) {
	    close();
	    break;
	  }
	/* allow all key events if player is alive
 	 * left key: move player to the left
	 * right key: move player to the right
	 * up key: move player up
	 * down key: move player down
	 * e key: allow mouse movement
	 * r key: restart the game
 	 * x key: close the window
  	 */
	else if (event.type == SDL_KEYDOWN && !player_.hasDied(wave_)) {
	    switch (event.key.keysym.sym) {
		case SDLK_LEFT:
		    player_.move("left");
		    break;
		case SDLK_RIGHT:
		    player_.move("right");
		    break;
		case SDLK_UP:
		    player_.move("up");
		    break;
		case SDLK_DOWN:
		    player_.move("down");
		    break;
		case SDLK_e:
		    allowMouseMovement_ = !allowMouseMovement_;
		    break;
		case SDLK_r:
		    wave_.resetWaveCount();
		    startNextWave();
		    refresh();
		    break;
	 	case SDLK_x:
		    close();
		    break;
		default: break;
	    }
	}
	// if the player has died, restrict key handling
	// to game reset and closing the window
	else if (event.type == SDL_KEYDOWN) {
	    switch (event.key.keysym.sym) {
		case SDLK_r:
		    wave_.resetWaveCount();
		    startNextWave();
		    refresh();
		    break;
		case SDLK_x:
		    close();
		    break;
		    default: break;
	    }
	}
	
	// allow the player to move by mouse
	else if (event.type == SDL_MOUSEMOTION && allowMouseMovement_) {
	    int x;
	    int y;
	    // obtain mouse coordinates
	    SDL_GetMouseState(&x, &y);	  
	    player_.move(x, y);
	}
    }
}

void GameDisplay::runGame() noexcept {
    unsigned int oldTime = SDL_GetTicks();
    unsigned int currentTime;

    /** Handle game start. Wait for player to begin game */	
    while(wave_.getReleasedCount() == 0) {
	    if (wasClosed_) {
	        break;
	    }
	    checkForKeyEvent();
	    refresh();
    }

    /** 
     * Handle a game in progress. Check for key events, refresh screen,
     * progress game.
     */
    while(!(player_.hasDied(wave_)) && wave_.getReleasedCount() != 0) {
	    if (wasClosed_) {
	        break;
	    }
	    checkForKeyEvent();
	    refresh();
	    wave_.onTick();
	
	    int upTo = 0;
	    int count = wave_.getWaiting().size();
	    /** Fire off a projectile every .15 seconds */
		while (upTo != count) {
	        currentTime = SDL_GetTicks();
			if (currentTime - oldTime >= 150) {
				wave_.release();
				oldTime = currentTime;
				upTo++;
			}
			wave_.onTick();
			refresh();
			checkForKeyEvent();
			// when player dies, stop game
			if (player_.hasDied(wave_)) {
				break;
			}
		}
    }

    /*
     * Handle player being dead. Disable mouse movement and wait
     * for player to restart or exit game
     */
    while (player_.hasDied(wave_)) {
		allowMouseMovement_ = false;
		if (wasClosed_) {
			break;
		}
		checkForKeyEvent();
		refresh();
	}
    oldTime = SDL_GetTicks();
    /* 
     * We are now inbetween waves. Give the player 2.5 seconds
     * to reposition / get ready for the next wave.
     */
    if (wave_.getReleasedCount() == 0 && !player_.hasDied(wave_)) {
		currentTime = SDL_GetTicks();
		// Prevent a wave from spawning for 2.5 seconds
		while (currentTime - oldTime <= 2500) {
			currentTime = SDL_GetTicks();
			checkForKeyEvent();
			refresh();
		}
		oldTime = currentTime;
		startNextWave();
    }
}

void GameDisplay::refresh() {
    //cout << "Refreshing sprites..." << endl;
    if (renderer_) {

    // Clear the window
    
    clearBackground();

    // The location of the sprite is a square

    SDL_Rect destination = { 0, 0, width_, height_ };

    // Get the image index and check that it is valid

    unsigned int imageIndex = 0;
    if (imageIndex >= 0 && imageIndex < images_.size()) {
	    // Get the image for the sprite
            SDL_Texture* imageTexture = images_.at(imageIndex);
            if (imageTexture) {

	    // Render the image at the location,
	    // rotated by its angle

                if (SDL_RenderCopyEx(renderer_, imageTexture, nullptr,
                               &destination, 0, 
                               nullptr, SDL_FLIP_NONE) != 0) {
            	    close();
            	    throw domain_error(string("Unable to render a sprite due to: ")
                               + SDL_GetError());
	    	}
	    } 
	    else {
	    	close();
            	throw domain_error("Missing image texture at index ");          
            }
    } 
    else {
	close();
        throw domain_error("Invalid image index " );
    }
	
    // Draw all of the sprites

    for (const auto& proj : wave_.getReleased()) {

        // The location of the sprite is a square

        SDL_Rect destination = { proj.getX(), proj.getY(), 
                               proj.getDiameter(), proj.getDiameter() };

        // Get the image index and check that it is valid
        if (imageIndex >= 0 && imageIndex < images_.size()) {

	    // Get the image for the sprite
            SDL_Texture* imageTexture = images_.at(2);
            if (imageTexture) {

	  	// Render the image at the location,
	  	// rotated by its angle

          	if (SDL_RenderCopyEx(renderer_, imageTexture, nullptr,
                               &destination, 0, 
                               nullptr, SDL_FLIP_NONE) != 0) {
            	    close();
            	    throw domain_error(string("Unable to render a sprite due to: ")
                               + SDL_GetError());
          	}
	    } 
	    else {
	  	close();
          	throw domain_error("Missing image texture at index ");          
            }
	} 
	else {
	    close();
            throw domain_error("Invalid image index " );
        }
    }
	
    // The location of the sprite is a square

    SDL_Rect destinationP = { player_.getX(), player_.getY(), 
                               player_.getDiameter(), player_.getDiameter() };


    // Get the image for the sprite

    SDL_Texture* imageTextureP = images_.at(1);
    if (imageTextureP) {

	// Render the image at the location,
	// rotated by its angle

        if (SDL_RenderCopyEx(renderer_, imageTextureP, nullptr,
                               &destinationP, 0, 
                               nullptr, SDL_FLIP_NONE) != 0) {
	    close();
            throw domain_error(string("Unable to render a sprite due to: ")
                               + SDL_GetError());
        }
    } 
    else {
	close();
        throw domain_error("Missing image texture at index ");          
    }
	
    SDL_RenderPresent(renderer_);
  }
}

void GameDisplay::clearBackground() {
    if (renderer_) {
    
    // Clear the window to opaque white
    
	if (SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff) != 0) {
     	    close();
      	    throw domain_error(string("Unable to set the background color due to: ")
                         + SDL_GetError());
    	}
    	if (SDL_RenderClear(renderer_) != 0) {
      	    close();
      	    throw domain_error(string("Unable to set the background color due to: ")
                         + SDL_GetError());
    	}
    }
}

int GameDisplay::getWaveCount() const noexcept {
    return wave_.getWave();
}
