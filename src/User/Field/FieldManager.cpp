#include "FieldManager.h"

#include "cinder/app/App.h"
#include "cinder/Json.h"

#include "GlobalData.hpp"
#include "../Utilitys/Hirasawa.h"

#include "../Utilitys/fileUtil.hpp"

#include "../Utilitys/Yumayo.h"

namespace User
{
    using namespace cinder;

    FieldManager::FieldManager( std::string const& path )
        : nowNumber( 0 )
        , shift( 0 )
    {
        sky = &GData::FindTexture( "Stage/BackGround.png" );

        JsonTree params( app::loadAsset( path ) );

        int index = 0;
        for ( auto& obj : params["Field"] )
        {
            fieldName.insert( std::make_pair( index++, obj.getValue( ) ) );
        }

        shift = getInt( params["Shift"] );

        CreateField( nowNumber );
    }
    void FieldManager::Update( )
    {
        field->update( );
    }
    void FieldManager::Draw( cinder::CameraPersp const& camera )
    {
        float zAngle = -( ( nowNumber + shift ) * 120 );
        if ( !field->IsEnd( ) )
        {
            int prevAngle = -( ( nowNumber + shift - 1 ) * 120 );
            zAngle = EasingElasticInOut( field->NormalizedCount( ), prevAngle, zAngle );
        }

        gl::pushModelView( );
        gl::translate( Vec3f( 0, -20, 20.0F ) );
        gl::rotate( Vec3f( 0, 0, zAngle ) );
        gl::draw( *sky, Rectf( Vec2f( -50, -50 ), Vec2f( 50, 50 ) ) );
        gl::popModelView( );

        field->draw( camera );
    }
    void FieldManager::End( )
    {
        field->End( );
    }
    bool FieldManager::IsChange( )
    {
        return field->IsNextField( );
    }
    bool FieldManager::IsMoveing( )
    {
        return field->IsMoveing( );
    }
    void FieldManager::ChangeField( )
    {
        if ( IsChange( ) && !IsLastField( ) ) NextField( );
    }
    bool FieldManager::IsLastField( )
    {
        return nowNumber == fieldName.size( ) - 1;
    }
    std::string FieldManager::FieldDataPath( )
    {
        return fieldName[nowNumber];
    }
    int FieldManager::GetFieldNumber( )
    {
        return nowNumber;
    }
    void FieldManager::NextField( )
    {
        if ( !IsLastField( ) )
        {
            nowNumber += 1;
            CreateField( nowNumber );
        }
    }
    void FieldManager::CreateField( int number )
    {
        if ( fieldName.find( number ) != fieldName.end( ) )
        {
            field = std::make_shared<Field>( fieldName.find( number )->second );
        }
    }
}