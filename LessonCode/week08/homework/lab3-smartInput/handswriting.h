#pragma once

#ifndef __HANDSWRITING_H__
#define __HANDSWRITING_H__

#include "common.h"

#define BACK_STROKE_FAIL 0
#define BACK_STROKE_SUCCESS 1
#define BACK_STROKE_LAST_STROKE 2


class handsInput : public QWidget {
	Q_OBJECT

public:

	explicit handsInput(QWidget* parent = 0);
	~handsInput();

	IInkCollector* g_pIInkCollector;
	IInkDisp* g_pIInkDisp;
	IInkRecognizerContext* g_pIInkRecoContext;

	std::vector<IInkStrokeDisp*> strokeRecord;  // 存储单根笔画

	void inputInit(HWND hwnd);
	void recognize(std::vector<std::string>& inputResult, bool isRecord);
	void clearStrokes();
	int backStrokes();

private:

};



#endif
