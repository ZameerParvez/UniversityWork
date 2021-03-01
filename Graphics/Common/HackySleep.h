#pragma once

#include <thread>
#include <chrono>

// This is a hacky workaround for a bug where the opengl window does not refresh
static void hackySleeper() {
	static int counter = 2;
	if (counter == 0) return;
	--counter;
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

// this will be used, so that the function can be removed more easily
#define HACKYSLEEPER hackySleeper();