#pragma once

class CCustomEdit :
	public CEdit
{
public:
	CCustomEdit();
	~CCustomEdit();

protected:
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnNcPaint();
};

