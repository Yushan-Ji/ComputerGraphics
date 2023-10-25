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

// ���DDA��ť��ִ�еĲ���
void CLineDlg::OnBnClickedButtonDDA() {
    // �ӱ༭���л�ȡ����
    CString strX0, strY0, strX1, strY1;
    // �������
    GetDlgItemText(IDC_EDIT_START1, strX0);
    GetDlgItemText(IDC_EDIT_START2, strY0);
    // �յ�����
    GetDlgItemText(IDC_EDIT_END1, strX1);
    GetDlgItemText(IDC_EDIT_END2, strY1);

    int startX = _ttoi(strX0);
    int startY = _ttoi(strY0);
    int endX = _ttoi(strX1);
    int endY = _ttoi(strY1);

    // ��ȡ��ͼ��ı߽����
    CRect rectDrawArea1;
    GetDlgItem(IDC_STATIC_DRAW_AREA1)->GetClientRect(rectDrawArea1);

    // ӳ�����굽��ͼ�������ϵ
    startX -= rectDrawArea1.left;
    startY -= rectDrawArea1.top;
    endX -= rectDrawArea1.left;
    endY -= rectDrawArea1.top;

    // �ڻ���ֱ��֮ǰ��������Ƿ��ڻ�ͼ����
    if (startX >= 0 && startX < rectDrawArea1.Width() &&
        startY >= 0 && startY < rectDrawArea1.Height() &&
        endX >= 0 && endX < rectDrawArea1.Width() &&
        endY >= 0 && endY < rectDrawArea1.Height()) {
        // �����ڻ�ͼ���ڣ����Ի���ֱ��
        // ����DDA����
        DrawLineDDA(startX, startY, endX, endY);
    } else {
        // ���곬���˻�ͼ��ı߽磬������ʾ
        CString message = _T("���곬����ͼ��ı߽磬����������");
        CString caption = _T("����");
        MessageBox(message, caption, MB_ICONERROR);
    }
}

// ���Bresenham��ť��ִ�еĲ���
void CLineDlg::OnBnClickedButtonBresenham() {
    // �ӱ༭���л�ȡ����
    CString strX0, strY0, strX1, strY1;
    // �������
    GetDlgItemText(IDC_EDIT_START1, strX0);
    GetDlgItemText(IDC_EDIT_START2, strY0);
    // �յ�����
    GetDlgItemText(IDC_EDIT_END1, strX1);
    GetDlgItemText(IDC_EDIT_END2, strY1);

    int startX = _ttoi(strX0);
    int startY = _ttoi(strY0);
    int endX = _ttoi(strX1);
    int endY = _ttoi(strY1);

    // ��ȡ��ͼ��ı߽����
    CRect rectDrawArea2;
    GetDlgItem(IDC_STATIC_DRAW_AREA2)->GetClientRect(rectDrawArea2);

    // ӳ�����굽��ͼ�������ϵ
    startX -= rectDrawArea2.left;
    startY -= rectDrawArea2.top;
    endX -= rectDrawArea2.left;
    endY -= rectDrawArea2.top;

    // �ڻ���ֱ��֮ǰ��������Ƿ��ڻ�ͼ����
    if (startX >= 0 && startX < rectDrawArea2.Width() &&
        startY >= 0 && startY < rectDrawArea2.Height() &&
        endX >= 0 && endX < rectDrawArea2.Width() &&
        endY >= 0 && endY < rectDrawArea2.Height()) {
        // �����ڻ�ͼ���ڣ����Ի���ֱ��
        // ����Bresenham����
        DrawLineBresenham(startX, startY, endX, endY);
    } else {
        // ���곬���˻�ͼ��ı߽磬������ʾ
        CString message = _T("���곬����ͼ��ı߽磬����������");
        CString caption = _T("����");
        MessageBox(message, caption, MB_ICONERROR);
    }
}

// DDA�㷨
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

// Bresenham�㷨
void CLineDlg::DrawLineBresenham(int x1, int y1, int x2, int y2) {

        // ����dx��dy
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);

        // ���㲽�����򣬸��ݲ��������������������ǵݼ�
        int stepX = (x1 < x2) ? 1 : -1;
        int stepY = (y1 < y2) ? 1 : -1;

        // ��ʼ��p0����ʼ��
        int p = 2 * dy - dx;
        int x = x1;
        int y = y1;

        // ��ȡIDC_STATIC_DRAW_AREA2
        CStatic* pStaticDrawArea2 = (CStatic*)GetDlgItem(IDC_STATIC_DRAW_AREA2);
        CDC* pDC = pStaticDrawArea2->GetDC();

        // �������
        pDC->SetPixel(x, y, RGB(0, 0, 0)); // ��ָ��λ�û������ص�

        // ѭ������ֱ��
        for (int k = 0; k < dx; k++)
        {
            // ����pk������һ�����λ��
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

            // �������ص�
            pDC->SetPixel(x, y, RGB(0, 0, 0));
        }

        // �ͷ��豸
        pStaticDrawArea2->ReleaseDC(pDC);

}
