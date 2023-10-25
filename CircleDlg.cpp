
// CircleDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Circle.h"
#include "CircleDlg.h"
#include "afxdialogex.h"
#include <cmath>

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


// CCircleDlg dialog



CCircleDlg::CCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CIRCLE, &CCircleDlg::ButtonCircle)
	ON_BN_CLICKED(IDC_BUTTON_ELLIPSOID, &CCircleDlg::ButtonEllipsoid)
END_MESSAGE_MAP()


// CCircleDlg message handlers

BOOL CCircleDlg::OnInitDialog()
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

void CCircleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCircleDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*************以下为绘制圆和椭圆的全部函数****************/

// 点击按钮开始绘制圆
void CCircleDlg::ButtonCircle() {

	// 从编辑框中获取输入的半径并转换为整型
	CString Radius;
	GetDlgItemText(IDC_EDIT_CIRCLE_R, Radius);
	int R = _ttoi(Radius);

	// 获取绘图矩形
	CRect AreaCircle;
	GetDlgItem(IDC_STATIC_AREA_CIRCLE)->GetClientRect(AreaCircle);

	// 检查是否会越界并绘制
	if (R > 0 && R <= AreaCircle.Width()/2 && R <= AreaCircle.Height()/2) {
		DrawCircle(R);
	}
	else {
		CString message = _T("半径超出绘图框的边界，请重新输入");
		CString caption = _T("圆绘制错误");
		MessageBox(message, caption, MB_ICONERROR);
	}

}

// 画圆
void CCircleDlg::DrawCircle(int r) {

	// 获取画板
	CStatic* Area = (CStatic*)GetDlgItem(IDC_STATIC_AREA_CIRCLE);
	CDC* pdc = Area->GetDC();

	// 获取绘图矩形
	CRect AreaCircle;
	GetDlgItem(IDC_STATIC_AREA_CIRCLE)->GetClientRect(AreaCircle);
	double dx = AreaCircle.Width() / 2.0;
	double dy = AreaCircle.Height() / 2.0;

	// 初始化P0和起始点
	double p = 5.0 / 4 - r;
	int x = 0, y = r;

	// 先绘制1/8个圆弧（第一象限上半部分）,并根据轴对称绘制另外3个1/8圆弧
	for (int x = 0; x <= y; ) {

		// 绘制当前像素点
		pdc->SetPixel(x + dx, y + dy, RGB(0, 0, 0));

		// 轴对称绘制其他像素点
		pdc->SetPixel(-x + dx, y + dy, RGB(0, 0, 0));
		pdc->SetPixel(-x + dx, -y + dy, RGB(0, 0, 0));
		pdc->SetPixel(x + dx, -y + dy, RGB(0, 0, 0));

		// 根据Pk的值决定下一个像素点
		if (p < 0) {
			x += 1;
			p = p + 2.0 * x + 1;
		}
		else {
			x += 1;
			y -= 1;
			p = p + 2.0 * (x - y) + 1;
		}
	}
	
	// 绘制第一象限下半部分的1/8圆弧，并根据轴对称绘制剩下3个1/8圆弧
	// 起始点选取(r,0)，重新初始化起始点和P0
	x = r, y = 0;
	p = 5.0 / 4 - r;

	for (y = 0; y <= x; ) {

		// 绘制当前像素点
		pdc->SetPixel(x + dx, y + dy, RGB(0, 0, 0));

		// 轴对称绘制其他像素点
		pdc->SetPixel(-x + dx, y + dy, RGB(0, 0, 0));
		pdc->SetPixel(-x + dx, -y + dy, RGB(0, 0, 0));
		pdc->SetPixel(x + dx, -y + dy, RGB(0, 0, 0));

		// 根据Pk的值决定下一个像素点
		if (p <= 0) {
			y += 1;
			p = p + 2 * y + 1;
		}
		else {
			x -= 1;
			y += 1;
			p = p + 2 * (y - x) - 1;
		}
	}

	// 释放设备
	Area->ReleaseDC(pdc);

}

// 椭圆的生成按钮
void CCircleDlg::ButtonEllipsoid() {

	// 从编辑框中获取输入的长短轴并转换为整型
	CString xAxis, yAxis;
	GetDlgItemText(IDC_EDIT_ELLPISOID_A, xAxis);
	GetDlgItemText(IDC_EDIT_ELLPISOID_B, yAxis);
	int A = _ttoi(xAxis); // x轴方向，记为长轴
	int B = _ttoi(yAxis); // y轴方向，记为短轴

	// 获取绘图区
	CRect AreaEllipsoid;
	GetDlgItem(IDC_STATIC_AREA_ELLIPSOID)->GetClientRect(AreaEllipsoid);

	// 检查是否会越界并绘制
	if (A > 0 && B > 0 && A <= AreaEllipsoid.Width()/2 && B <= AreaEllipsoid.Height()/2) {
		DrawEllipsoid(A, B);
	}
	else {
		CString message = _T("轴长超出绘图框的边界，请重新输入");
		CString caption = _T("椭圆绘制错误");
		MessageBox(message, caption, MB_ICONERROR);
	}

}

// 画椭圆
void CCircleDlg::DrawEllipsoid(int a, int b) {
	
	// 获取画板
	CStatic* Area = (CStatic*)GetDlgItem(IDC_STATIC_AREA_ELLIPSOID);
	CDC* pdc = Area->GetDC();

	// 获取绘图矩形
	CRect AreaEllipsoid;
	GetDlgItem(IDC_STATIC_AREA_ELLIPSOID)->GetClientRect(AreaEllipsoid);
	double dx = AreaEllipsoid.Width() / 2.0;
	double dy = AreaEllipsoid.Height() / 2.0;

	// 初始化P0和起始点(0,b)
	double p = b * b - a * a * b + (1.0 / 4) * a * a;
	int x = 0, y = b;

	// 先绘制第一象限上半部分的椭圆弧,并根据轴对称绘制另外3个椭圆弧
	for (int x = 0; b * b * x <= a * a * y; ) {
		// 绘制当前像素点
		pdc->SetPixel(x + dx, y + dy, RGB(0, 0, 0));

		// 轴对称绘制其他像素点
		pdc->SetPixel(-x + dx, y + dy, RGB(0, 0, 0));
		pdc->SetPixel(-x + dx, -y + dy, RGB(0, 0, 0));
		pdc->SetPixel(x + dx, -y + dy, RGB(0, 0, 0));

		// 根据Pk的值决定下一个像素点
		if (p < 0) {
			x += 1;
			p = p + b * b * (2 * x + 1);
		}
		else {
			x += 1;
			y -= 1;
			p = p + b * b * (2 * x + 1) - 2 * a * a * y;
		}
	}

	// 绘制第一象限下半部分的椭圆弧，并根据轴对称绘制剩下3个
	// 起始点选取(a,0)，重新初始化起始点和P0
	x = a, y = 0;
	p = a * a - b * b * a + 0.25 * b * b;

	for (y = 0; a * a * y <= b * b * x; ) {

		// 绘制当前像素点
		pdc->SetPixel(x + dx, y + dy, RGB(0, 0, 0));

		// 轴对称绘制其他像素点
		pdc->SetPixel(-x + dx, y + dy, RGB(0, 0, 0));
		pdc->SetPixel(-x + dx, -y + dy, RGB(0, 0, 0));
		pdc->SetPixel(x + dx, -y + dy, RGB(0, 0, 0));

		// 根据Pk的值决定下一个像素点
		if (p < 0) {
			y += 1;
			p = p + a * a * (2 * y + 1);
		}
		else {
			x -= 1;
			y += 1;
			p = p + a * a * (2 * y + 1) - 2 * b * b * x;
		}
	}

	// 释放设备
	Area->ReleaseDC(pdc);

}