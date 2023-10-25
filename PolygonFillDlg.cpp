
// PolygonFillDlg.cpp : implementation file
//
#include <algorithm>

#include "pch.h"
#include "framework.h"
#include "PolygonFill.h"
#include "PolygonFillDlg.h"
#include "afxdialogex.h"

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


// CPolygonFillDlg dialog


// 构造函数初始化
CPolygonFillDlg::CPolygonFillDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POLYGONFILL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	IsCompleted = FALSE;
	points.clear();
	YMAX = 0;
}

void CPolygonFillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// 消息关联
BEGIN_MESSAGE_MAP(CPolygonFillDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()

	ON_WM_LBUTTONDOWN() // 关联鼠标左键点击事件
	ON_WM_RBUTTONDOWN() // 关联鼠标右键点击事件
	ON_BN_CLICKED(IDC_BUTTON_FILL, &PolygonFill) // 关联按钮点击事件

END_MESSAGE_MAP()


// CPolygonFillDlg message handlers

BOOL CPolygonFillDlg::OnInitDialog()
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

void CPolygonFillDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPolygonFillDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/***************************鼠标点击多边形绘制***************************/

// 鼠标左键点击事件（绘制多边形）
void CPolygonFillDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 若多边形绘制完毕，忽略该函数，不执行操作
	if (IsCompleted == TRUE) 
	{
		return;
	}

	else 
	{
		// 如果是第一个点，存入points数组中
		if (points.empty()) 
		{
			points.push_back(point);
		}

		// 若不是第一个点，则连接当前点和上一个点
		else 
		{
			// 获取画板
			CStatic* CANVAS = (CStatic*)GetDlgItem(IDC_STATIC_CANVAS);
			CDC* pDC = CANVAS->GetDC();

			// 获取points数组的最后一个point（就是上一个点），并将当前绘图位置移动到该点
			pDC->MoveTo(points[points.size() - 1]);
			// 连接上一个点和当前点
			pDC->LineTo(point);

			points.push_back(point); // 将当前点加入数组

			ReleaseDC(pDC);
		}
	}
}

// 鼠标右键点击事件（多边形绘制完毕）
void CPolygonFillDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(IsCompleted == TRUE) 
	{
		return;
	}

	else 
	{
		// 如果前面少于3个点，则无法连接成为多边形，弹出错误信息
		if (points.size() < 3) {
			CString message = _T("顶点数量少于3，无法绘制多边形");
			CString caption = _T("多边形绘制错误");
			MessageBox(message, caption, MB_ICONERROR);
		}

		// 若points已存储有2个以上的顶点，则连接上一个点和第一个点
		else 
		{
			// 获取画板
			CStatic* CANVAS = (CStatic*)GetDlgItem(IDC_STATIC_CANVAS);
			CDC* pDC = CANVAS->GetDC();

			// 将绘图位置移动到上一个点
			pDC->MoveTo(points[points.size() - 1]);
			// 连接第一个点和当前点
			pDC->LineTo(points[0]);

			ReleaseDC(pDC);

			// 成功消息
			CString message = _T("多边形绘制完毕");
			CString caption = _T("多边形绘制完毕");
			MessageBox(message, caption, MB_OK);

			IsCompleted = TRUE;
		}
	}
}


/**************************扫描线填充****************************/

// Edge类构造函数
Edge::Edge(float x_val, float dx_val, float y_val) 
{
	x_lower = x_val;
	dx = dx_val;
	y_upper = y_val;
}

// 比较函数，按照CPoint对象的x值升序排列
bool CPolygonFillDlg::compareCPoint(const CPoint& a, const CPoint& b) {
	return a.x < b.x;
}

void CPolygonFillDlg::getET() 
{
	for (int i = 1; i <= points.size() ; i++) 
	{
		float dx;
		float x_lower, y_lower, y_upper;

		// points中最后一个点和第一个点的连线
		if (i == points.size())
		{
			if (points[i - 1].x == points[0].x)
			{
				continue; // 边平行于x轴，不处理
			}
			else if (points[i - 1].y == points[0].y)
			{
				dx = 0.0;
			}
			else
			{
				dx = (points[i - 1].x - points[0].x) * 1.0 / (points[i - 1].y - points[0].y);
			}

			if (points[0].y <= points[i - 1].y)
			{
				y_lower = points[0].y;
				y_upper = points[i - 1].y;
				x_lower = points[0].x;
			}
			else
			{
				y_lower = points[i - 1].y;
				y_upper = points[0].y;
				x_lower = points[i - 1].x;
			}
			// 插入到Edge边信息中
			Edge e(x_lower, dx, y_upper);
			// 插入到ET表中
			ET[y_lower].push_back(e);
		}

		else 
		{
			// 斜率
			if (points[i - 1].x == points[i].x)
			{
				continue; // 边平行于x轴，不处理
			}
			else if (points[i - 1].y == points[i].y)
			{
				dx = 0;
			}
			else
			{
				dx = (points[i - 1].x - points[i].x) * 1.0 / (points[i - 1].y - points[i].y);
			}

			// 线段上端点的x，y值
			if (points[i - 1].y <= points[i].y)
			{
				y_lower = points[i - 1].y;
				y_upper = points[i].y;
				x_lower = points[i - 1].x;
			}
			else
			{
				y_lower = points[i].y;
				y_upper = points[i - 1].y;
				x_lower = points[i].x;
			}

			// 计算多边形的最高点y值
			if (y_upper >= YMAX)
			{
				YMAX = y_upper;
			}

			// 插入到Edge边信息中
			Edge e(x_lower, dx, y_upper);
			// 插入到ET表中
			ET[y_lower].push_back(e);
		}
	}
}

void CPolygonFillDlg::PolygonFill()
{
	// 获取ET表
	getET();

	// 从ymin到ymax进行扫描，将y初始化为ymin
	float y = ET.begin()->first;

	auto it_ET = ET.begin(); // AET初始化为ET表中ymin映射的边
	AET = (it_ET++)->second;

	while (y < YMAX)
	{
		// 移除已经超出扫描线的边
		std::vector<Edge> newAET;
		for (auto it = AET.begin(); it != AET.end(); ++it) {
			if (it->y_upper > y) {
				newAET.push_back(*it);
			}
		}
		AET = newAET;

		//检查是否有新的边加入
		if (it_ET != ET.end() && y >= it_ET->first) {
			AET.insert(AET.begin(), it_ET->second.begin(), it_ET->second.end());
			it_ET++;
		}

		// 获取活动边与当前扫描线的所有交点
		auto it = AET.begin();
		while (it != AET.end())
		{
			float x1 = it->x_lower;
			it->x_lower += it->dx;
			it++;

			if (it != AET.end())
			{
				float x2 = it->x_lower;
				it->x_lower += it->dx;
				it++;

				// 扫描线遇到两条线相交的顶点时，不记录交点
				if (x1 != x2)
				{
					scanPoints.push_back(CPoint(x1, y));
					scanPoints.push_back(CPoint(x2, y));
				}
			}
		}

		// 对扫描到的所有交点进行升序排列
		std::sort(scanPoints.begin(), scanPoints.end(), compareCPoint);

		// 获取画板
		CStatic* CANVAS = (CStatic*)GetDlgItem(IDC_STATIC_CANVAS);
		CDC* pDC = CANVAS->GetDC();
		// 成对绘制，i每次+2
		for (int i = 0; i < scanPoints.size(); i += 2)
		{
			pDC->MoveTo(scanPoints[i]);
			pDC->LineTo(scanPoints[i + 1]);
		}
		ReleaseDC(pDC);

		// 当前扫描线已绘制完毕，清空交点，继续扫描
		scanPoints.clear();
		y += 1.0;
	}
}