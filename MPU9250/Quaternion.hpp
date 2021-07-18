#ifndef _QUATERNION_HPP_
#define _QUATERNION_HPP_
#include "Vector3/Vector3.hpp"
/**
* �N�H�[�^�j�I���̑����C�����C�|���Z�Ȃǂ��ȒP�ɂł���悤�ɂȂ�܂��D
* @author  Gaku MATSUMOTO
* @bref  �N�H�[�^�j�I�����g����N���X�ł��D
*/

class Quaternion {
public:
	/**
		@bref	Quaternion�C���X�^���X�𐶐����܂�
	*/
	Quaternion() {
		w = 1.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	};

	/**
	*  @bref Vector3�N���X����N�H�[�^�j�I�������܂�
	*/
	Quaternion(Vector3 vector) {
		Set(vector);
	}

	/**
	*  @bref   �N�H�[�^�j�I������]���Ɖ�]�p�x�ɂ���ď��������܂��B
	*  @param   vec ��]���ƂȂ�R�����x�N�g��
	*  @param   angle ��]�p [rad]
	*/
	Quaternion(Vector3 vec, float angle) {
		Set(vec, angle);
	}

	/**
	@bref	�v�f�������Ȃ���C�C���X�^���X�𐶐����܂��D
	@param[in]  _w  ����w�̏����l
	@param[in]  _x  ����i�̏����l
	@param[in]  _y  ����j�̏����l
	@param[in]  _z  ����k�̏����l
	*/
	Quaternion(float _w, float _x, float _y, float _z) {
		w = _w; x = _x; y = _y; z = _z;
	};

public:
	float w;
	float x;
	float y;
	float z;


public:

	/**
	@bref  �N�H�[�^�j�I���̗v�f���R�s�[���܂��D
	@note  �ʏ�̐��̂悤�ɑ���ł��܂�
	*/
	Quaternion operator=(Quaternion r) {
		w = r.w;
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	};

	/**
	@bref  �N�H�[�^�j�I���𑫂��đ�����܂��D
	@note  �ʏ�̐��̂悤�ɑ���ł��܂�
	*/
	Quaternion operator+=(Quaternion r) {
		w += r.w;
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	};

	/**
	@bref  �N�H�[�^�j�I���������đ�����܂��D
	@note  �ʏ�̐��̂悤�ɑ���ł��܂�
	*/
	Quaternion operator-=(Quaternion r) {
		w -= r.w;
		x -= r.x;
		y -= r.y;
		z -= r.z;
		return *this;
	};

	/**
	*	@bref	�N�H�[�^�j�I���̊|���Z�����܂��D
	*	@note	���̍ۂ������͏d�v�ł��D
	*/
	Quaternion operator*=(Quaternion r) {
		static Quaternion QQ;
		QQ.w = w * r.w - x * r.x - y * r.y - z * r.z;
		QQ.x = x * r.w + w * r.x - z * r.y + y * r.z;
		QQ.y = y * r.w + z * r.x + w * r.y - x * r.z;
		QQ.z = z * r.w - y * r.x + x * r.y + w * r.z;
		w = QQ.w;
		x = QQ.x;
		y = QQ.y;
		z = QQ.z;
		return *this;
	};

	/**
		@bref	�N�H�[�^�j�I���̕��f������Ԃ��܂��D
		@note	�{���̓A�X�^���X�N���ǂ������̂ł����C�|�C���^�ƕ���킵���̂Ń}�C�i�X�ɂ��܂����D
	*/
	Quaternion operator-() {
		Quaternion Q;
		Q.w = w;
		Q.x = -x;
		Q.y = -y;
		Q.z = -z;
		return Q;
	};

	/**
		@bref	�N�H�[�^�j�I���𐳋K�����āC�P�ʃN�H�[�^�j�I���ɂ��܂��D
		@note	�|���Z�Ȃǂ��s�����тɎ��s���邱�Ƃ������߂��܂��D
		@note   �����A�N�H�[�^�j�I���̎��Ԕ����͐��K�����Ă͂����܂���
	*/
	void Normalize() {
		float norm = sqrt(w * w + x * x + y * y + z * z);
		if (norm != 0.0f) {
			w /= norm;
			x /= norm;
			y /= norm;
			z /= norm;
			return;
		}
		else {
			return;
		}
	};

	/**
	*  @bref   �N�H�[�^�j�I�������������܂�
	*/
	template <typename T> void Set(T _w, T _x, T _y, T _z);
	/**
	*  @bref   �N�H�[�^�j�I����Vector3�N���X�ŏ��������܂��B
	*/
	void Set(Vector3 vec);

	/**
	*  @bref   �N�H�[�^�j�I������]���Ɖ�]�p�x�ɂ���ď��������܂��B
	*  param   vec ��]���ƂȂ�R�����x�N�g��
	*  param   angle ��]�p [rad]
	*/
	void Set(Vector3 vec, float angle) {
		vec.Normalize();
		float halfAngle = 0.5f * angle;

		w = cosf(halfAngle);
		x = vec.x * sinf(halfAngle);
		y = vec.y * sinf(halfAngle);
		z = vec.z * sinf(halfAngle);
	}

	/**
	*  @bref   �N�H�[�^�j�I���̊e�v�f�ɔz��̂悤�ɃA�N�Z�X���܂�
	*/
	float q(int i) {
		float ans = 0.0;
		switch (i) {
		case 1:
			ans = w;
			break;
		case 2:
			ans = x;
			break;
		case 3:
			ans = y;
			break;
		case 4:
			ans = z;
			break;
		}
		return ans;
	}

	/**
	*  @bref   �N�H�[�^�j�I���̃m�������v�Z���܂�
	*/
	float Norm() {
		return fabsf(w * w + x * x + y * y + z * z);
	}


	/** �N�H�[�^�j�I���ƃN�H�[�^�j�I�����r���ē��������true �������Ȃ����false*/
	bool operator==(Quaternion Q) {
		if (w == Q.w && x == Q.x && y == Q.y && z == Q.z) {
			return true;
		}
		return false;
	}
	/** �N�H�[�^�j�I���ƃN�H�[�^�j�I�����r���ē������Ȃ����true ���������false*/
	bool operator!=(Quaternion Q) {
		if (w == Q.w && x == Q.x && y == Q.y && z == Q.z) {
			return false;
		}
		return true;
	}

	/**
	*  @bref  2�̂R�����x�N�g������v������N�H�[�^�j�I�����v�Z
	*  @param from �n�_�ƂȂ�x�N�g���̃C���X�^���X
	*  @param to   �I�_�ƂȂ�x�N�g���̃C���X�^���X
	*/
	void FromToRotation(Vector3 from, Vector3 to);

	/**
	@bref   �I�C���[�p�Ŏp�����擾���܂��D
	@param  val ���[���C�s�b�`�C���[�̏��ɔz��Ɋi�[���܂��D�R�ȏ�̗v�f�̔z������Ă��������D
	@note   �l��[rad]�ł��D[degree]�ɕϊ����K�v�ȏꍇ�͕ʓr�v�Z���ĉ������D
	*/
	void GetEulerAngle(float* val) {
		float q0q0 = w * w, q1q1q2q2 = x * x - y * y, q3q3 = z * z;
		val[0] = (atan2f(2.0f * (w * x + y * z), q0q0 - q1q1q2q2 + q3q3));
		val[1] = (-asinf(2.0f * (x * z - w * y)));
		val[2] = (atan2f(2.0f * (x * y + w * z), q0q0 + q1q1q2q2 - q3q3));
	}

	/**
	@bref   �I�C���[�p�Ŏp�����擾���܂��D
	@param  val ���[���C�s�b�`�C���[�̏��ɔz��Ɋi�[���܂��D�R�ȏ�̗v�f�̔z������Ă��������D
	@note   �l��[rad]�ł��D[degree]�ɕϊ����K�v�ȏꍇ�͕ʓr�v�Z���ĉ������D
	*/
	void GetEulerAngle(Vector3* v) {
		float q0q0 = w * w, q1q1q2q2 = x * x - y * y, q3q3 = z * z;
		v->x = (atan2f(2.0f * (w * x + y * z), q0q0 - q1q1q2q2 + q3q3));
		v->y = (-asinf(2.0f * (x * z - w * y)));
		v->z = (atan2f(2.0f * (x * y + w * z), q0q0 + q1q1q2q2 - q3q3));
	}

	/**
	*  @bref  �N�H�[�^�j�I����Vector3�N���X�ɕϊ����܂�
	*  @note  �N�H�[�^�j�I����x,y,z�������������x�N�g�������܂�
	*/
	Vector3 ToVector3() {
		Vector3 vec3(x, y, z);
		return vec3;
	}

	/**
	*  @bref  3�����x�N�g������]���܂�
	*  @param v ��]��������3�����x�N�g���̃|�C���^
	*  @note  �]�v�ȃI�u�W�F�N�g�����܂���
	*/
	void Rotation(Vector3* v) {
		if (v == NULL) return;
		static float ww = 0.0f;
		static float xx = 0.0f;
		static float yy = 0.0f;
		static float zz = 0.0f;

		static float vx = 0.0f, vy = 0.0f, vz = 0.0f;
		static float _wx, _wy, _wz, _xy, _zx, _yz;
		ww = w * w;
		xx = x * x;
		yy = y * y;
		zz = z * z;

		_wx = w * x;
		_wy = w * y;
		_wz = w * z;
		_xy = x * y;
		_zx = z * x;
		_yz = y * z;

		vx = (ww + xx - yy - zz) * v->x + 2.0f * (_xy - _wz) * v->y + 2.0f * (_zx + _wy) * v->z;
		vy = 2.0f * (_xy + _wz) * v->x + (ww - xx + yy - zz) * v->y + 2.0f * (_yz - _wx) * v->z;
		vz = 2.0f * (_zx - _wy) * v->x + 2.0f * (_wx + _yz) * v->y + (ww - xx - yy + zz) * v->z;

		v->x = vx;
		v->y = vy;
		v->z = vz;
	}

	/**
	*  @bref  3�����x�N�g������]���܂�.�������t��]�ł�
	*  @param v ��]��������3�����x�N�g���̃|�C���^
	*  @note  �]�v�ȃI�u�W�F�N�g�����܂���
	*/
	void InvRotation(Vector3* v) {
		if (v == NULL) return;
		static float ww = 0.0f;
		static float xx = 0.0f;
		static float yy = 0.0f;
		static float zz = 0.0f;

		static float vx = 0.0f, vy = 0.0f, vz = 0.0f;
		static float _wx, _wy, _wz, _xy, _xz, _yz;
		ww = w * w;
		xx = x * x;
		yy = y * y;
		zz = z * z;

		_wx = w * x;
		_wy = w * y;
		_wz = w * z;
		_xy = x * y;
		_xz = x * z;
		_yz = y * z;

		vx = (ww + xx - yy - zz) * v->x + 2.0f * (_xy + _wz) * v->y + 2.0f * (_xz - _wy) * v->z;
		vy = 2.0f * (_xy - _wz) * v->x + (ww - xx + yy - zz) * v->y + 2.0f * (_yz + _wx) * v->z;
		vz = 2.0f * (_xz + _wy) * v->x + 2.0f * (-_wx + _yz) * v->y + (ww - xx - yy + zz) * v->z;

		v->x = vx;
		v->y = vy;
		v->z = vz;
	}
};

void Quaternion::FromToRotation(Vector3 from, Vector3 to) {
	float halfTheta = 0.5f * from.Angle(to);//��]�p�x 0����pi/2
	Vector3 axis = from * to;
	axis.Normalize();

	w = cos(halfTheta);
	x = axis.x * sin(halfTheta);
	y = axis.y * sin(halfTheta);
	z = axis.z * sin(halfTheta);
}



template<typename T>void Quaternion::Set(T _w, T _x, T _y, T _z) {
	w = _w;
	x = _x;
	y = _y;
	z = _z;
	return;
}

void Quaternion::Set(Vector3 vec) {
	w = 0.0;
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return;
}

/**
* @fn Quaternion operator*(Quaternion l, Quaternion r)
* @bref �N�H�[�^�j�I���̊|���Z�����܂��D���̍ہC�������d�v�ł��D
*/
Quaternion operator*(Quaternion l, Quaternion r) {
	static Quaternion Q;
	Q.w = l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z;
	Q.x = l.x * r.w + l.w * r.x - l.z * r.y + l.y * r.z;
	Q.y = l.y * r.w + l.z * r.x + l.w * r.y - l.x * r.z;
	Q.z = l.z * r.w - l.y * r.x + l.x * r.y + l.w * r.z;

	return Q;
};

/**
* @fn Quaternion operator*(double s, Quaternion q)
* @bref �N�H�[�^�j�I�����X�J���[�{���܂��D
*/
Quaternion operator*(float s, Quaternion q) {
	static Quaternion Q;
	Q.w = q.w * s;
	Q.x = q.x * s;
	Q.y = q.y * s;
	Q.z = q.z * s;
	return Q;
};

/**
* @fn Quaternion operator*(Quaternion q, double s)
* @bref �N�H�[�^�j�I�����X�J���[�{���܂��D
*/
Quaternion operator*(Quaternion q, float s) {
	static Quaternion Q;
	Q.w = q.w * s;
	Q.x = q.x * s;
	Q.y = q.y * s;
	Q.z = q.z * s;
	return Q;
};

/**
*/
Vector3 operator*(Quaternion q, Vector3 v) {

	static Vector3 ans;
	static float ww = 0.0f;
	static float xx = 0.0f;
	static float yy = 0.0f;
	static float zz = 0.0f;

	//static float vx = 0.0f, vy = 0.0f, vz = 0.0f;
	static float _wx, _wy, _wz, _xy, _zx, _yz;
	ww = q.w * q.w;
	xx = q.x * q.x;
	yy = q.y * q.y;
	zz = q.z * q.z;

	_wx = q.w * q.x;
	_wy = q.w * q.y;
	_wz = q.w * q.z;
	_xy = q.x * q.y;
	_zx = q.z * q.x;
	_yz = q.y * q.z;

	ans.x = (ww + xx - yy - zz) * v.x + 2.0f * (_xy - _wz) * v.y + 2.0f * (_zx + _wy) * v.z;
	ans.y = 2.0f * (_xy + _wz) * v.x + (ww - xx + yy - zz) * v.y + 2.0f * (_yz - _wx) * v.z;
	ans.z = 2.0f * (_zx - _wy) * v.x + 2.0f * (_wx + _yz) * v.y + (ww - xx - yy + zz) * v.z;

	return ans;
}


/**
	@bref	�N�H�[�^�j�I���̑����Z�����܂��D
*/
Quaternion operator+(Quaternion l, Quaternion r) {
	static Quaternion Q;
	Q.w = l.w + r.w;
	Q.x = l.x + r.x;
	Q.y = l.y + r.y;
	Q.z = l.z + r.z;
	return Q;
}

/**
	@bref	�N�H�[�^�j�I���̈����Z�����܂��D
*/
Quaternion operator-(Quaternion l, Quaternion r) {
	static Quaternion Q;
	Q.w = l.w - r.w;
	Q.x = l.x - r.x;
	Q.y = l.y - r.y;
	Q.z = l.z - r.z;
	return Q;
}

#endif