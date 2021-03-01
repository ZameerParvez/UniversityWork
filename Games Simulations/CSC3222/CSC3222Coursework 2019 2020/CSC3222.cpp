#include "../../Common/Window.h"
#include "BadFoodGame.h"

#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"
#include "../../Common/Matrix4.h"

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

using namespace NCL;
using namespace CSC3222;

int main() {
	Window*w = Window::CreateGameWindow("CSC3222 - B4DF00D!", 1080, 720);

	if (!w->HasInitialised()) {
		return -1;
	}

	//Matrix4 test = Matrix4::Orthographic(-1, 1, 800, 0, 0, 600);
	
	BadFoodGame* theGame = new BadFoodGame();
	w->GetTimer()->GetTimeDeltaSeconds(); //Reset so we don't get a big dt in the first frame!
	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		
		HACKYSLEEPER

		float time = w->GetTimer()->GetTimeDeltaSeconds();

		theGame->Update(time);

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::HOME)) {
			w->SetFullScreen(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::END)) {
			w->SetFullScreen(false);
		}

		w->SetTitle(std::to_string(time));
	}

	delete theGame;

	Window::DestroyGameWindow();
}