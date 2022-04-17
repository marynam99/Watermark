
// WatermarkDoc.h: CWatermarkDoc 클래스의 인터페이스
//


#pragma once


class CWatermarkDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
    CWatermarkDoc() noexcept;
    DECLARE_DYNCREATE(CWatermarkDoc)

// 특성입니다.
public:
    int m_width; // m으로 시작하면 클래스의 member라는 뜻
    int m_height;
    int m_size;

    int m_Re_width;
    int m_Re_height;
    int m_Re_size;

    unsigned char* m_InputImage;
    unsigned char* m_OutputImage;

    // 비트 플레인 배열
    unsigned char** m_BitPlane_ptr;

// 작업입니다.
public:

// 재정의입니다.
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
    virtual ~CWatermarkDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    // 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
    void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
    void OnWatermarkBitplanewatermark(int wm);
    // 비트플레인 분리
    unsigned char* SplitBitPlane(unsigned char mask, int position, int wm);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
