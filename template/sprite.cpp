// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"

using namespace Tmpl8;

// constructor
Sprite::Sprite( Surface* surface, unsigned int frameCount ) :
	width( surface->width / frameCount ),
	height( surface->height ),
	numFrames( frameCount ),
	currentFrame( 0 ),
	flags( 0 ),
	start( new unsigned int* [frameCount] ),
	surface( surface ),
	ownership(surface->ownBuffer)
{
	InitializeStartData();
}

// destructor
Sprite::~Sprite()
{
	if (ownership)
		delete surface;

	for (unsigned int i = 0; i < numFrames; i++) delete start[i];
	delete[] start;
}

// draw sprite to target surface
void Sprite::Draw( Surface* target, int x, int y )
{
	if (x < -width || x >( target->width + width )) return;
	if (y < -height || y >( target->height + height )) return;
	int x1 = x, x2 = x + width;
	int y1 = y, y2 = y + height;
	uint* src = GetBuffer() + currentFrame * width;
	if (x1 < 0) src += -x1, x1 = 0;
	if (x2 > target->width) x2 = target->width;
	if (y1 < 0) src += -y1 * width * numFrames, y1 = 0;
	if (y2 > target->height) y2 = target->height;
	uint* dest = target->pixels;
	int xs;
	if (x2 > x1 && y2 > y1)
	{
		unsigned int addr = y1 * target->width + x1;
		const int w = x2 - x1;
		const int h = y2 - y1;
		for (int j = 0; j < h; j++)
		{
			const int line = j + (y1 - y);
			const int lsx = start[currentFrame][line] + x;
			xs = (lsx > x1) ? lsx - x1 : 0;
			for (int i = xs; i < w; i++)
			{
				const uint c1 = *(src + i);
				if (c1 & 0xffffff) *(dest + addr + i) = c1;
			}
			addr += target->width;
			src += width * numFrames;
		}
	}
}

void Sprite::Draw(Surface* target, int x, int y, float scale, float rotation)
{
	if (width == 0 || height == 0) return;

	// Precompute trigonometric values
	float radians = rotation * (PI / 180.0f);
	float cosTheta = cos(radians), sinTheta = sin(radians);
	float invScale = 1.0f / scale;

	// Calculate bounding box
	auto [bboxWidth, bboxHeight] = CalculateBox(scale, rotation);

	// Compute center
	int destCenterX = (int)(x + width * 0.5f * scale);
	int destCenterY = (int)(y + height * 0.5f * scale);

	// Iterate over the bounding box
	for (int destX = -bboxWidth / 2; destX < bboxWidth / 2; destX++)
	{
		for (int destY = -bboxHeight / 2; destY < bboxHeight / 2; destY++)
		{
			// Map to source space
			auto [srcX, srcY] = MapToSourceSpace(destX, destY, cosTheta, sinTheta, invScale);

			// Check if source coordinates are in bounds
			if (!IsSourceInBounds(srcX, srcY)) continue;

			// Sample pixel color
			uint color = SamplePixelBilinear(srcX, srcY);
			if ((color & 0xffffff) == 0) continue;

			// Draw the pixel to the target
			int finalX = destCenterX + destX;
			int finalY = destCenterY + destY;
			DrawPixel(target, finalX, finalY, color);
		}
	}
}

// draw scaled sprite
void Sprite::DrawScaled( int x1, int y1, int w, int h, Surface* target )
{
	if (width == 0 || height == 0) return;
	for (int x = 0; x < w; x++) for (int y = 0; y < h; y++)
	{
		int u = (int)((float)x * ((float)width / (float)w));
		int v = (int)((float)y * ((float)height / (float)h));
		uint color = GetBuffer()[u + v * width * numFrames];
		if (color & 0xffffff) target->pixels[x1 + x + ((y1 + y) * target->width)] = color;
	}
}

uint Sprite::SamplePixelBilinear(float srcX, float srcY)
{
	// Get integer and fractional parts of the source coordinates
	int x = (int)srcX;
	int y = (int)srcY;
	float fx = srcX - x;
	float fy = srcY - y;

	// Ensure source coordinates are within bounds
	if (x < 0 || y < 0 || x >= width - 1 || y >= height - 1)
		return 0; // Transparent or background color

	// Get neighboring pixels
	uint c00 = GetBuffer()[x + y * width * numFrames];
	uint c10 = GetBuffer()[(x + 1) + y * width * numFrames];
	uint c01 = GetBuffer()[x + (y + 1) * width * numFrames];
	uint c11 = GetBuffer()[(x + 1) + (y + 1) * width * numFrames];

	// Interpolate between the pixels
	uint r = (uint)(
		(1 - fx) * (1 - fy) * ((c00 >> 16) & 0xFF) +
		fx * (1 - fy) * ((c10 >> 16) & 0xFF) +
		(1 - fx) * fy * ((c01 >> 16) & 0xFF) +
		fx * fy * ((c11 >> 16) & 0xFF));
	uint g = (uint)(
		(1 - fx) * (1 - fy) * ((c00 >> 8) & 0xFF) +
		fx * (1 - fy) * ((c10 >> 8) & 0xFF) +
		(1 - fx) * fy * ((c01 >> 8) & 0xFF) +
		fx * fy * ((c11 >> 8) & 0xFF));
	uint b = (uint)(
		(1 - fx) * (1 - fy) * (c00 & 0xFF) +
		fx * (1 - fy) * (c10 & 0xFF) +
		(1 - fx) * fy * (c01 & 0xFF) +
		fx * fy * (c11 & 0xFF));

	return (r << 16) | (g << 8) | b; // Return the interpolated color
}

// prepare sprite outline data for faster rendering
void Sprite::InitializeStartData()
{
	for (unsigned int f = 0; f < numFrames; ++f)
	{
		start[f] = new unsigned int[height];
		for (int y = 0; y < height; ++y)
		{
			start[f][y] = width;
			uint* addr = GetBuffer() + f * width + y * width * numFrames;
			for (int x = 0; x < width; ++x) if (addr[x])
			{
				start[f][y] = x;
				break;
			}
		}
	}
}

std::pair<int, int> Sprite::CalculateBox(float scale, float rotation) const
{
	float radians = rotation * (PI / 180.0f);
	float cosTheta = cos(radians), sinTheta = sin(radians);

	float halfWidth = width * 0.5f * scale;
	float halfHeight = height * 0.5f * scale;

	float rotatedWidth = abs(cosTheta * halfWidth) + abs(sinTheta * halfHeight);
	float rotatedHeight = abs(sinTheta * halfWidth) + abs(cosTheta * halfHeight);

	int bboxWidth = (int)(rotatedWidth * 2 + 1.5f); // Add slight padding
	int bboxHeight = (int)(rotatedHeight * 2 + 1.5f);

	return { bboxWidth, bboxHeight };
}

std::pair<float, float> Sprite::MapToSourceSpace(int destX, int destY, float cosTheta, float sinTheta,
	float invScale) const
{
	float srcX = (cosTheta * destX + sinTheta * destY) * invScale + width * 0.5f;
	float srcY = (-sinTheta * destX + cosTheta * destY) * invScale + height * 0.5f;

	return { srcX, srcY };
}

bool Sprite::IsSourceInBounds(float srcX, float srcY) const
{
	return srcX >= 0 && srcY >= 0 && srcX < width && srcY < height;
}

void Sprite::DrawPixel(Surface* target, int finalX, int finalY, uint color) const
{
	if (finalX < 0 || finalY < 0 || finalX >= target->width || finalY >= target->height)
		return;

	target->pixels[finalX + finalY * target->width] = color;
}
