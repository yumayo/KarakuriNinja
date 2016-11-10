# pragma once

# include "cinder/Vector.h"
# include "cinder/Color.h"

# include "cinder/Json.h"

namespace User
{
    // JSON‚Ì”z—ñ‚©‚çVec3f‚Ö
    cinder::Vec3f getVec3f( cinder::JsonTree const& params );

    // JSON‚Ì”z—ñ‚©‚çColor‚Ö
    cinder::Color getColor( cinder::JsonTree const& params );

    // JSON‚Ì”z—ñ‚©‚çColorA‚Ö
    cinder::Color getColorA( cinder::JsonTree const& params );
}