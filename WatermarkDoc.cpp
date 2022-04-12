
// WatermarkDoc.cpp: CWatermarkDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Watermark.h"
#endif

#include "WatermarkDoc.h"
#include "CDlgNumber.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWatermarkDoc

IMPLEMENT_DYNCREATE(CWatermarkDoc, CDocument)

BEGIN_MESSAGE_MAP(CWatermarkDoc, CDocument)
END_MESSAGE_MAP()


// CWatermarkDoc 생성/소멸

CWatermarkDoc::CWatermarkDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CWatermarkDoc::~CWatermarkDoc()
{
}

BOOL CWatermarkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CWatermarkDoc serialization

void CWatermarkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CWatermarkDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CWatermarkDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWatermarkDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWatermarkDoc 진단

#ifdef _DEBUG
void CWatermarkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWatermarkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWatermarkDoc 명령


void CWatermarkDoc::OnWatermarkBitplanewatermark(int wm)
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	unsigned char mask = 0x01;
	int position = 0;

	m_BitPlane_ptr = new unsigned char* [8];

	m_BitPlane_ptr[7] = SplitBitPlane(mask, position, wm);

	for (int i = 6; i >= 0; i--)
	{
		unsigned char* bitPlane = new unsigned char[m_Re_size];
		mask <<= 1;
		position += 1;
		
		//TODO: 0이 대체 안됨
		if (wm == 7-i) // 워터마크 이미지로 대체
		{
			OutputDebugString(L"wm");
			for (int p = 0; p < m_Re_size; p++)
			{
				bitPlane[p] = 255;
			}
			m_BitPlane_ptr[i] = bitPlane;
		}
		else
		{
			OutputDebugString(L"else");
			m_BitPlane_ptr[i] = SplitBitPlane(mask, position, wm);
		}
	}
}


// 비트플레인 분리
unsigned char* CWatermarkDoc::SplitBitPlane(unsigned char mask, int position, int wm)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char* bitPlane = new unsigned char[m_Re_size];

	for (int i = 0; i < m_Re_size; i++)
	{
		unsigned char pel = m_InputImage[i];
		if ((pel & mask) == pow(2, position)) {
			bitPlane[i] = 255;
		}
		else {
			bitPlane[i] = 0;
		}
	}

	return bitPlane;
}


BOOL CWatermarkDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CFile File; //파일 객체 선언

	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);

	if (File.GetLength() == 256 * 256) // 이미지 크기 = 256 * 256
	{
		this->m_height = 256; // m_height = 256; 와 똑같다
		this->m_width = 256;
	}
	else
	{
		AfxMessageBox(L"256 * 256 사이즈 이미지를 사용해주세요.",
			MB_OK | MB_ICONEXCLAMATION);
		return 0;
	} // 정의되지 않은 이미지 크기는 에러 처리

	m_size = m_width * m_height;
	m_InputImage = new unsigned char[m_size]; //메모리 할당
	for (int i = 0; i < m_size; i++) //메모리를 초기화
	{
		m_InputImage[i] = 255;
	}
	File.Read(m_InputImage, m_size); //이미지 파일을 읽어서 변수에 저장
	File.Close(); //파일 닫기

	return TRUE;
}
