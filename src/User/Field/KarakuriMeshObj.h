# pragma once

# include "KarakuriActor.h"

# include "../Utilitys/ObjRender.h"

namespace User
{
    class KarakuriMeshObj : public KarakuriActor
    {
        ObjRender objRender;
    public:
        KarakuriMeshObj( cinder::JsonTree const& params );
    public:
        virtual void Update( int count ) override;
        virtual void Draw( ) override;
    };
}