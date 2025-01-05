#include "precomp.h"
#include "Label.h"

Label::Label(const std::string& t, uint c)
	: text(t), color(c)
{
	setPosition(float2(0, 0));
}

Label::Label(const std::string& t, const float2& pos, uint c)
	: text(t), color(c)
{
	setPosition(pos);
	setScale(1.f);
}

void Label::initialize()
{
	name = "Label";
	setLayer(Layer::UserInterface);
}

void Label::render(Surface* screen)
{
	screen->Print(text.c_str(), static_cast<int>(position.x), static_cast<int>(position.y), color);
}