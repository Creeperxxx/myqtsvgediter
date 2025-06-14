﻿#pragma once

#include "resource.h"
#include <atlhost.h>
#include <atlstr.h>

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

		m_chineseCountWindow = GetDlgItem(IDC_STATIC2);
		m_englishCountWindow = GetDlgItem(IDC_STATIC4);
		m_chineseIndex = GetDlgItem(IDC_STATIC1);
		m_chineseIndex.SetWindowTextW(L"Chinese: ");
		m_englishIndex = GetDlgItem(IDC_STATIC3);
		m_englishIndex.SetWindowTextW(L"English：");
		m_chineseCountWindow.SetWindowTextW(L"0");
		m_englishCountWindow.SetWindowTextW(L"0");

		CenterWindow();
		bHandled = TRUE;
		return 1;  // 让系统设置焦点
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		//EndDialog(wID); //这是关闭模态对话框的
		DestroyWindow();
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		DestroyWindow();
		return 0;
	}
	void OnFinalMessage(HWND /*hWnd*/) override
	{
		delete this;
	}
	
	void OnMyDestory();

	void showCount(LONG chineseCount, LONG englishCount);
public:
	CWindow m_chineseCountWindow;
	CWindow m_englishCountWindow;
	CWindow m_chineseIndex;
	CWindow m_englishIndex;
};
