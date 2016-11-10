#include "Assets.h"

namespace Izanami
{
	FileReader::FileReader()
	{

	}
	FileReader::~FileReader()
	{

	}

	void FileReader::make(std::string path)
	{
		clear();

		ci::JsonTree json(ci::app::loadAsset(path.c_str()));

		for (auto& object : json)
		{
			strings.insert(std::make_pair(object.getKey(), object.getValue<std::string>()));
		}
	}

	std::map<std::string, std::string> FileReader::members()
	{
		return strings;
	}

	void FileReader::clear()
	{
		if (strings.size() != 0)
			strings.clear();
	}


	ci::gl::Texture TextureMaker::Find(std::string path)
	{
		tex = ci::loadImage(GetFileFullPath(path.c_str()));
		return tex;
	}
}