#pragma once
class Wall : public GameObject
{
public:
	Wall();
	Wall(int w, int h);

private:
	void initialize();

private:
	int width, height;
};

