# pragma once

# include <string>

# include <memory>

# include "cinder/gl/Texture.h"
# include "cinder/Vector.h"

# include "Yumayo.h"

namespace User
{
    class Moji
    {
    public:
        class Data
        {
            int begin;
            int move;
            cinder::Vec2f beginPosition;
            cinder::Vec2f endPosition;
            cinder::Vec2f beginSize;
            cinder::Vec2f endSize;
            cinder::gl::Texture* texture;
            std::string moji;
        public:
            Data( cinder::JsonTree const & params );
        public:
            cinder::Vec2f Position( int frame );
            cinder::Vec2f Size( int frame );
            cinder::gl::Texture* Texture( ) { return texture; }
            std::string GetMoji( ) { return moji; }
        private:
            float Frame( int frame );
        };
    private: Data data;
    public: Moji( Data data );
    public: void Draw( int frame, Fonts& font );
    };

    using MojiRef = std::shared_ptr<Moji>;
}