#include "MojiManager.h"

#include "Hirasawa.h"

namespace User
{
	using namespace cinder;

	MojiManager::MojiManager()
		: font(u8"HG�s����", 128)
	{


	}
	void MojiManager::ReCall(std::string const & jsonPath)
	{
		if (IsEnded())
		{
			Setup(jsonPath);
		}
	}
	void MojiManager::Setup(std::string const & jsonPath)
	{
		frame = 0;
		isEnd = false;
		clearCount = 60;
		auto json = JsonTree(app::loadAsset(jsonPath));

		auto jsonItr = json["Moji"].begin();

		std::vector<std::string> mojitati;

		if (jsonPath == "JSON/GameStart.json")
		{
			mojitati.emplace_back(u8"�E");
			mojitati.emplace_back(u8"��");
			mojitati.emplace_back(u8"�J");
			mojitati.emplace_back(u8"�n");
		}
		else if (jsonPath == "JSON/GameClear.json")
		{
			mojitati.emplace_back(u8"�E");
			mojitati.emplace_back(u8"��");
			mojitati.emplace_back(u8"��");
			mojitati.emplace_back(u8"��");
		}
		else if (jsonPath == "JSON/GameOver.json")
		{
			mojitati.emplace_back(u8"�E");
			mojitati.emplace_back(u8"��");
			mojitati.emplace_back(u8"��");
			mojitati.emplace_back(u8"�s");
		}
		else
		{
			mojitati.emplace_back(u8"�h");
		}

		auto mojiItr = mojitati.begin();

		while (true)
		{
			auto rand = *jsonItr;
			Moji::Data data();
			mojiList.push_back(Moji(Moji::Data(*jsonItr, *mojiItr)));

			jsonItr = ++jsonItr;
			if (jsonItr == json["Moji"].end()) break;
			mojiItr = ++mojiItr;
			if (mojiItr == mojitati.end()) break;
		}
	}
	void MojiManager::End()
	{
		isEnd = true;
	}
	bool MojiManager::IsEnded()
	{
		return clearCount == 0;
	}
	void MojiManager::Update()
	{
		frame += 1;

		if (isEnd)
		{
			clearCount = std::max(clearCount - 1, 0);
			if (clearCount == 0)
			{
				mojiList.clear();
				isEnd = false;
			}
		}
	}
	void MojiManager::Draw(cinder::Vec2f position, float mojiWidth)
	{
		position.y = EasingElasticInOut(static_cast<float>(60 - clearCount) / 60, position.y, -mojiWidth);

		auto size = mojiList.size();

		gl::pushModelView();
		gl::translate(position.x - mojiWidth * static_cast<float>(size) / 2.0F, position.y - mojiWidth / 2.0F);
		gl::translate(mojiWidth / 2.0F, mojiWidth / 2.0F);
		for (auto& obj : mojiList)
		{
			obj.Draw(frame, font);
			gl::translate(mojiWidth, 0);
		}
		gl::popModelView();
	}
}