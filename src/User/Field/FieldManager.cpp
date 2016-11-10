#include "FieldManager.h"

namespace User
{
    FieldManager::FieldManager( )
        : nowNumber( 0 )
    {
        fieldName.insert( std::make_pair( 0, "Field1.json" ) );
        fieldName.insert( std::make_pair( 1, "Field3.json" ) );
        fieldName.insert( std::make_pair( 2, "Field5.json" ) );

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