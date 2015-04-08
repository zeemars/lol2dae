#include "stdafx.h"
#include "CustomEdit.h"

BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

void CCustomEdit::OnNcPaint()
{
	CDC* pDC = GetWindowDC();

	CRect rect;
	GetWindowRect(&rect);
	rect.OffsetRect(-rect.left, -rect.top);
	++rect.top;
	++rect.left;
	--rect.bottom;
	--rect.right;

	pDC->Draw3dRect(&rect, RGB(32, 32, 32), RGB(168, 168, 168));
	ReleaseDC(pDC);
}

CCustomEdit::CCustomEdit()
{
}


CCustomEdit::~CCustomEdit()
{
}
