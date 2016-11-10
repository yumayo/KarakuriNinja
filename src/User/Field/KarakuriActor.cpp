#include "KarakuriActor.h"

namespace User
{
    KarakuriActor::KarakuriActor( cinder::JsonTree const & params )
        : object( params )
    {
    }
    void KarakuriActor::Update( int count )
    {
        object.Move( count );
    }
}