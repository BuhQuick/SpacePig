#include <ctime>
#include <iostream>
#include <stdexcept>

#include "Display.h"

using namespace std;
using namespace spacePig;

/**
 * @namespace spacePig This is my final project for CS3520.
 * It is a simple wave-based "bullet-hell" esque game. Although,
 * it is not very bullet-helly in classic fashion. Each wave, more
 * projectiles are released. The releases are timed so that the entire
 * Wave does not spawn at once. The player can move with the keyboard,
 * or enable mouse movement and play in that manner.
 *
 * @author Nicholas Buquicchio
 */

/**
 * Main program to get the game running.
 * Ensures that the program exits if the user has closed
 * the window.
 *
 * @return The status code. Status code 0 means
 * the program succeeds, and nonzero status code
 * means the program failed.
 */
int main() {
    try {
	Player player(450, 800);

	// Initialize the game display.
	GameDisplay* display = new GameDisplay(player, 450, 800);
	
	// loop forever so the display remains open.
	// If the display is closed, we can exit the program.
	for (;;) {
	    display->runGame();
	    if (display->wasClosed()) {
		return 0;
	    }
	}
    } 
    catch (const exception& e) {
	cerr << e.what() << endl;
	return 1;
    }

return 0;
}
