// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#pragma once

namespace Tmpl8
{

// basic sprite class
class Sprite
{
public:
	// structors
	Sprite( Surface* surface, unsigned int frameCount );
	~Sprite();
	// methods
	void Draw( Surface* target, int x, int y );

	/* New Draw method, now taking a scale factor and rotation angles. */
	void Draw(Surface* target, int x, int y, float scale, float rotation);
	void DrawScaled( int x, int y, int width, int height, Surface* target );
	void SetFlags( unsigned int f ) { flags = f; }
	void SetFrame( unsigned int i ) { currentFrame = i; }
	unsigned int GetFlags() const { return flags; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	uint* GetBuffer() { return surface->pixels; }
	unsigned int Frames() { return numFrames; }
	Surface* GetSurface() { return surface; }
	void InitializeStartData();

	bool ownership;

private:
	// --------------------------------------------------
	// Note: Claude.ai helped me with this implementation.
	// 
	std::pair<int, int> CalculateBox(float scale, float rotation) const;
	std::pair<float, float> MapToSourceSpace(int destX, int destY, float cosTheta, float sinTheta, float invScale) const;
	bool IsSourceInBounds(float srcX, float srcY) const;
	void DrawPixel(Surface* target, int finalX, int finalY, uint color) const;
	uint SamplePixelBilinear(float srcX, float srcY);
	// 
	// --------------------------------------------------

private:
	// attributes
	int width, height;
	unsigned int numFrames;
	unsigned int currentFrame;
	unsigned int flags;
	unsigned int** start;
	Surface* surface;
};

}