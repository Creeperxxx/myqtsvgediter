#pragma once

#include "resource.h"
#include <atlhost.h>
#include <atlstr.h>

using namespace ATL;

// Cwordcountatldialog
class CWordAddin;

class Cwordcountatldialog :
	public CAxDialogImpl<Cwordcountatldialog>
{
public:
	Cwordcountatldialog()
		:m_isDelete(false)
	{
	}
	~Cwordcountatldialog()
	{
	}

	enum { IDD = IDD_DIALOG1 };

	BEGIN_MSG_MAP(Cwordcountatldialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)   // 添加这一行
		CHAIN_MSG_MAP(CAxDialogImpl<Cwordcountatldialog>)
	END_MSG_MAP()

	// 处理程序原型:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<Cwordcountatldialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		m_chineseCountWindow = GetDlgItem(IDC_STATIC2);
		m_englishCountWindow = GetDlgItem(IDC_STATIC4);
		m_chineseCountWindow.SetWindowTextW(L"0");
		m_englishCountWindow.SetWindowTextW(L"0");

		bHandled = TRUE;
		return 1;  // 让系统设置焦点
	}
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		DestroyWindow();
		return 0;
	}
	void OnFinalMessage(HWND hWnd) override
	{
		myDestroy();
		if (m_isDelete)
		{
			delete this;
		}
	}
	void showCount(uint64_t chineseCount, uint64_t englishCount);
	void myDestroy();
public:
	CWindow m_chineseCountWindow;
	CWindow m_englishCountWindow;
	//CWindow m_chineseIndex;
	//CWindow m_englishIndex;

	CWordAddin* m_pAddIn;
	bool m_isDelete;
};
