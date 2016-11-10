# pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"

#include "../Utilitys/Yumayo.h"
#include "../Effect/EffectBase.h"

namespace User
{
	class Combo
	{
		cinder::gl::Texture* backGround;
		EffectBase effect;
		Fonts font;
		int comboNumber;
		int maxComboNumber;
	public:
		Combo();
        int ComboNumber( ) { return comboNumber; }
		int MaxComboNumber() { return maxComboNumber; }
		void Update( );
        void PlusCombo( int attackSuccessNum );
        void ResetCombo( );
		void Draw(cinder::Vec2f position);
	};
}