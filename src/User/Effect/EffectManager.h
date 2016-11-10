# pragma once

# include "EffectBase.h"
# include "EffectAlpha.h"
# include "EffectCombo.h"

# include <list>
# include <functional>

namespace User
{
    using EffectList = std::list<EffectBaseRef>;

    class EffectManager : public EffectActor
    {
        EffectList effectList;
    public:
        EffectManager( );
        void Update( ) override;
        void Draw( ) override;
    public:
        void EffectRegister( EffectList& effectList );
    private: // �ȉ��A�b�v�f�[�g�ɋL��
        void EffectEraser( );
    private:
        void Each( std::function<void( EffectBaseRef& )> function );
    };

    using EffectManagerRef = std::shared_ptr<EffectManager>;
}