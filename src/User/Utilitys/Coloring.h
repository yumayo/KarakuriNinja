# pragma once

# include "cinder/Color.h"

# include <string>

namespace Utl
{
    // カラーリングに関する項目です。
    // ・htmlカラーをカラーへ変換
    class Coloring
    {
    public:
        static cinder::ColorA html( const std::string& str );
    private:
        Coloring( ) {}
        ~Coloring( ) {}
    };
}