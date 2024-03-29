# pragma once

#include <memory>
#include <list>

# include "KarakuriActor.h"

# include "../Utilitys/Skydome.h"
# include "cinder/Camera.h"

namespace User
{
    using KarakuriRef = std::shared_ptr<KarakuriActor>;
    using KarakuriList = std::list<KarakuriRef>;

    class Field
    {
    private:
        KarakuriList karakuriMeshObjects;
        KarakuriList karakuriRectObjects;
        int count;
        int maxCount;
        bool isEnd;
    public:
        Field( std::string const& path );
    public:
        void update( );
        void draw( cinder::CameraPersp const& camera );
        void End( );
        bool IsEnd( );
        bool IsNextField( );
        bool IsMoveing( );

        float NormalizedCount( ) { return static_cast<float>( count ) / maxCount; }
    private:
        void Each( std::function<void( KarakuriRef& karakuri )> func );
    };

    using FieldRef = std::shared_ptr<Field>;
}