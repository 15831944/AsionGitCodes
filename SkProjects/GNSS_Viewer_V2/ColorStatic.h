#pragma once


// CColorStatic

class CColorStatic : public CStatic
{
	DECLARE_DYNAMIC(CColorStatic)

public:
	CColorStatic();
	virtual ~CColorStatic();

	void	SetBgColor(COLORREF crColor)
	{ 
		if(m_pBgBrush) delete m_pBgBrush;
		m_pBgBrush = new CBrush(crColor);
	}
	void	SetTextColor(COLORREF crColor)
	{	m_crTextColor = crColor; }
protected:
	DECLARE_MESSAGE_MAP()

	CBrush*	m_pTextBrush;
	CBrush*	m_pBgBrush;
	COLORREF m_crTextColor;


public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnSetText(WPARAM,LPARAM);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


