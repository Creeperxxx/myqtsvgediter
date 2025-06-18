#pragma once

#include "resource.h"
#include <atlhost.h>

using namespace ATL;

// Cnonemptycellscountdialog

class Cnonemptycellscountdialog :
	public CAxDialogImpl<Cnonemptycellscountdialog>
{
public:
	Cnonemptycellscountdialog()
		:m_isdelete(false)
	{
	}

	~Cnonemptycellscountdialog()
	{
	}

	enum { IDD = IDD_DIALOG1 };

BEGIN_MSG_MAP(Cnonemptycellscountdialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	//COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	//COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<Cnonemptycellscountdialog>)
END_MSG_MAP()

// 处理程序原型:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<Cnonemptycellscountdialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		m_countIndex = GetDlgItem(IDC_STATIC1);
		m_count = GetDlgItem(IDC_STATIC2);

		m_countIndex.SetWindowTextW(L"非空单元格数：");
		m_count.SetWindowTextW(L"0");

		bHandled = TRUE;
		return 1;  // 让系统设置焦点
	}

	CWindow m_countIndex;
	CWindow m_count;

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = false;
		DestroyWindow();
		return 0;
	}

	void OnFinalMessage(HWND hwnd) override
	{
		myDestroy();
		if (m_isdelete)
		{
			delete this;
		}
	}

	void myDestroy()
	{
		m_countIndex.Detach();
		m_count.Detach();
	}

	bool m_isdelete;

	void ShowCount(LONG count);


	//LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	//{
	//	//EndDialog(wID);
	//	return 0;
	//}

	//LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	//{
	//	//EndDialog(wID);
	//	return 0;
	//}
};
