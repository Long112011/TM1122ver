#pragma once
struct SMovingChild
{
	HWND        m_hWnd;
	double      m_dXMoveFrac;
	double      m_dYMoveFrac;
	double      m_dXSizeFrac;
	double      m_dYSizeFrac;
	CRect       m_rcInitial;
};
struct SMovingChildTest
{
	HWND        m_hWnd;
	double      m_dXMoveFrac;
	double      m_dYMoveFrac;
	double      m_dXSizeFrac;
	double      m_dYSizeFrac;
	CRect       m_rcInitial;
};