// This file was created on March 21st 2001 by Robert Brault.
// I created this Class to be able change the Color of your Edit Box
// as well as your Edit Box Text. This is Derived from CListBox so you
// do not have all the overhead of a CRichEditCtrl.
//
// There are three functions available Currently:
// SetBkColor(COLORREF crColor)
// SetTextColor(COLORREF crColor)
// SetReadOnly(BOOL flag = TRUE)
//
// How To Use:
// Add three files to your project
// ColorEdit.cpp, ColorEdit.h and Color.h
// Color.h has (#define)'s for different colors (add any color you desire).
//
// Add #include "ColorEdit.h" to your Dialogs Header file.
// Declare an instance of CColorListBox for each edit box being modified.
// Ex. CColorListBox m_ebName;
//
// In your OnInitDialog() add a SubclassDlgItem for each CColorListBox member variable.
// Ex. m_ebName.SubclassDlgItem(IDC_EB_NAME, this);
// In this same function initialize your color for each box unless you want the default.


// ColorEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ColorListBox.h"
#include "Color.h" // File Holding (#define)'s for COLORREF Values

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorListBox

CColorListBox::CColorListBox()
{
	m_crBkColor = ::GetSysColor(COLOR_3DFACE); // Initializing background color to the system face color.
	m_crTextColor = BLACK; // Initializing text color to black
	m_brBkgnd.CreateSolidBrush(m_crBkColor); // Creating the Brush Color For the Edit Box Background
}

CColorListBox::~CColorListBox()
{
}


BEGIN_MESSAGE_MAP(CColorListBox, CListBox)
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()
int num;
void CColorListBox::OnOK()
{
	
	switch (num)
	{
	case 6:
		num = 0;
	break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 0: CColorListBox::AddString("test", RGB(255, 0, 0)); break;
	}
	num++;
}
HBRUSH CColorListBox::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == 2222)
	{
		pDC->SetTextColor(RGB(160, 32, 240));
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}
void CColorListBox::SetTextColor(COLORREF crColor)
{
	m_crTextColor = crColor; // Passing the value passed by the dialog to the member varaible for Text Color
	RedrawWindow();
}

void CColorListBox::SetBkColor(COLORREF crColor)
{
	m_crBkColor = crColor; // Passing the value passed by the dialog to the member varaible for Backgound Color
	m_brBkgnd.DeleteObject(); // Deleting any Previous Brush Colors if any existed.
	m_brBkgnd.CreateSolidBrush(crColor); // Creating the Brush Color For the Edit Box Background
	RedrawWindow();
}



HBRUSH CColorListBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hbr;
	hbr = (HBRUSH)m_brBkgnd; // Passing a Handle to the Brush
	pDC->SetBkColor(m_crBkColor); // Setting the Color of the Text Background to the one passed by the Dialog
	pDC->SetTextColor(m_crTextColor); // Setting the Text Color to the one Passed by the Dialog
	pDC->SetBkMode(TRANSPARENT);
	if (nCtlColor)       // To get rid of compiler warning
      nCtlColor += 0;

	return hbr;
}

BOOL CColorListBox::SetReadOnly(BOOL flag)
{
   if (flag == TRUE)
      SetBkColor(m_crBkColor);
   else
      SetBkColor(WHITE);

   return flag;// CListBox::SetReadOnly(flag);
}

////////////////////////////////////////////////////////

/********************************************************************/
/*																	*/
/* Function name : AddString										*/
/* Description   : Add string to the listbox and save color info.	*/
/*																	*/
/********************************************************************/
int CColorListBox::AddString(LPCTSTR lpszItem, COLORREF itemColor)
{
	// Add the string to the list box
	int nIndex = CListBox::AddString(lpszItem);

	// save color data
	if (nIndex >= 0)
		SetItemData(nIndex, itemColor);
	Invalidate();
	return nIndex;
}

BOOL CColorListBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}
/********************************************************************/
/*																	*/
/* Function name : DrawItem											*/
/* Description   : Called by the framework when a visual aspect of	*/
/*				   an owner-draw list box changes.					*/
/*																	*/
/********************************************************************/
void CColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	/*CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	RECT rect = lpDrawItemStruct->rcItem;
	UINT nId = lpDrawItemStruct->itemID;
	CString strItemText;
	GetText(lpDrawItemStruct->itemID, strItemText);
	//if (nId == 1 || nId == 3)
	if (nId!=-1)
	{
		//dc.FillSolidRect(&rect, RGB(255, 0, 0));
		//dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(255, 0, 0));
		//dc.SetBkColor(RGB(255, 34, 34));
		dc.DrawText(strItemText, &rect, DT_LEFT | DT_VCENTER);
	}
	else
	{
		//CCheckListBox dlg;
		//dlg.DrawItem(lpDrawItemStruct);
		CListBox::DrawItem(lpDrawItemStruct);
	}
	dc.Detach();*/
	// Losing focus ?
	if (lpDrawItemStruct->itemID == -1)
	{
		DrawFocusRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem);
		return;
	}

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	COLORREF clrOld;
	CString sText;

	// get color info from item data
	COLORREF clrNew = (COLORREF)(lpDrawItemStruct->itemData);

	// item selected ?
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brush(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	}

	// item deselected ?
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		CBrush brush(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	}

	// item has focus ?
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	}

	// lost focus ?
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
		!(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	}

	// set the background mode to TRANSPARENT
	int nBkMode = pDC->SetBkMode(TRANSPARENT);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		clrOld = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
		clrOld = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	else
		clrOld = pDC->SetTextColor(clrNew);

	// get item text
	GetText(lpDrawItemStruct->itemID, sText);
	CRect rect = lpDrawItemStruct->rcItem;

	// text format
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;

	// draw the text
	pDC->DrawText(sText, -1, &rect, nFormat);

	// restore old values
	pDC->SetTextColor(clrOld);
	//pDC->SetBkMode(nBkMode);
	pDC->SetBkMode(TRANSPARENT);
}


void CColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENUCHECK);
}