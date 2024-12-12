#include "handswriting.h"
#include <QDebug>

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

    // ʹ��Ĭ��ʶ��������һ��ʶ��������
    // �������Ļ��ں�������ʶ�������ʹ��
    hr = CoCreateInstance(CLSID_InkRecognizerContext,
        NULL, CLSCTX_INPROC_SERVER,
        IID_IInkRecognizerContext,
        (void**)&g_pIInkRecoContext);

    if (FAILED(hr)) {
        qDebug() << "û�а�װ��дrecoginizer";
        return;
    }

    
    // ����ī���ռ�����
    hr = CoCreateInstance(CLSID_InkCollector,
        NULL, CLSCTX_INPROC_SERVER,
        IID_IInkCollector,
        (void**)&g_pIInkCollector);

    if (FAILED(hr)) {
        qDebug() << "����ī���ռ�����ʧ��";
        return;
    }


    // ��ȡī�������ָ��
    hr = g_pIInkCollector->get_Ink(&g_pIInkDisp);
    if (FAILED(hr)) {
        qDebug() << "��ȡī������ָ��ʧ��";
        return;
    }


    // �����ռ���Ҫ�ռ��ĸ����ڵ�ī��
    hr = g_pIInkCollector->put_hWnd((long)hwnd);
    if (FAILED(hr)) {
        return;
    }


    // ������ɫ
    IInkDrawingAttributes* p;
    if (SUCCEEDED(g_pIInkCollector->get_DefaultDrawingAttributes(&p))) {
        p->put_Color(RGB(255, 0, 0));
    }


    // ����ī������
    hr = g_pIInkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr)) {
        qDebug() << "ī�����뿪��ʧ��";
        return;
    }



}


void handsInput::recognize(std::vector<std::string>& inputResult) {
    // �����ɳ©
    HCURSOR hCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
    // Get a pointer to the ink stroke collection
    // This collection is a snapshot of the entire ink object

    if (!inputResult.empty()) {
        inputResult.clear();
    }

    // ��ȡָ��ī���ռ�����ָ��
    IInkStrokes* pIInkStrokes = NULL;
    HRESULT hr = g_pIInkDisp->get_Strokes(&pIInkStrokes);
    if (SUCCEEDED(hr))
    {
        // ���ʴ��ռ������ݸ�ʶ����
        hr = g_pIInkRecoContext->putref_Strokes(pIInkStrokes);
        if (SUCCEEDED(hr))
        {
            // ʶ��
            IInkRecognitionResult* pIInkRecoResult = NULL;
            InkRecognitionStatus RecognitionStatus;
            hr = g_pIInkRecoContext->Recognize(&RecognitionStatus, &pIInkRecoResult);
            if (SUCCEEDED(hr) && (pIInkRecoResult != NULL))
            {
                // ö�����п��ܽ��
                IInkRecognitionAlternates* spIInkRecoAlternates;
                hr = pIInkRecoResult->AlternatesFromSelection(
                    0,
                    -1,
                    10,  // ��ѡ��������
                    &spIInkRecoAlternates);
                long lCount = 0;

                if (SUCCEEDED(hr) && SUCCEEDED(spIInkRecoAlternates->get_Count(&lCount))) {
                    // ��ȡ����ʶ����
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
            // ����ʶ��������
            g_pIInkRecoContext->putref_Strokes(nullptr);
        }
        pIInkStrokes->Release();
    }
    // restore the cursor
    ::SetCursor(hCursor);
}


void handsInput::clearStrokes() {
    // ����ʴ�
	HRESULT hr = this->g_pIInkDisp->DeleteStrokes(0);
	if (FAILED(hr)) {
		qDebug() << "����ʴ�ʧ��";
		return;
	}

}
