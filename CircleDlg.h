
// CircleDlg.h : header file
//

#pragma once


// CCircleDlg dialog
class CCircleDlg : public CDialogEx
{
// Construction
public:
	CCircleDlg(CWnd* pParent = nullptr);	// standard constructor
	void ButtonCircle();
	void ButtonEllipsoid();
	void DrawCircle(int r);
	void DrawEllipsoid(int a, int b);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
