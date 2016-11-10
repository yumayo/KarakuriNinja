# pragma once

# include "cinder/Vector.h"
# include "cinder/Color.h"

# include "cinder/Json.h"

namespace User
{
    // JSONの配列からVec3fへ
    cinder::Vec3f getVec3f( cinder::JsonTree const& params );

    // JSONの配列からColorへ
    cinder::Color getColor( cinder::JsonTree const& params );

    // JSONの配列からColorAへ
    cinder::Color getColorA( cinder::JsonTree const& params );
}