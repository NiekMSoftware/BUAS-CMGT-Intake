#pragma once
class Item : public GameObject
{
public:
	explicit Item(float w, float h);

protected:
	void initialize();

protected:
	float width, height;
};

