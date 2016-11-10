#include "FieldManager.h"

#include "cinder/app/App.h"
#include "cinder/Json.h"

namespace User
{
    using namespace cinder;

    FieldManager::FieldManager( std::string const& path )
        : nowNumber( 0 )
    {
        JsonTree params( app::loadAsset( path ) );

        int index = 0;
        for ( auto& obj : params["Field"] )
        {
            fieldName.insert( std::make_pair( index++, obj.getValue( ) ) );
        }

        CreateField( nowNumber );
    }
    void FieldManager::Update( )
    {
        field->update( );
    }
    void FieldManager::Draw( cinder::CameraPersp const& camera )
    {
        field->draw( camera );
    }
    void FieldManager::End( )
    {
        // 最後のフィールドはカラクリが発動しません。
        if ( !IsLastField( ) ) field->End( );
    }
    bool FieldManager::IsChange( )
    {
        return field->IsNextField( );
    }
    void FieldManager::ChangeField( )
    {
        if ( IsChange( ) ) NextField( );
    }
    bool FieldManager::IsLastField( )
    {
        return nowNumber == fieldName.size( ) - 1;
    }
    std::string FieldManager::FieldDataPath( )
    {
        return fieldName[nowNumber];
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