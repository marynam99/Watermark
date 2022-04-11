// CDlgNumber.cpp: 구현 파일
//

#include "pch.h"
#include "Watermark.h"
#include "afxdialogex.h"
#include "CDlgNumber.h"


// CDlgNumber 대화 상자

IMPLEMENT_DYNAMIC(CDlgNumber, CDialogEx)

CDlgNumber::CDlgNumber(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CDlgNumber::~CDlgNumber()
{
}

void CDlgNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT1, m_InputNumber);
	DDX_Text(pDX, IDC_EDIT2, m_InputNumber);
}


BEGIN_MESSAGE_MAP(CDlgNumber, CDialogEx)
END_MESSAGE_MAP()


// CDlgNumber 메시지 처리기
