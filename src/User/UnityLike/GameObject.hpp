# pragma once

# include <memory>
# include <string>
# include <algorithm>

# include "Types.hpp"
# include "Component.hpp"

# include "CPMeshFilter.hpp"
# include "CPMeshRenderer.hpp"

namespace User
{
    class GameObject
    {
    public:
        GameObject( ) : transform( ) { }
        virtual ~GameObject( ) { }
    public:
        virtual void update( );
        virtual void shadowDraw( );
        virtual void draw( );
    public:
        void addComponent( ComponentPtr instans );
        template <class _Ty>
        _Ty& getComponent( );
    public:
        template <class _Ty>
        void destroy( );
    public:
        Transform transform;
    protected:
        ComponentRefs componentRefs;
    private:
        friend class ColliderStack;
    };

    template<class _Ty>
    inline _Ty & GameObject::getComponent( )
    {
        static ComponentPtr null = nullptr;
        auto component = std::find_if( componentRefs.begin( ), componentRefs.end( ),
                                       [ ] ( ComponentRef component ) { return component->name( ) == typeid( _Ty ).name( ); } );

        if ( component != nullptr ) return *( dynamic_cast<_Ty*>( ( *component )->get( ) ) );
        else return *( dynamic_cast<_Ty*>( null ) );
    }

    template<class _Ty>
    inline void GameObject::destroy( )
    {
        auto eraceList = std::remove_if( componentRefs.begin( ), componentRefs.end( ),
                                         [ ] ( ComponentRef compornent ) { return compornent->name( ) == typeid( _Ty ).name( ); } );
        componentRefs.erase( eraceList, componentRefs.end( ) );
    }
}