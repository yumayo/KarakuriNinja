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
        // �X�e�[�W�������Ă���Ȃ�true��Ԃ��܂��B
        bool IsMoveing( );
        // �����X�e�[�W��ς��Ă������Ȃ玟�̃X�e�[�W���C���X�^���X�����܂��B
        void ChangeField( );
        bool IsLastField( );
        // ���݂̃t�B�[���h���̓�����JSON�t�@�C���̃p�X��Ԃ��܂��B
        std::string FieldDataPath( );
    private:
        void NextField( );
        void CreateField( int number );
    };

    using FieldManagerRef = std::shared_ptr<FieldManager>;
}