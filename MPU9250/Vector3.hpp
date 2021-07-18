#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _VECTOR3_HPP_
#define _VECTOR3_HPP_

class Vector3 {
public:
	Vector3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	};
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	};

public:
	float x;
	float y;
	float z;

	/**
	*  @bref �R�����x�N�g���̗v�f���R�s�[
	*/
	Vector3 operator=(Vector3 vector) {
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return *this;
	};

	/**�R�����x�N�g���𑫂��đ������*/
	Vector3 operator+=(Vector3 vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	};

	/** �R�����x�N�g���������đ������ */
	Vector3 operator-=(Vector3 vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	};

	/**�R�����x�N�g�����O�ς��đ������*/
	Vector3 operator*=(Vector3 vec) {
		float _x = y * vec.z - z * vec.y;
		float _y = z * vec.x - x * vec.z;
		float _z = x * vec.y - y * vec.x;
		x = _x;
		y = _y;
		z = _z;
		return *this;
	}

	Vector3 operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	bool operator==(Vector3 vec) {
		if ((x == vec.x) && (y == vec.y) && (z == vec.z)) {
			return true;
		}
		return false;
	}
	bool operator!=(Vector3 vec) {
		if ((x == vec.x) && (y == vec.y) && (z == vec.z)) {
			return false;
		}
		return true;
	}
	/**
	*  @bref Vector3�N���X�̊e�v�f�����������܂��B
	*/
	template<typename T>void Set(T _x, T _y, T _z);

	/**
	*  @bref �Q��Vector3�N���X�̂Ȃ��p���v�Z���܂�.
	*  @param �����Ƃ̂Ȃ��p�x���v�Z����Vector3�N���X�̃C���X�^���X
	*/
	float Angle(Vector3 v);

	/**
	*  @bref �Q��Vector3�N���X�̂Ȃ��p���v�Z���܂�.
	*  @param �����Ƃ̂Ȃ��p�x���v�Z����Vector3�N���X�̃C���X�^���X
	*  @return ���� * ����ƂȂ�x�N�g������]���Ƃ��ĕ����t���̂Ȃ��p�x�ł�
	*/
	float SgnAngle(Vector3 vec);

	/**
	*  @bref �[���x�N�g�����ǂ������肵�܂�.
	*  @note 0���@��h�~����̂Ɏg���Ă�������.
	*  @return 1�Ȃ�[���x�N�g���A0�Ȃ�[���x�N�g���ł͂���܂���.
	*/
	int CheckZero() {
		if (x == 0.0f && y == 0.0f && z == 0.0f) {
			return 1;
		}
		return 0;
	}

	/**
	*  @bref ���g�̃m�������v�Z���ĕԂ��܂�.
	*/
	float Norm();

	/**
	*  @bref  �P�ʃx�N�g���ɂ��܂�
	*/
	void Normalize() {
		float norm = sqrtf(x * x + y * y + z * z);
		if (norm != 0.0f) {
			x /= norm;
			y /= norm;
			z /= norm;
			return;
		}
		else {
			return;
		}
	}

	char* toString() {
		char str[128] = {};

		sprintf(str, "(%f,%f,%f)", x, y, z);
		return str;
	}

};

/**
*  @bref Vector3�N���X�̊e�v�f�̘a���v�Z���܂��B
*/
inline Vector3 operator+(Vector3 left, Vector3 right) {
	static Vector3 vec;
	vec.x = left.x + right.x;
	vec.y = left.y + right.y;
	vec.z = left.z + right.z;
	return vec;
}

/**
* @bref Vector3�N���X�̊e�v�f�̍����v�Z���܂��B
*/
inline Vector3 operator-(Vector3 left, Vector3 right) {
	static Vector3 vec;
	vec.x = left.x - right.x;
	vec.y = left.y - right.y;
	vec.z = left.z - right.z;
	return vec;
}

/**
*  @bref Vector3�N���X�̊O�ς��v�Z���܂�.
*  @note �O�ςł��̂ŏ����ɒ��ӂ��Ă�������.
*/
inline Vector3 operator*(Vector3 left, Vector3 right) {
	static Vector3 vec;
	vec.x = left.y * right.z - left.z * right.y;
	vec.y = left.z * right.x - left.x * right.z;
	vec.z = left.x * right.y - left.y * right.x;
	return vec;
}

/**
*  @bref ���ς��v�Z���܂�
*  @note �h�b�g���g���Ȃ������̂Ł��ɂȂ�܂����B�����ăq���V���X
*/
inline float operator%(Vector3 left, Vector3 right) {
	return (left.x * right.x + left.y * right.y + left.z * right.z);
}

/**
*  @bref Vector3�N���X�̊e�v�f���X�J���[�{���܂�
*/
inline Vector3 operator*(float scalar, Vector3 vec3) {
	static Vector3 vec;
	vec.x = scalar * vec3.x;
	vec.y = scalar * vec3.y;
	vec.z = scalar * vec3.z;
	return vec;
}

/**
*  @bref Vector3�N���X�̊e�v�f���X�J���[�{���܂�
*/
inline Vector3 operator*(Vector3 vec3, float scalar) {
	static Vector3 vec;
	vec.x = scalar * vec3.x;
	vec.y = scalar * vec3.y;
	vec.z = scalar * vec3.z;
	return vec;
}

/**
*  @bref  Vector3�N���X�̊e�v�f���X�J���[�Ŋ���܂�
*/
inline Vector3 operator/(Vector3 vec3, float scalar) {
	static Vector3 vec;
	vec.x = vec3.x / scalar;
	vec.y = vec3.y / scalar;
	vec.z = vec3.z / scalar;
	return vec;
}

template<typename T>void Vector3::Set(T _x, T _y, T _z) {
	x = _x;
	y = _y;
	z = _z;
}

inline float Vector3::Angle(Vector3 vec) {
	float r = (*this % vec) / (this->Norm() * vec.Norm());
	return acosf(r);
}

inline float Vector3::SgnAngle(Vector3 vec) {
	float 	theta = this->Angle(vec);
	vec = *this * vec;
	if (vec.z < 0) theta *= -1.0;

	return theta;
}

inline float Vector3::Norm() {
	return sqrtf(x * x + y * y + z * z);
}

#endif