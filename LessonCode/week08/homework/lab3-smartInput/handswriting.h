#ifndef __HANDSWRITING_H__
#define __HANDSWRITING_H__

#include <Qwidget>

#include <msinkaut_i.c>
#include <msinkaut.h>
#include <vector>
#include <string>


class handsInput : public QWidget {
	Q_OBJECT

public:

	explicit handsInput(QWidget *parent = 0);
	~handsInput();

	IInkCollector* g_pIInkCollector;
	IInkDisp* g_pIInkDisp;
	IInkRecognizerContext* g_pIInkRecoContext;


	void inputInit(HWND hwnd);
	void recognize(std::vector<std::string> &inputResult);
	void clearStrokes();


private:

};



#endif
