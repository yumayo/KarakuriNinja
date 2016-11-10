# include "EnemyObject.h"

namespace User
{
    using namespace cinder;

    void EnemyObject::Direction( cinder::Vec3f direction )
    {
        this->direction = direction.normalized( );
        quaternion = Quatf( Vec3f::zAxis( ), this->direction );
    }

    void EnemyObject::Target( cinder::Vec3f targetPosition )
    {
        // �z�[�~���O�̌v�Z
        // TIPS:�N�I�[�^�j�I���ƊO�ς��g���΂����ȒP�Ɍv�Z�ł���
        Vec3f d = direction - pos;
        // �ڕW�܂ł̌����x�N�g���𐳋K��
        d.normalize( );
        // �����̌����x�N�g���ƖڕW�܂ł̌����x�N�g���̊O��
        // ���N�I�[�^�j�I���̉�]��
        Vec3f cross = this->direction.cross( d );
        if ( cross.lengthSquared( ) > 0.0f ) {
            // FIXME:�O�ςŋ��߂��x�N�g���̒������p�x���ɂȂ�̂����A
            //       �p�x��\���Ă���킯�ł͂Ȃ��̂Ŕ���
            Quatf rotate( cross.normalized( ), cross.length( ) );

            // ��]��\���N�I�[�^�j�I�����Čv�Z
            // TIPS:�s��Ɠ������A�|���Z�Ō������ώZ������
            quaternion = quaternion * rotate;
            // �����x�N�g�����Čv�Z
            this->direction = quaternion * Vec3f::zAxis( );
        }
    }
}