
#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include"SpecialSceneBase.h"
#include"SpecialSelect.h"
#include"SpecialMinigame.h"
#include"SpecialEffect.h"
#include"NotSpecial.h"
#include"SubTime.h"
#include<memory>

class Special {
public:
	Special() {
		isspesial = false;
		mode = NOTSPECIAL;
		special_type_ = NOTSELECTED;
		special_ = std::make_shared<NotSpecial>();
		effectend_ = false;
	}
	void update();
	void draw();
	
	//�X�y�V������I�����郂�[�h�ɓ��鎞�Ɏg���܂��A�ꉞ�T�u���ʒ��͓���Ȃ��悤�ɂ��Ă����܂���
	void goSpecialMode() {
		if (special_type_ == NOTSELECTED) {
			isspesial = true;
			special_ = std::make_shared<SpecialSelect>();
			mode = SELECT;
		}
	}

	//�X�y�V�����Z���N�g�ɓ����Ă���G�t�F�N�g���I���܂Ŋ�TRUE���A��܂��B
	//�v���C���[�̍U����K�[�h��update�A�G��update���~�߂�ȂǂɎg���Ă�������
	bool getIsSpecial() {
		return isspesial;
	}

	//�~�j�Q�[�����͉�ʑS�̂��g�����߃}�b�v��G�������Ȃ��\��Ȃ̂ŁA
	//�����d���Ȃ�����y�������̂��߂��̃t���O�ŕ`����Ƃ߂Ă����Ă��������B�ʏ펞�Ffalse,�Z���N�g�Ffalse,�~�j�Q�[��:true,�G�t�F�N�g:false
	bool isMinigame() {
		return special_->isFullScreen();
	}

	//�I�񂾃X�y�V�������擾���܂��B�G�t�F�N�g���I����ď��߂�FIRE�Ȃǂ�����܂�
	SpecialType getSpecialType() {
		return special_type_;
	}
	//�T�u���ʂ̎��Ԃ��I������炱����Ă�ł�������
	void resetSpecial() {
		special_type_ = NOTSELECTED;
	}
	//���݃v���C���[�N���X�ł�����Ă�ł��Ȃ����߁A�T�u���ʂ̎��Ԃ������݂����ȏ󋵂Ȃ̂�
	//��x�X�y�V�������g���ƂQ�x�ƃX�y�V�������g���Ȃ������ɂȂ��Ă��܂�


	//�G�t�F�N�g���ɓG���_���[�W��������Ă��銴�����o���Ƃ��ȂǂɎg���Ă�������
	bool isEffecting() {
		return mode == MODE::EFFECT;
	}

	//�G�t�F�N�g���I������u�Ԃ̂P�t���[������TRUE���A��܂�
	//�G�S�̂Ƀ_���[�W��^���鏈���ȂǂɎg���Ă�������
	bool getEffectEnd() {
		return effectend_;
	}
private:
	std::shared_ptr<SpecialSceneBase>special_;
	void shift();
	enum MODE {
		NOTSPECIAL, SELECT, MINIGAME, EFFECT,SUBTIME
	};
	MODE mode;
	bool isspesial;
	bool effectend_;
	SpecialType special_type_;
};