#include "pch.h"
#include "wordcountatldialog.h"
#include "WordAddin.h"


// Cwordcountatldialog


void Cwordcountatldialog::showCount(LONG chineseCount, LONG englishCount)
{
	CAtlString chineseCountStr;
	CAtlString englishCountStr;

	chineseCountStr.Format(_T("%d"), chineseCount);
	englishCountStr.Format(_T("%d"), englishCount);

	m_chineseCountWindow.SetWindowTextW(chineseCountStr);
	m_englishCountWindow.SetWindowTextW(englishCountStr);
}

void Cwordcountatldialog::myDestroy()
{
	m_chineseCountWindow.Detach();
	m_englishCountWindow.Detach();
}

