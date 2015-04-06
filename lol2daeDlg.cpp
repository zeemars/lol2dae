
// lol2daeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lol2dae.h"
#include "lol2daeDlg.h"

#include "SknImporter.h"
#include "SklImporter.h"
#include "AnmImporter.h"
#include "ColladaWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clol2daeDlg dialog



Clol2daeDlg::Clol2daeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Clol2daeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Clol2daeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITINPUT, m_Mesh);
	DDX_Control(pDX, IDC_EDITOUTPUT, m_Output);
	DDX_Control(pDX, IDC_CHECKSKL, m_IncludeSkeleton);
	DDX_Control(pDX, IDC_EDITANIMATION, m_Animation);
}

BEGIN_MESSAGE_MAP(Clol2daeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONCONVERT, &Clol2daeDlg::OnBnClickedButtonconvert)
	ON_BN_CLICKED(IDC_BUTTONINPUT, &Clol2daeDlg::OnBnClickedButtonmesh)
	ON_BN_CLICKED(IDC_BUTTONOUTPUT, &Clol2daeDlg::OnBnClickedButtonoutput)
	ON_BN_CLICKED(IDC_BUTTONANIMATION, &Clol2daeDlg::OnBnClickedButtonanimation)
	ON_BN_CLICKED(IDC_CHECKSKL, &Clol2daeDlg::OnBnClickedCheckskl)
END_MESSAGE_MAP()


// Clol2daeDlg message handlers

BOOL Clol2daeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_DialogBgBrush = CreateSolidBrush(RGB(128, 128, 128));
	m_EditCtrlBrush = CreateSolidBrush(RGB(86, 86, 86));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Clol2daeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Clol2daeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH Clol2daeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_MSGBOX:
		pDC->SetTextColor(RGB(234, 234, 234));
		pDC->SetBkColor(RGB(86, 86, 86));
		return (m_EditCtrlBrush);

	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(234, 234, 234));
		pDC->SetBkColor(RGB(128, 128, 128));
		return m_DialogBgBrush;

	case CTLCOLOR_DLG:
		return m_DialogBgBrush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void Clol2daeDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	CDC dc;
	RECT rect;
	dc.Attach(lpDrawItemStruct->hDC);

	rect = lpDrawItemStruct->rcItem;

	UINT state = lpDrawItemStruct->itemState;

	if ((state & ODS_SELECTED))
	{
		dc.FillSolidRect(&rect, RGB(86, 86, 86));
		dc.Draw3dRect(&rect, RGB(64, 64, 64), RGB(168, 168, 168));
	}

	else
	{
		dc.FillSolidRect(&rect, RGB(96, 96, 96));
		dc.Draw3dRect(&rect, RGB(168, 168, 168), RGB(64, 64, 64));
	}

	dc.SetBkColor(RGB(96, 96, 96));
	dc.SetTextColor(RGB(234, 234, 234));

	TCHAR buffer[MAX_PATH];
	ZeroMemory(buffer, MAX_PATH);
	::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);

	dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	dc.Detach();

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void Clol2daeDlg::OnBnClickedButtonconvert()
{
	AfxBeginThread(convertWorkerThread, this);
}


void Clol2daeDlg::OnBnClickedButtonmesh()
{
	CStringW fileName;
	wchar_t* buffer = fileName.GetBuffer(MAX_PATH);

	CFileDialog openFileDlg(TRUE);
	OPENFILENAME& ofn = openFileDlg.GetOFN();
	ofn.Flags |= OFN_FILEMUSTEXIST;
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"League of Legends (*.skn)\0*.skn\0All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = L"skn";

	if (openFileDlg.DoModal() == IDOK)
	{
		m_Mesh.SetWindowTextW(fileName);
		FileLocation defaultOutputPath;

		_wsplitpath_s(fileName, defaultOutputPath.Drive, defaultOutputPath.Directory, defaultOutputPath.Name, defaultOutputPath.Extension);
		_wmakepath_s(defaultOutputPath.Path, defaultOutputPath.Drive, defaultOutputPath.Directory, defaultOutputPath.Name, L"dae");
		m_Output.SetWindowTextW(defaultOutputPath.Path);
	}

	fileName.ReleaseBuffer();
}


void Clol2daeDlg::OnBnClickedButtonoutput()
{
	CStringW fileName;
	wchar_t* buffer = fileName.GetBuffer(MAX_PATH);

	CFileDialog openFileDlg(FALSE);
	OPENFILENAME& ofn = openFileDlg.GetOFN();
	ofn.Flags |= OFN_OVERWRITEPROMPT;
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"COLLADA (*.dae)\0*.dae\0All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = L"dae";

	if (openFileDlg.DoModal() == IDOK)
	{
		m_Output.SetWindowTextW(fileName);
	}

	fileName.ReleaseBuffer();
}

UINT Clol2daeDlg::convertWorkerThread(LPVOID lParam)
{
	Clol2daeDlg* dlg = (Clol2daeDlg*)lParam;

	CStringW sknPath;
	dlg->m_Mesh.GetWindowTextW(sknPath);

	CStringW anmPath;
	dlg->m_Animation.GetWindowTextW(anmPath);

	CStringW outputPath; 
	dlg->m_Output.GetWindowTextW(outputPath);

	bool includeSkl = (dlg->m_IncludeSkeleton.GetCheck() == 1) ? true : false;
	bool includeAnm = false;

	try
	{
		SknImporter inputSkn;
		inputSkn.invertTextureV = true;
		inputSkn.readFile(sknPath);

		SklImporter inputSkl(inputSkn.fileVersion);
		AnmImporter inputAnm(inputSkl.boneHashes);

		if (includeSkl)
		{
			FileLocation sklPath;
			_wsplitpath_s(sknPath, sklPath.Drive, sklPath.Directory, sklPath.Name, sklPath.Extension);
			_wmakepath_s(sklPath.Path, sklPath.Drive, sklPath.Directory, sklPath.Name, L"skl");

			inputSkl.readFile(sklPath.Path);

			if (anmPath.Compare(L"") != 0)
			{
				includeAnm = true;
				inputAnm.readFile(anmPath);
			}
		}

		ColladaWriter outputCollada(inputSkn.indices, inputSkn.vertices, inputSkl.bones, inputSkl.boneIndices, inputAnm);
		outputCollada.includeSkl = includeSkl;
		outputCollada.includeAnm = includeAnm;
		outputCollada.writeFile(outputPath);

		dlg->MessageBox(L"File was written successfully.", L"Information", MB_ICONINFORMATION);
	}

	catch (lol2daeError& e)
	{
		dlg->MessageBox((CStringW)e.what(), L"Error", MB_ICONERROR);
	}

	return 0;
}

void Clol2daeDlg::OnBnClickedButtonanimation()
{
	CStringW fileName;
	wchar_t* buffer = fileName.GetBuffer(MAX_PATH);

	CFileDialog openFileDlg(TRUE);
	OPENFILENAME& ofn = openFileDlg.GetOFN();
	ofn.Flags |= OFN_FILEMUSTEXIST;
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"League of Legends (*.anm)\0*.anm\0All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = L"anm";

	if (openFileDlg.DoModal() == IDOK)
	{
		m_Animation.SetWindowTextW(fileName);
	}

	fileName.ReleaseBuffer();
}


void Clol2daeDlg::OnBnClickedCheckskl()
{
	if (m_IncludeSkeleton.GetCheck() == 1)
	{
		m_Animation.EnableWindow(TRUE);

		CWnd* CWnd = GetDlgItem(IDC_BUTTONANIMATION);
		CWnd->EnableWindow(TRUE);
	}

	else
	{
		m_Animation.EnableWindow(FALSE);

		CWnd* CWnd = GetDlgItem(IDC_BUTTONANIMATION);
		CWnd->EnableWindow(FALSE);
	}
}
