
// BezierCurve.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Bezier.h"
#include "BezierCurve.h"
#include "afxdialogex.h"
#include <cstdlib>  // 包含随机数生成相关的头文件
#include <ctime>    // 包含时间头文件，用于初始化随机数生成器
#include <cmath>    // 包含数学函数头文件，用于生成随机坐标

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// BezierCurve dialog

BezierCurve::BezierCurve(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BEZIERCURVE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	IsCompleted = false;
	points.clear();
}

void BezierCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(BezierCurve, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &BezierCurve::DrawBezierSurface)
END_MESSAGE_MAP()


// BezierCurve message handlers

BOOL BezierCurve::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void BezierCurve::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void BezierCurve::OnPaint()
{
	DrawBezierCurve();
	//DrawBezierSurface();
}

void BezierCurve::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 若多边形绘制完毕，忽略该函数，不执行操作
	if (IsCompleted)
	{
		return;
	}
	else
	{
		// 将点添加到points数组中
		points.push_back(point);

		// 若不是第一个点，则连接当前点和上一个点
		if (points.size() > 1)
		{
			CStatic* CANVAS = (CStatic*)GetDlgItem(IDC_STATIC_CANVAS);
			CDC* pDC = CANVAS->GetDC();
			pDC->MoveTo(points[points.size() - 2]);
			pDC->LineTo(point);
			ReleaseDC(pDC);
		}
	}
}

void BezierCurve::OnRButtonDown(UINT nFlags, CPoint point)
{
	IsCompleted = true;
	return;
}

CPoint BezierCurve::BezierCurvePoint(CPoint p0, CPoint p1, CPoint p2, double t)
{
	double x = 0.0;
	double y = 0.0;
	x = (1 + t * t - 2 * t) * p0.x + 2 * (t - t * t) * p1.x + t * t * p2.x;
	y = (1 + t * t - 2 * t) * p0.y + 2 * (t - t * t) * p1.y + t * t * p2.y;
	CPoint point((int)x, (int)y);
	return point;
}

void BezierCurve::DrawBezierCurve()
{
	if (IsCompleted && points.size() >= 3)
	{
		double t = 0.0;
		CStatic* CANVAS = (CStatic*)GetDlgItem(IDC_STATIC_CANVAS);
		CDC* pDC = CANVAS->GetDC();

		CPoint tempPoint = BezierCurvePoint(points[0], points[1], points[2], t);
		CPoint oldPoint = tempPoint;

		while (t <= 1)
		{
			tempPoint = BezierCurvePoint(points[0], points[1], points[2], t);
			pDC->MoveTo(oldPoint);
			pDC->LineTo(tempPoint);
			oldPoint = tempPoint;
			t += 0.01;
		}

		ReleaseDC(pDC);
	}
}


void BezierCurve::GenerateControlPoints()
{
	CRect Area;
	GetDlgItem(IDC_STATIC_CANVAS_SURFACE)->GetClientRect(Area);
	int width = Area.Width();
	int height = Area.Height();
	std::srand(static_cast<unsigned int>(std::time(0)));
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			int x = std::rand() % (width + 1);
			int y = std::rand() % (height + 1);
			controlPoints[i][j] = CPoint(x, y);
		}
	}
}

int BezierCurve::Combination(int n, int k)
{
	if (k == 0 || k == n)
		return 1;
	else
		return Combination(n - 1, k - 1) + Combination(n - 1, k);
}

CPoint BezierCurve::BezierSurfacePoint(float u, float v)
{
	float x = 0.0;
	float y = 0.0;
	// 计算双三次贝塞尔曲面上的点坐标
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			float coef = float(Combination(3, i) * Combination(3, j) * pow(u, i) * pow(1 - u, 3 - i) * pow(v, j) * pow(1 - v, 3 - j));
			x += controlPoints[i][j].x * coef;
			y += controlPoints[i][j].y * coef;
		}
	}
	return CPoint(x, y);
}

void BezierCurve::DrawBezierSurface()
{
	CStatic* CANVAS = (CStatic*)GetDlgItem(IDC_STATIC_CANVAS_SURFACE);
	CDC* pDC = CANVAS->GetDC();

	CPen pen(PS_SOLID, 1, RGB(1, 0, 0));
	pDC->SelectObject(&pen);
	// 产生随机控制点
	GenerateControlPoints();
	// 计算并绘制曲面
	int i = 0, j = 0;
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			float u = i / 50.0;  // 将i映射到范围[0, 1]
			float v = j / 50.0;  // 将j映射到范围[0, 1]
			surfacePoints[i][j] = BezierSurfacePoint(u, v);
		}
	}
	for (i = 1; i < 50; i++)
	{
		for (j = 1; j < 50; j++)
		{
			pDC->MoveTo(surfacePoints[i - 1][j - 1]);
			pDC->LineTo(surfacePoints[i - 1][j]);
			pDC->LineTo(surfacePoints[i][j]);
			pDC->LineTo(surfacePoints[i][j - 1]);
		}
	}
	ReleaseDC(pDC);
}