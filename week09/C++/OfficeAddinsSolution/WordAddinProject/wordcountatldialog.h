#pragma once

#include "resource.h"
#include <atlhost.h>

using namespace ATL;

// Cwordcountatldialog

class Cwordcountatldialog :
	public CAxDialogImpl<Cwordcountatldialog>
{
public:
	Cwordcountatldialog()
	{
	}

	~Cwordcountatldialog()
	{
	}

	enum { IDD = IDD_DIALOG1 };

BEGIN_MSG_MAP(Cwordcountatldialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<Cwordcountatldialog>)
END_MSG_MAP()

// 处理程序原型:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<Cwordcountatldialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		GetDlgItem(IDC_STATIC2);
		bHandled = TRUE;
		return 1;  // 让系统设置焦点
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};
