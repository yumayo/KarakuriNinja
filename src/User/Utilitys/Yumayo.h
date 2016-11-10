# pragma once

# include "cinder/Vector.h"
# include "cinder/Color.h"

# include "cinder/Json.h"

namespace User
{
    // JSON�̔z�񂩂�Vec3f��
    cinder::Vec3f getVec3f( cinder::JsonTree const& params );

    // JSON�̔z�񂩂�Color��
    cinder::Color getColor( cinder::JsonTree const& params );

    // JSON�̔z�񂩂�ColorA��
    cinder::Color getColorA( cinder::JsonTree const& params );
}