
// WatermarkView.cpp: CWatermarkView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Watermark.h"
#endif

#include "WatermarkDoc.h"
#include "WatermarkView.h"
#include "CDlgNumber.h"
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWatermarkView

IMPLEMENT_DYNCREATE(CWatermarkView, CView)

BEGIN_MESSAGE_MAP(CWatermarkView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWatermarkView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_WATERMARK_BITPLANEWATERMARK, &CWatermarkView::OnWatermarkBitplanewatermark)
END_MESSAGE_MAP()

// CWatermarkView 생성/소멸

CWatermarkView::CWatermarkView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CWatermarkView::~CWatermarkView()
{
}

BOOL CWatermarkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

void CWatermarkView::OnDraw(CDC* pDC)
{
	CWatermarkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 이미지를 빠르게 출력
	CRect viewRect;
	GetClientRect(viewRect);

	int height = viewRect.Height();
	int width = viewRect.Width();
	CDC memDC;
	CBitmap bit;
	memDC.CreateCompatibleDC(pDC);
	bit.CreateCompatibleBitmap(pDC, width, height);
	memDC.SelectObject(&bit);
	memDC.Rectangle(0, 0, width, height);


	// 입력 이미지를 화면에 출력
	int i, j;
	unsigned char R, G, B;
	unsigned char* m_temp_bitplane;

	// 원본 이미지 출력
	for (int i = 0; i < pDoc->m_height; i++)
	{
		for (int j = 0; j < pDoc->m_width; j++)
		{
			R = G = B = pDoc->m_InputImage[i * (pDoc->m_width) + j]; //Input: Grayscale. 따라서 rgb를 똑같이 함
			memDC.SetPixel(j + 5, i + 5, RGB(R, G, B));
		}
	}

	// 비트플레인 이미지를 화면에 출력
	if (is_bitplaneall)
	{
		// bp_num: 8개의 비트플레인
		for (int bp_num = 0; bp_num < 8; bp_num++)
		{
			for (int i = 0; i < pDoc->m_Re_height; i++)
			{
				for (int j = 0; j < pDoc->m_Re_width; j++)
				{
					R = G = B = pDoc->m_BitPlane_ptr[bp_num][i * (pDoc->m_Re_width) + j];
					memDC.SetPixel(
						j + pDoc->m_width * (bp_num % 4 + 1) + 10, //한 줄에 이미지 네개씩 출력
						i + pDoc->m_Re_height * (bp_num / 4) + 5,
						RGB(R, G, B));
				}
			}
		}
	}
	// 결과 출력
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);

	// 메모리 해제
	memDC.DeleteDC();
	bit.DeleteObject();
}

// CWatermarkView 인쇄


void CWatermarkView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWatermarkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWatermarkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWatermarkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CWatermarkView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWatermarkView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWatermarkView 진단

#ifdef _DEBUG
void CWatermarkView::AssertValid() const
{
	CView::AssertValid();
}

void CWatermarkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWatermarkDoc* CWatermarkView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWatermarkDoc)));
	return (CWatermarkDoc*)m_pDocument;
}
#endif //_DEBUG


// CWatermarkView 메시지 처리기


void CWatermarkView::OnWatermarkBitplanewatermark()
{
	is_bitplaneall = true;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWatermarkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// wm: 몇번째 비트맵에 워터마크 이미지 넣을 지 입력
	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK) {
		int wm = (int)dlg.m_InputNumber;

		// 잘못된 입력값에 대한 메시지 처리(올바른 입력 들어올 때까지 반복)
		while ((wm > 7) || (wm < 0)) {
			AfxMessageBox("0~7 범위의 정수를 입력해주세요",
				MB_OK | MB_RETRYCANCEL );
			if (dlg.DoModal() == IDOK) {
				wm = (int)dlg.m_InputNumber;
			}
		}
		pDoc->OnWatermarkBitplanewatermark(wm);
		Invalidate(TRUE);
	}
}
