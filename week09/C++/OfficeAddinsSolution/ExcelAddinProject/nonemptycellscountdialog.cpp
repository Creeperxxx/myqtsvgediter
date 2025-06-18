#include "pch.h"
#include "nonemptycellscountdialog.h"


// Cnonemptycellscountdialog

void Cnonemptycellscountdialog::ShowCount(LONG count)
{
	static CAtlString text;
	text.Format(_T("%d"), count);
	m_count.SetWindowTextW(text);
}

