#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

// X軸回転行列
Matrix4x4 MakeRotateXmatrix(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYmatrix(float radian); 

// Z軸回転行列
Matrix4x4 MakeRotateZmatrix(float radian);

// 平行移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate); 

// 拡大縮小
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
