/////////////////////////////////////////////////////////////////
//
// camera.cpp
// Camera class
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Game Includes
#include "camera.h"
#include "monkeyking.h"
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
extern DJVector2			g_vScaleScreen;
extern djuint32				g_nScreenWidth;
extern djuint32				g_nScreenHeight;
/////////////////////////////////////////////////////////////////
Camera::Camera()
{
	m_vPos = DJVector2(0.0f);
	m_vPrevTargetPos = DJVector2(0.0f);
	m_vDir = DJVector2(0.0f);
	m_fShakingTime = 1.0f;
	m_fPaddingOffsetX = 0.0f;
	m_fPaddingDesiredOffsetX = 0.0f;
	m_fPaddingSpeedXFactor = 0.0f;

	m_fPaddingOffsetY = 0.0f;
	m_fPaddingDesiredOffsetY = 0.0f;
	m_fPaddingSpeedYFactor = 0.0f;

	DJTrace("%s()", __FUNCTION__);
}

///

Camera::~Camera()
{
}

///

void Camera::Reset()
{
	m_vPrevTargetPos = m_vSpanVel = m_vPos = DJVector2(0.0f, 200.0f);
	m_vDir = DJVector2(0.0f);
	m_fShakingTime = 1.0f;
	m_fPaddingOffsetX = 0.0f;
}

///

void Camera::SetPosition(const DJVector2 &vPos)
{
	m_vPrevTargetPos = m_vPos = vPos;
}

///

void Camera::OnUpdate(float fDeltaTime, const DJVector2 &vTargetPos, const DJVector2 &vYRestriction)
{
	DJVector2 vCalculatedTargetPos(vTargetPos);
	if (vCalculatedTargetPos[1] - (RUNNER_PADDING.e[1] * (float)g_nScreenHeight) < vYRestriction[0])
		vCalculatedTargetPos[1] = vYRestriction[0] + (RUNNER_PADDING.e[1] * (float)g_nScreenHeight);
	if (vCalculatedTargetPos[1] + ((1.0f - RUNNER_PADDING.e[1]) * (float)g_nScreenHeight) > vYRestriction[1])
		vCalculatedTargetPos[1] = vYRestriction[1] - ((1.0f - RUNNER_PADDING.e[1]) * (float)g_nScreenHeight);

	DJVector2 vVel = (vCalculatedTargetPos - m_vPrevTargetPos) / fDeltaTime;
	DJVector2 vVelDiff = vVel - m_vSpanVel;
	vVelDiff = vVelDiff & DJVector2(1.0f, 1.0f);
	m_vSpanVel[0] += vVelDiff[0] * (fDeltaTime / CAMERA_VELOCITY_CATCHUP_TIME.e[0]);
	m_vSpanVel[1] += vVelDiff[1] * (fDeltaTime / CAMERA_VELOCITY_CATCHUP_TIME.e[1]);
	DJVector2 vDist = (vCalculatedTargetPos - m_vPos);
	vDist = vDist & DJVector2(1.0f, 0.3f);
	m_vPos += m_vSpanVel * fDeltaTime;
	m_vPos[0] += vDist[0] * (fDeltaTime / CAMERA_POSITION_CATCHUP_TIME[0]);
	m_vPos[1] += vDist[1] * (fDeltaTime / CAMERA_POSITION_CATCHUP_TIME[1]);
	//m_vPos += vDist * (fDeltaTime / CAMERA_CATCHUP_TIME);
	m_vPrevTargetPos = vCalculatedTargetPos;

	m_fShakingTime += fDeltaTime;
	if(m_fShakingTime < 1.0f)
	{
		float fTran = m_fShakingTime * 20.0f;
		DJVector2 vTran(5.0f * g_vScaleScreen[0] * djSin(fTran * DJ_PIf), 5.0f * g_vScaleScreen[1] * djSin(fTran * DJ_PIf));
		vTran = vTran & m_vDir;
		if (fTran > 1.0f)
		{
			fTran = 1.0f;
			vTran = DJVector2(0.0f);
		}
		DJMatrix2D tmmm;
		tmmm = theSpriteEngine.GetGlobalTransform();
		tmmm.SetTranslation(vTran);
		theSpriteEngine.SetGlobalTransform(tmmm);
	}

	// Padding
	m_fPaddingOffsetX += djStepToDesiredValue(m_fPaddingOffsetX, m_fPaddingDesiredOffsetX, fDeltaTime * m_fPaddingSpeedXFactor);

	if(m_vPos[1] < vYRestriction[1] / 1.5f)
		m_fPaddingOffsetY += djStepToDesiredValue(m_fPaddingOffsetY, 0.075f, fDeltaTime * 0.01f);
	else
		m_fPaddingOffsetY += djStepToDesiredValue(m_fPaddingOffsetY, 0.0f, fDeltaTime * 0.01f);
		//m_fPaddingOffsetY = 0.075f;
}

///

DJMatrix2D Camera::GetTransform()
{
	return DJMatrix2D::Translate(DJVector2(-m_vPos.e[0] + ((RUNNER_PADDING.e[0] + m_fPaddingOffsetX) * (float)g_nScreenWidth),
										   -m_vPos.e[1] + ((RUNNER_PADDING.e[1] + m_fPaddingOffsetY) * (float)g_nScreenHeight)));
}

///

DJMatrix2D Camera::GetTransform(const DJVector2 &vDriftRatio)
{
	return DJMatrix2D::Translate(DJVector2(-m_vPos.e[0] + ((RUNNER_PADDING.e[0] + m_fPaddingOffsetX) * (float)g_nScreenWidth),
										   -m_vPos.e[1] + ((RUNNER_PADDING.e[1] + m_fPaddingOffsetY) * (float)g_nScreenHeight))
								 & vDriftRatio);
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////