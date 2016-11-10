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


        cinder::gl::Texture* ground;
        SkydomeRef skydome;

        cinder::gl::Texture* sky;

    public:
        FieldManager( std::string const& path );
    public:
        void Update( );
        void Draw( cinder::CameraPersp const& camera );
        void End( );
        bool IsChange( );
        // ステージが動いているならtrueを返します。
        bool IsMoveing( );
        // もしステージを変えてもいいなら次のステージをインスタンス化します。
        void ChangeField( );
        bool IsLastField( );
        // 現在のフィールド情報の入ったJSONファイルのパスを返します。
        std::string FieldDataPath( );
    private:
        void NextField( );
        void CreateField( int number );
    };

    using FieldManagerRef = std::shared_ptr<FieldManager>;
}