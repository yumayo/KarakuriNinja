# pragma once

# include "cinder/Vector.h"

namespace User
{
    template <typename Ty>
    class TransformTy
    {
    public:
        TransformTy( const cinder::Vec3<Ty>& translation = cinder::Vec3<Ty>::zero( ), const cinder::Vec3<Ty>& rotation = cinder::Vec3<Ty>::zero( ), const cinder::Vec3<Ty>& scale = cinder::Vec3<Ty>::one( ) )
            : translation( translation )
            , rotation( rotation )
            , scale( scale )
        { }
        TransformTy( const TransformTy<Ty>& transform )
            : translation( transform.translation )
            , rotation( transform.rotation )
            , scale( transform.scale )
        { }
    public:
        cinder::Vec3<Ty> translation;
        cinder::Vec3<Ty> rotation;
        cinder::Vec3<Ty> scale;

    public:
        const TransformTy<Ty> operator+( const TransformTy<Ty>& rhs ) const { return TransformTy<Ty>( translation + rhs.translation, rotation + rhs.rotation, scale + rhs.scale ); }
        const TransformTy<Ty> operator-( const TransformTy<Ty>& rhs ) const { return TransformTy<Ty>( translation - rhs.translation, rotation - rhs.rotation, scale - rhs.scale ); }
        const TransformTy<Ty> operator*( const TransformTy<Ty>& rhs ) const { return TransformTy<Ty>( translation * rhs.translation, rotation * rhs.rotation, scale * rhs.scale ); }
        const TransformTy<Ty> operator/( const TransformTy<Ty>& rhs ) const { return TransformTy<Ty>( translation / rhs.translation, rotation / rhs.rotation, scale / rhs.scale ); }

        TransformTy<Ty>& operator=( const TransformTy<Ty>& rhs )
        {
            translation = rhs.translation; rotation = rhs.rotation; scale = rhs.scale;
            return *this;
        }

        template<typename FromT>
        TransformTy<Ty>& operator=( const TransformTy<FromT> &rhs )
        {
            translation = static_cast<cinder::Vec3<Ty>>( rhs.translation ); rotation = static_cast<cinder::Vec3<Ty>>( rhs.rotation ); scale = static_cast<cinder::Vec3<Ty>>( rhs.scale );
            return *this;
        }
    };

    template<typename Ty, typename Sc>
    inline TransformTy<Ty> operator *( Sc s, const TransformTy<Ty> &t ) { return TransformTy<Ty>( t.translation * s, t.rotation * s, t.scale * s ); }
    template<typename Ty, typename Sc>
    inline TransformTy<Ty> operator *( const TransformTy<Ty> &t, Sc s ) { return TransformTy<Ty>( t.translation * s, t.rotation * s, t.scale * s ); }
    template<typename Ty, typename Sc>
    inline TransformTy<Ty> operator /( Sc s, const TransformTy<Ty> &t ) { return TransformTy<Ty>( t.translation / s, t.rotation / s, t.scale / s ); }
    template<typename Ty, typename Sc>
    inline TransformTy<Ty> operator /( const TransformTy<Ty> &t, Sc s ) { return TransformTy<Ty>( t.translation / s, t.rotation / s, t.scale / s ); }

    using Transform = TransformTy<float>;
    using Transformf = TransformTy<float>;
    using Transformi = TransformTy<int>;
}