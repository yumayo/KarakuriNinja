# pragma once

# include "EffectBase.h"

# include "../Utilitys/Yumayo.h"

namespace User
{
    // Effect�͉摜�̍������_�Ƃ��ĕ\������܂��B
    class EffectScore : public EffectBase
    {
        float alpha = 0.0F;
        int score;
        Fonts font;
    public:
        EffectScore( cinder::Vec2f position, int scoreNum );
    protected:
        virtual void Update( ) override;
        virtual void Draw( ) override;
    };
}