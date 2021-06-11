#include "BoneAnimation.h"

float BoneAnimation::GetStartTime() const
{
    return 0.0f;
}

float BoneAnimation::GetEndTime() const
{
    return 0.0f;
}

void BoneAnimation::Interpolate(float t, XMFLOAT4X4& M) const
{
    if (t <= KeyFrames.front().TimePos)
    {
        XMVECTOR S = XMLoadFloat3(&KeyFrames.front().Scale);
        XMVECTOR P = XMLoadFloat3(&KeyFrames.front().Translation);
        XMVECTOR Q = XMLoadFloat4(&KeyFrames.front().RotationQuat);

        XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
    }

    else if (t >= KeyFrames.back().TimePos)
    {
        XMVECTOR S = XMLoadFloat3(&KeyFrames.back().Scale);
        XMVECTOR P = XMLoadFloat3(&KeyFrames.back().Translation);
        XMVECTOR Q = XMLoadFloat4(&KeyFrames.back().RotationQuat);

        XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.f);
        XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
    }

    else
    {
        for (UINT i = 0; i < KeyFrames.size() - 1; ++i)
        {
            if (t >= KeyFrames[i].TimePos && t <= KeyFrames[i + 1].TimePos)
            {
                float lerpPercent = (t - KeyFrames[i].TimePos) /
                    (KeyFrames[i + 1].TimePos - KeyFrames[i].TimePos);

                XMVECTOR s0 = XMLoadFloat3(&KeyFrames[i].Scale);
                XMVECTOR s1 = XMLoadFloat3(&KeyFrames[i + 1].Scale);

                XMVECTOR p0 = XMLoadFloat3(&KeyFrames[i].Translation);
                XMVECTOR p1 = XMLoadFloat3(&KeyFrames[i + 1].Translation);

                XMVECTOR q0 = XMLoadFloat4(&KeyFrames[i].RotationQuat);
                XMVECTOR q1 = XMLoadFloat4(&KeyFrames[i + 1].RotationQuat);

                XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
                XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
                XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);

                XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.f);
                XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
                break;
            }
        }
    }
}

KeyFrame::KeyFrame()
{
}

KeyFrame::~KeyFrame()
{
}

void Test()
{

    XMVECTOR q0 = XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f,
        0.f), XMConvertToRadians(30.f));
    XMVECTOR q1 = XMQuaternionRotationAxis(XMVectorSet(1.f, 1.f, 2.f,
        0.f), XMConvertToRadians(45.f));
    XMVECTOR q2 = XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f,
        0.f), XMConvertToRadians(-30.f));
    XMVECTOR q3 = XMQuaternionRotationAxis(XMVectorSet(1.f, 0.f, 0.f,
        0.f), XMConvertToRadians(70.f));

    mSkullAnimation.KeyFrames.resize(5);
    mSkullAnimation.KeyFrames[0].TimePos = 0.f;
    mSkullAnimation.KeyFrames[0].Translation = XMFLOAT3(-7.f, 0.f, 0.f);
    mSkullAnimation.KeyFrames[0].Scale = XMFLOAT3(0.25f, 0.25f, 0.25f);
    XMStoreFloat4(&mSkullAnimation.KeyFrames[0].RotationQuat, q0);

    mSkullAnimation.KeyFrames[1].TimePos = 2.f;
    mSkullAnimation.KeyFrames[1].Translation = XMFLOAT3(0.f, 2.f, 10.f);
    mSkullAnimation.KeyFrames[1].Scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
    XMStoreFloat4(&mSkullAnimation.KeyFrames[1].RotationQuat, q1);

    mSkullAnimation.KeyFrames[2].TimePos = 4.f;
    mSkullAnimation.KeyFrames[2].Translation = XMFLOAT3(7.f, 0.f, 0.f);
    mSkullAnimation.KeyFrames[2].Scale = XMFLOAT3(0.25f, 0.25f, 0.25f);
    XMStoreFloat4(&mSkullAnimation.KeyFrames[2].RotationQuat, q2);

    mSkullAnimation.KeyFrames[3].TimePos = 6.f;
    mSkullAnimation.KeyFrames[3].Translation = XMFLOAT3(0.f, 1.f, -10.f);
    mSkullAnimation.KeyFrames[3].Scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
    XMStoreFloat4(&mSkullAnimation.KeyFrames[3].RotationQuat, q3);

    mSkullAnimation.KeyFrames[4].TimePos = 8.f;
    mSkullAnimation.KeyFrames[4].Translation = XMFLOAT3(-7.f, 0.f, 0.f);
    mSkullAnimation.KeyFrames[4].Scale = XMFLOAT3(0.25f, 0.25f, 0.25f);
    XMStoreFloat4(&mSkullAnimation.KeyFrames[4].RotationQuat, q0);
}

void UpdateScene(float dt)
{
    mAnimTimePos += dt;
    if (mAnimTimePos >= mSkullAnimation.GetEndTime())
    {
        mAnimTimePos = 0.f;
    }

    mSkullAnimation.Interpolate(mAnimTimePos, mSkullWorld);
}
