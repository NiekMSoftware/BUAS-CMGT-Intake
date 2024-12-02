#include "precomp.h"
#include "resourceholder.h"

ResourceHolder::~ResourceHolder()
{

}

void ResourceHolder::Clean()
{

}

ResourceHolder& ResourceHolder::Instance()
{
	static ResourceHolder s;
	return s;
}

bool ResourceHolder::LoadSprite(const char* fileName, const std::string& id, int numFrames)
{
	return false;
}