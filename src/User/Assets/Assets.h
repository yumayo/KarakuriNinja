#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/imageIo.h"
#include <map>
#include <vector>
#include <fstream>
#include "../Utilitys/Nomoto.h"
#include "cinder/Json.h"

//! @file Objectを管理してKeyで関連つけしたいときに使ってください
//! @brief 一括で読み込みしたいときに使用してください
//! @note 
//!        
//! @date 2016-07-29
//! @author Taka Nomoto

namespace Izanami
{
	template <typename T>
	class Holder
	{
		std::map<std::string, T> objects;

	public:

		Holder() = default;

		//Obejctの追加
		void add(std::string path, T object)
		{
			auto itr = objects.find(path);
			if (itr != objects.end()) return;
			objects.insert(std::make_pair(path, object));
		}

		//検索
		T* find(std::string path)
		{
			auto itr = objects.find(path);
			return (itr != objects.end()) ? &itr->second : nullptr;
		}

		//要素削除
		void clear()
		{
			objects.clear();
		}
	};

	class FileReader
	{
	public:

		FileReader();

		~FileReader();

		void make(std::string path);

		std::map<std::string, std::string> members();

		void clear();

	private:

		std::map<std::string, std::string> strings;
	};

	class TextureMaker
	{
	public:
		ci::gl::Texture Find(std::string path);

	private:
		ci::gl::Texture tex;
	};
}