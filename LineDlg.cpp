// LineDlg.cpp

#include "LineDlg.h"
#include "Line.h"
#include "afxdialogex.h"
#include "framework.h"
#include "pch.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx {
  public:
    CAboutDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

  protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support

    // Implementation
  protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CLineDlg dialog

CLineDlg::CLineDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_LINE_DIALOG, pParent) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLineDlg::DoDataExchange(CDataExchange *pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLineDlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_DDA, &CLineDlg::OnBnClickedButtonDDA)
ON_BN_CLICKED(IDC_BUTTON_BRESENHAM, &CLineDlg::OnBnClickedButtonBresenham)
END_MESSAGE_MAP()

// CLineDlg message handlers

BOOL CLineDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu *pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr) {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty()) {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);  // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // TODO: Add extra initialization here

    return TRUE; // return TRUE  unless you set the focus to a control
}

void CLineDlg::OnSysCommand(UINT nID, LPARAM lParam) {
    if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    } else {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CLineDlg::OnPaint() {
    if (IsIconic()) {
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
    } else {
        CDialogEx::OnPaint();
    }
}

HCURSOR CLineDlg::OnQueryDragIcon() {
    return static_cast<HCURSOR>(m_hIcon);
}

// 点击DDA按钮将执行的操作
void CLineDlg::OnBnClickedButtonDDA() {
    // 从编辑框中获取坐标
    CString strX0, strY0, strX1, strY1;
    // 起点坐标
    GetDlgItemText(IDC_EDIT_START1, strX0);
    GetDlgItemText(IDC_EDIT_START2, strY0);
    // 终点坐标
    GetDlgItemText(IDC_EDIT_END1, strX1);
    GetDlgItemText(IDC_EDIT_END2, strY1);

    int startX = _ttoi(strX0);
    int startY = _ttoi(strY0);
    int endX = _ttoi(strX1);
    int endY = _ttoi(strY1);

    // 获取绘图框的边界矩形
    CRect rectDrawArea1;
    GetDlgItem(IDC_STATIC_DRAW_AREA1)->GetClientRect(rectDrawArea1);

    // 映射坐标到绘图框的坐标系
    startX -= rectDrawArea1.left;
    startY -= rectDrawArea1.top;
    endX -= rectDrawArea1.left;
    endY -= rectDrawArea1.top;

    // 在绘制直线之前检查坐标是否在绘图框内
    if (startX >= 0 && startX < rectDrawArea1.Width() &&
        startY >= 0 && startY < rectDrawArea1.Height() &&
        endX >= 0 && endX < rectDrawArea1.Width() &&
        endY >= 0 && endY < rectDrawArea1.Height()) {
        // 坐标在绘图框内，可以绘制直线
        // 调用DDA函数
        DrawLineDDA(startX, startY, endX, endY);
    } else {
        // 坐标超出了绘图框的边界，错误提示
        CString message = _T("坐标超出绘图框的边界，请重新输入");
        CString caption = _T("错误");
        MessageBox(message, caption, MB_ICONERROR);
    }
}

// 点击Bresenham按钮将执行的操作
void CLineDlg::OnBnClickedButtonBresenham() {
    // 从编辑框中获取坐标
    CString strX0, strY0, strX1, strY1;
    // 起点坐标
    GetDlgItemText(IDC_EDIT_START1, strX0);
    GetDlgItemText(IDC_EDIT_START2, strY0);
    // 终点坐标
    GetDlgItemText(IDC_EDIT_END1, strX1);
    GetDlgItemText(IDC_EDIT_END2, strY1);

    int startX = _ttoi(strX0);
    int startY = _ttoi(strY0);
    int endX = _ttoi(strX1);
    int endY = _ttoi(strY1);

    // 获取绘图框的边界矩形
    CRect rectDrawArea2;
    GetDlgItem(IDC_STATIC_DRAW_AREA2)->GetClientRect(rectDrawArea2);

    // 映射坐标到绘图框的坐标系
    startX -= rectDrawArea2.left;
    startY -= rectDrawArea2.top;
    endX -= rectDrawArea2.left;
    endY -= rectDrawArea2.top;

    // 在绘制直线之前检查坐标是否在绘图框内
    if (startX >= 0 && startX < rectDrawArea2.Width() &&
        startY >= 0 && startY < rectDrawArea2.Height() &&
        endX >= 0 && endX < rectDrawArea2.Width() &&
        endY >= 0 && endY < rectDrawArea2.Height()) {
        // 坐标在绘图框内，可以绘制直线
        // 调用Bresenham函数
        DrawLineBresenham(startX, startY, endX, endY);
    } else {
        // 坐标超出了绘图框的边界，错误提示
        CString message = _T("坐标超出绘图框的边界，请重新输入");
        CString caption = _T("错误");
        MessageBox(message, caption, MB_ICONERROR);
    }
}

// DDA算法
void CLineDlg::DrawLineDDA(int x1, int y1, int x2, int y2) {
    CDC *pDC = GetDlgItem(IDC_STATIC_DRAW_AREA1)->GetDC();

    int dx = x2 - x1;
    int dy = y2 - y1;
    int L = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float dx = static_cast<float>(dx) / static_cast<float>(L);
    float dy = static_cast<float>(dy) / static_cast<float>(L);

    float x = static_cast<float>(x1);
    float y = static_cast<float>(y1);

    pDC->MoveTo(x1, y1);

    for (int i = 0; i < L; i++) {
        x += dx;
        y += dy;

        pDC->LineTo(static_cast<int>(x), static_cast<int>(y));
    }
}

// Bresenham算法
void CLineDlg::DrawLineBresenham(int x1, int y1, int x2, int y2) {

        // 计算dx和dy
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);

        // 计算步长方向，根据步长方向来决定递增还是递减
        int stepX = (x1 < x2) ? 1 : -1;
        int stepY = (y1 < y2) ? 1 : -1;

        // 初始化p0和起始点
        int p = 2 * dy - dx;
        int x = x1;
        int y = y1;

        // 获取IDC_STATIC_DRAW_AREA2
        CStatic* pStaticDrawArea2 = (CStatic*)GetDlgItem(IDC_STATIC_DRAW_AREA2);
        CDC* pDC = pStaticDrawArea2->GetDC();

        // 绘制起点
        pDC->SetPixel(x, y, RGB(0, 0, 0)); // 在指定位置绘制像素点

        // 循环绘制直线
        for (int k = 0; k < dx; k++)
        {
            // 根据pk决定下一个点的位置
            if (p < 0)
            {
                x += stepX;
                p += 2 * dy;
            }
            else
            {
                x += stepX;
                y += stepY;
                p += 2 * (dy - dx);
            }

            // 绘制像素点
            pDC->SetPixel(x, y, RGB(0, 0, 0));
        }

        // 释放设备
        pStaticDrawArea2->ReleaseDC(pDC);

}
