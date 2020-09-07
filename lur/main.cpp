#include <SDL.h>
#include <GL/glew.h>
#include <thread>
#include <memory>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

#include "Scene.hpp"

void update(lur::Scene** scene, bool& runnable);

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { return -1; }

	SDL_Window* window = SDL_CreateWindow("Lur", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window) { return -2; }

	SDL_GLContext context = SDL_GL_CreateContext(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) { return -3; }

	ImGuiContext* imcontext = ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(window, &context);
	ImGui_ImplOpenGL3_Init();

	lur::Scene* currentScene = new lur::Scene();
	bool runnable = true;

	std::thread uth(update, &currentScene, runnable);
	while (runnable) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) { runnable = false; }
		}
		glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		currentScene->draw();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		currentScene->drawGui();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glFlush();
		SDL_GL_SwapWindow(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext(imcontext);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}


void update(lur::Scene** scene, bool& runnable) {
	unsigned int lastUpdate = clock();
	while (runnable) {
		auto t = clock() - lastUpdate;
		lastUpdate = clock();
		(*scene)->tick(t);
	}
}