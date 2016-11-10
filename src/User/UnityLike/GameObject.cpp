#include "GameObject.hpp"

# include "cinder/gl/gl.h"

# include "Hierarchy.hpp"

namespace User
{
    using namespace cinder;

    void GameObject::addComponent( ComponentPtr instans )
    {
        instans->gameObjectPtr = this;
        componentRefs.emplace_back( std::move( instans ) );
    }

    void GameObject::draw( )
    {
        gl::pushModelView( );
        gl::translate( transform.translation );
        gl::scale( transform.scale );
        gl::rotate( transform.rotation );

        for ( auto& obj : componentRefs ) obj->drawBegin( );
        for ( auto& obj : componentRefs ) obj->draw( );
        for ( auto& obj : componentRefs ) obj->drawEnd( );

        gl::popModelView( );
    }
    
    void GameObject::update( )
    {
        for ( auto& obj : componentRefs ) obj->updateBegin( );
        for ( auto& obj : componentRefs ) obj->update( );
        for ( auto& obj : componentRefs ) obj->updateCollision( );
        for ( auto& obj : componentRefs ) obj->updateEnd( );
    }
    void GameObject::shadowDraw( )
    {
        gl::pushModelView( );
        gl::translate( transform.translation );
        gl::scale( transform.scale );
        gl::rotate( transform.rotation );

        for ( auto& obj : componentRefs ) obj->draw( );

        gl::popModelView( );
    }
}