
// BezierCurve.h : header file
//

#pragma once
#include <vector>

// BezierCurve dialog
class BezierCurve : public CDialogEx
{
	// Construction
public:
	BezierCurve(CWnd* pParent = nullptr);	// standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BEZIER_DIALOG };
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	std::vector<CPoint> points;
	bool IsCompleted;
	CPoint controlPoints[4][4];  // 16个控制点
	CPoint surfacePoints[50][50];

	CPoint BezierCurvePoint(CPoint p0, CPoint p1, CPoint p2, double t);
	void DrawBezierCurve();
	void GenerateControlPoints();
	int Combination(int n, int k);
	CPoint BezierSurfacePoint(float u, float v);
	void DrawBezierSurface();
};
