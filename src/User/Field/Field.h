# pragma once

#include <memory>
#include <list>

# include "KarakuriBase.h"

namespace User
{
    using KarakuriRef = std::shared_ptr<KarakuriBase>;
    using KarakuriList = std::list<KarakuriRef>;

    class Field
    {
        KarakuriList karakuris;
        int count;
    public:
        Field( std::string const& path );
    public:
        void update( );
        void draw( );
    private:
        void Each( std::function<void( KarakuriRef& karakuri )> func );
    };

    using FieldRef = std::shared_ptr<Field>;
}