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
	void DrawScaled( int x, int y, int width, int height, Surface* target );
	
	void SetFlags( unsigned int f ) { flags = f; }
	void SetFrame( unsigned int i ) { currentFrame = i; }
	void SetOrigin(float x, float y) { originX = x, originY = y; }
	
	unsigned int GetFlags() const { return flags; }
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	uint* GetBuffer() { return surface->pixels; }
	Surface* GetSurface() { return surface; }
	float GetOriginX() { return originX; }
	float GetOriginY() { return originY; }

	unsigned int Frames() { return numFrames; }
	void InitializeStartData();

public:
	bool ownership = true;

	float originX = 0.f;
	float originY = 0.f;

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