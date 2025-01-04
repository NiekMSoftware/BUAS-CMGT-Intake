#pragma once

class Label : public GameObject
{
public:
	Label(const std::string& t = "", uint c = 0xFFFFFF);
	Label(const std::string& t = "", const float2& pos = { 10, 1 }, uint c = 0xffffff);
	void initialize() override;

	void setText(const std::string& newText) { text = newText; }
	const std::string& getText() const { return text; }

	void render(Surface* screen) override;

private:
	std::string text;
	uint color;
};
