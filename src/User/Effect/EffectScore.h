# pragma once

# include "EffectBase.h"

# include "../Utilitys/Yumayo.h"

namespace User
{
    // Effect‚Í‰æ‘œ‚Ì¶ã‚ğŠî“_‚Æ‚µ‚Ä•\¦‚³‚ê‚Ü‚·B
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