#include "handswriting.h"

handsInput::handsInput(QWidget* parent) : QWidget(parent) {

}


handsInput::~handsInput() {
    if (this->g_pIInkRecoContext != NULL)
    {
        this->g_pIInkRecoContext->Release();
        this->g_pIInkRecoContext = NULL;
    }

    if (this->g_pIInkDisp != NULL)
    {
        this->g_pIInkDisp->Release();
        this->g_pIInkDisp = NULL;
    }

    if (this->g_pIInkCollector != NULL)
    {
        this->g_pIInkCollector->Release();
        this->g_pIInkCollector = NULL;
    }
}


void handsInput::inputInit(HWND hwnd) {
	if (hwnd == nullptr)
		return;

	this->g_pIInkCollector = nullptr;
	this->g_pIInkDisp = nullptr;
	this->g_pIInkRecoContext = nullptr;

    CoInitialize(nullptr);

    HRESULT hr;

    // 使用默认识别器创建一个识别上下文
    // 该上下文会在后续所有识别对象中使用
    hr = CoCreateInstance(CLSID_InkRecognizerContext,
        NULL, CLSCTX_INPROC_SERVER,
        IID_IInkRecognizerContext,
        (void**)&g_pIInkRecoContext);

    if (FAILED(hr)) {
        qDebug() << "没有安装手写recoginizer";
        return;
    }

    
    // 创建墨迹收集对象
    hr = CoCreateInstance(CLSID_InkCollector,
        NULL, CLSCTX_INPROC_SERVER,
        IID_IInkCollector,
        (void**)&g_pIInkCollector);

    if (FAILED(hr)) {
        qDebug() << "创建墨迹收集对象失败";
        return;
    }


    // 获取墨迹对象的指针
    hr = g_pIInkCollector->get_Ink(&g_pIInkDisp);
    if (FAILED(hr)) {
        qDebug() << "获取墨迹对象指针失败";
        return;
    }


    // 告诉收集器要收集哪个窗口的墨迹
    hr = g_pIInkCollector->put_hWnd((long)hwnd);
    if (FAILED(hr)) {
        return;
    }


    // 设置颜色
    IInkDrawingAttributes* p;
    if (SUCCEEDED(g_pIInkCollector->get_DefaultDrawingAttributes(&p))) {
        p->put_Color(RGB(255, 0, 0));
    }

    // 开启墨迹输入
    hr = g_pIInkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr)) {
        qDebug() << "墨迹输入开启失败";
        return;
    }

    //设置手写只识别为一个字
    IInkRecognizerGuide* RecognizerGuide;
    hr = CoCreateInstance(CLSID_InkRecognizerGuide, NULL, CLSCTX_INPROC_SERVER, IID_IInkRecognizerGuide, (void**)&RecognizerGuide);
    if (FAILED(hr))
        return;
    InkRecoGuide recoguide;
    RECT rect;

    rect.bottom = 2;//不能为1
    rect.left = 0;
    rect.right = 2;//不能为1
    rect.top = 0;

    recoguide.rectWritingBox = rect;
    recoguide.rectDrawnBox = rect;
    recoguide.cRows = 1;//不要过大
    recoguide.cColumns = 1;
    recoguide.midline = -1;
    RecognizerGuide->put_GuideData(recoguide);
    g_pIInkRecoContext->putref_Guide(RecognizerGuide);

    VARIANT_BOOL autoRedraw = VARIANT_TRUE;
    hr = g_pIInkCollector->put_AutoRedraw(autoRedraw);
    if (FAILED(hr)) {
        qDebug() << "设置自动重绘墨迹失败";
        g_pIInkCollector->Release();
        CoUninitialize();
        return;
    }
}


void handsInput::recognize(std::vector<std::string>& inputResult, bool isRecord) {
    // 鼠标变成沙漏
    HCURSOR hCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
    // Get a pointer to the ink stroke collection
    // This collection is a snapshot of the entire ink object

    if (!inputResult.empty()) {
        inputResult.clear();
    }

    // 获取指向墨迹收集器的指针
    IInkStrokes* pIInkStrokes = NULL;
    HRESULT hr = g_pIInkDisp->get_Strokes(&pIInkStrokes);
    if (SUCCEEDED(hr))
    {
        if (isRecord) {
            // 存储单个笔画
            long strokesSize = 0;
            IInkStrokeDisp* newStroke = nullptr;
            pIInkStrokes->get_Count(&strokesSize);
            pIInkStrokes->Item(strokesSize - 1, &newStroke);
            this->strokeRecord.push_back(newStroke);
            //newStroke->AddRef();
        }

        // 将笔触收集器传递给识别器
        hr = g_pIInkRecoContext->putref_Strokes(pIInkStrokes);
        if (SUCCEEDED(hr))
        {
            // 识别
            IInkRecognitionResult* pIInkRecoResult = NULL;
            InkRecognitionStatus RecognitionStatus;
            hr = g_pIInkRecoContext->Recognize(&RecognitionStatus, &pIInkRecoResult);
            if (SUCCEEDED(hr) && (pIInkRecoResult != NULL))
            {
                // 枚举所有可能结果
                IInkRecognitionAlternates* spIInkRecoAlternates;
                hr = pIInkRecoResult->AlternatesFromSelection(
                    0,
                    -1,
                    10,  // 候选单词数量
                    &spIInkRecoAlternates);
                long lCount = 0;

                if (SUCCEEDED(hr) && SUCCEEDED(spIInkRecoAlternates->get_Count(&lCount))) {
                    // 获取所有识别结果
                    IInkRecognitionAlternate* pIInkRecoAlternate = nullptr;
                    for (long i = 0; (i < lCount) && (i < 10); i++) {
                        if (SUCCEEDED(spIInkRecoAlternates->Item(i, &pIInkRecoAlternate))) {
							BSTR singleResult = NULL;
							if (SUCCEEDED(pIInkRecoAlternate->get_String(&singleResult))) {
								
								QString str = QString::fromWCharArray(singleResult);
								std::string tmp = str.toUtf8().constData();
								inputResult.push_back(tmp);
							}
							pIInkRecoAlternate->Release();
                        }
                    }
                }
            }
            // 重置识别器内容
            g_pIInkRecoContext->putref_Strokes(nullptr);
        }
        pIInkStrokes->Release();
    }
    // restore the cursor
    ::SetCursor(hCursor);
}


int handsInput::backStrokes() {
    if (this->strokeRecord.empty()) 
        return BACK_STROKE_FAIL;
    else
        qDebug() << "屏幕为空，无需撤销";
        
    IInkStrokeDisp* lastStroke = this->strokeRecord.back();
    this->strokeRecord.pop_back();
    if (lastStroke) {
        HRESULT hr = g_pIInkDisp->DeleteStroke(lastStroke);
        if (FAILED(hr)) {
            qDebug() << "撤销失败";
            return BACK_STROKE_FAIL;
        }
        lastStroke->Release();

        // 如果删的是最后一个笔画就返回BACK_STROKE_LAST_STROKE，后续无需再recognize
        if (this->strokeRecord.empty())
            return BACK_STROKE_LAST_STROKE;
    }

    return BACK_STROKE_SUCCESS;
}


void handsInput::clearStrokes() {
    // 清除笔触
	HRESULT hr = this->g_pIInkDisp->DeleteStrokes(0);
	if (FAILED(hr)) {
		qDebug() << "清除笔触失败";
		return;
	}
}
