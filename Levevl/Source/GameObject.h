#pragma once

class GameObject {
public:
	GameObject(int x = 0, int y = 0, const char* textureFileName = "Assets/brick.png");
	~GameObject();

	void Update();
	void Render();

private:

	int xpos;
	int ypos;

	SDL_Texture* objectTexture;
	SDL_Rect sourceRect, destinationRect;
};