
// lol2daeDlg.h : header file
//

#pragma once
#include "afxwin.h"

// Clol2daeDlg dialog
class Clol2daeDlg : public CDialog
{
// Construction
public:
	Clol2daeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOL2DAE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedButtonconvert();
	afx_msg void OnBnClickedButtonmesh();
	afx_msg void OnBnClickedButtonoutput();
	afx_msg void OnBnClickedButtonanimation();
	afx_msg void OnBnClickedCheckskl();
	DECLARE_MESSAGE_MAP()

private:
	HBRUSH m_DialogBgBrush;
	HBRUSH m_EditCtrlBrush;
	CCustomEdit m_Mesh;
	CCustomEdit m_Animation;
	CCustomEdit m_Output;
	CButton m_IncludeSkeleton;

	static UINT convertWorkerThread(LPVOID lParam);
public:


};
