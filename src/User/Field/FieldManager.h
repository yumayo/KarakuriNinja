# pragma once

# include <memory>
# include <map>

# include "Field.h"

namespace User
{
    using FieldRef = std::shared_ptr<Field>;

    class FieldManager
    {
        int nowNumber;
        std::map<int, std::string> fieldName;
        FieldRef field;
    public:
        FieldManager( std::string const& path );
    public:
        void Update( );
        void Draw( cinder::CameraPersp const& camera );
        void End( );
        bool IsChange( );
        void ChangeField( );
        bool IsLastField( );
        std::string FieldDataPath( );
    private:
        void NextField( );
        void CreateField( int number );
    };

    using FieldManagerRef = std::shared_ptr<FieldManager>;
}