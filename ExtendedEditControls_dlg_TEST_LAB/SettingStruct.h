#pragma once
struct sGAMEAI
{
	struct PAGE1
	{
		bool		fontUnderline[TOTAL_STYLE];
		bool		fontBold[TOTAL_STYLE];
		bool		fontItalic[TOTAL_STYLE];

		//bool		Underline;
		bool		m_nOpenFileNewWindow;
		bool		GlobalBack;
		bool		DarkMode;
		bool		m_nLineNumber;
		bool		m_colorSyntax;
		bool		m_enableEOLflag;
		char		fontName[MAX_NAME_LENGTH + 1];
		int			fontSize;
		bool		m_edgeNoBorder;
		int			borderWidth;
		COLORREF	fontTextColor[TOTAL_STYLE];
		COLORREF	fontBackColor[TOTAL_STYLE];

		bool		m_styleAlpha;
		int			m_styleAlphaPercent;

		char		m_storedGUID[MAX_NAME_LENGTH + 1];

		CFont		m_font;
	};


	PAGE1	sPage1;
};