/**
* 11.09.11 Merge Sort
* Author: teaprog
*/

#include <SDL.h>
#include <SDL_opengl.h>

#include <vector>
using std::vector;

int screen_w = 1800;
int screen_h = 950;

bool quit = false;

SDL_Event event;

const int POLE_NUMBER = 1000;

class Pole;

void init();
void reshape(int, int);
void display();
void drawAndInputHandle();
void shuffle();

void mergeSort(Pole [], int);
void mergeSort(vector<Pole*>, int, int);
void merge(vector<Pole*>, vector<Pole*>);

class Pole {
	private:
		GLfloat xPos;
		GLfloat yPos;
		GLfloat height;
		GLfloat width;

		static int poleCounter;
	public:
		Pole() {
			GLfloat spaceBetween = 0.0f;
			width = (screen_w - 20.0f - POLE_NUMBER * spaceBetween) / (POLE_NUMBER);
			changeHeight();

			xPos = 10.0f + poleCounter * (width + spaceBetween);
			yPos = screen_h - 100.0f;

			poleCounter++;
		}

		void changeHeight() {
			height = (GLfloat)(rand() % (screen_h - 200));
		}

		void setHeight(GLfloat height) {
			this->height = height;
		}

		GLfloat getHeight() {
			return height;
		}

		void drawPole() {
			glColor3f(0.0f, 0.0f, 1.0f);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(xPos, yPos);
			glVertex2f(xPos + width, yPos);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(xPos + width, yPos - height);
			glVertex2f(xPos, yPos - height);
			glEnd();
		}
} pole[POLE_NUMBER];

int Pole::poleCounter = 0;

void mergeSort(Pole pole[], int arrayLength) {
	vector<Pole*> pole1;
	vector<Pole*> pole2;

	for (int i = 0; i < arrayLength / 2; i++) {
		pole1.push_back(&pole[i]);
	}
	for (int i = arrayLength / 2; i < arrayLength; i++) {
		pole2.push_back(&pole[i]);
	}

	mergeSort(pole1, 0, arrayLength / 2);
	mergeSort(pole2, arrayLength / 2, arrayLength);

	merge(pole1, pole2);
}

void mergeSort(vector<Pole*> poles, int from, int to) {
	if (to - from < 2 | quit)
		return;

	vector<Pole*> pole1;
	vector<Pole*> pole2;

	for (int i = from; i < (to + from) / 2; i++) {
		pole1.push_back(&pole[i]);
	}
	for (int i = (to + from) / 2; i < to; i++) {
		pole2.push_back(&pole[i]);
	}

	mergeSort(pole1, from, (to + from) / 2);
	mergeSort(pole2, (to + from) / 2, to);

	merge(pole1, pole2);
}

void merge(vector<Pole*> pole1, vector<Pole*> pole2) {
	if (quit)
		return;

	vector<GLfloat> heights;

	int index1 = 0;
	int index2 = 0;

	while (index1 < pole1.size() && index2 < pole2.size()) {
		if (pole1.at(index1)->getHeight() < pole2.at(index2)->getHeight()) {
			heights.push_back(pole1.at(index1++)->getHeight());
		}
		else {
			heights.push_back(pole2.at(index2++)->getHeight());
		}
	}

	while (index1 < pole1.size()) {
		heights.push_back(pole1.at(index1++)->getHeight());
	}

	while (index2 < pole2.size()) {
		heights.push_back(pole2.at(index2++)->getHeight());
	}

	for (int i = 0; i < pole1.size(); i++) {
		pole1.at(i)->setHeight(heights.at(i));
		drawAndInputHandle();
	}

	for (int i = 0; i < pole2.size(); i++) {
		pole2.at(i)->setHeight(heights.at(i + pole1.size()));
		drawAndInputHandle();
	}

	
}

void drawAndInputHandle() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				return;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
					return;
				}
				else if (event.key.keysym.sym == SDLK_SPACE)
					shuffle();
				break;
			case SDL_VIDEORESIZE:
				reshape(event.resize.w, event.resize.h);
				break;
			default:
				break;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	for (int i = 0; i < POLE_NUMBER; i++)
		pole[i].drawPole();

	SDL_GL_SwapBuffers();
}

void shuffle() {
	for (int i = 0; i < POLE_NUMBER; i++) {
		pole[i].changeHeight();
	}

	mergeSort(pole, POLE_NUMBER);
}

void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_WM_SetCaption("11.09.11 Merge Sort", 0);
	SDL_putenv("SDL_VIDEO_CENTERED=center");
	
	reshape(screen_w, screen_h);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void reshape(int screenW, int screenH) {
	screen_w = screenW;
	screen_h = screenH;

	SDL_SetVideoMode(screenW, screenH, 32, SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0, 0, screenW, screenH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, screen_w, screen_h, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	for (int i = 0; i < POLE_NUMBER; i++)
		pole[i].drawPole();

	SDL_GL_SwapBuffers();
}

int main(int argc, char** argv) {
	init();

	mergeSort(pole, POLE_NUMBER);

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						quit = true;
					else if (event.key.keysym.sym == SDLK_SPACE)
						shuffle();
					break;
				case SDL_VIDEORESIZE:
					reshape(event.resize.w, event.resize.h);
					break;
				default:
					break;
			}
		}

		display();
	}

	SDL_Quit();
	return 0;
}