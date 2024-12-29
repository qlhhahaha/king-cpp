## lab3：智能手写输入法

#### 一、 项目设计方案

1. **单字识别**

   在handswriting类中基于windows tablet pc sdk设置public成员变量g_pIInkCollector、g_pIInkDisp、g_pIInkRecoContext，类构造函数中对其初始化，开启墨迹输入；

   mouseReleaseEvent()事件中进行墨迹识别，将在指定书写区域内采集到的笔触收集器内容传递给识别器g_pIInkRecoContext，利用其Recognize()方法得到数个最佳匹配结果，输出前九个



2. **笔画撤销**

   在handswriting类中设置私有成员变量std::vector<IInkStrokeDisp*> strokeRecord，用于**存储单根笔画**，每次调用recognize()进行墨迹识别时，就把当前写的这根笔画push_back进去。点击撤销按钮时进行判断，如果当前为空（屏幕中没有墨迹）就什么都不做；只剩一根墨迹时就直接调用屏幕重写clearStrokes()函数进行清空（包括候选词区域）；墨迹大于一根则重新识别以及匹配中文联想词



3. **小键盘**

   分别采用三个QDialog + QLayout来绘制英文、数字、标点符号键盘，使用**QSignalMapper**对小键盘上各个pushbutton的信号槽进行统一映射，即点击键盘时都是输出对应pushbutton上的文字

   

4. **设置功能**

   同样采用QDialog绘制设置界面，用QComboBox绘制笔迹颜色和候选词大小选择、QSlider绘制笔迹粗细滑动条、用IInkDrawingAttributes和IInkCollector类实现颜色更改、用QGridLayout的itemAt方法来统一更改候选词大小



5. **区分单字手写和长句手写**

   长句书写本身不用额外设计，tablet pc的g_pIInkRecoContext默认开启长句书写；反倒是单字手写需要额外设置，方案是RecognizerGuide->put_GuideData(recoguide);和g_pIInkRecoContext->putref_Guide(RecognizerGuide);其中要把recoguide的cRows和cColumns设为1、midline设为-1、rectWritingBox的bottom和right设为2，从而**将手写识别范围限制在第一个字的外包矩形中**；将上述参数设为0时则又恢复默认的长句书写

   此外使用QSS设置单字手写和长句手写的高亮，从而直观区分两种状态



6. **中文联想**

   思路来源：[How to Write a Spelling Corrector](https://norvig.com/spell-correct.html)

   采用基于**贝叶斯概率的自适应方法**实现中文词组联想，所谓贝叶斯概率简而言之就是在已知事件B发生时计算后验事件A的概率
   $$
   P(A|B)=\frac{P(B|A)P(A)}{P(B)}
   $$
   对应到输入法中，假设P(B)代表用户输入”上“的概率，P(A|B)代表已经输入”上“后、继续输入”上班“的概率，由公式可知，P(B)未知、无法改进；P(B|A)表示输入”上班“的情况下输入"上"的概率，这是必然的，概率值为1；那么可供进行联想计算的只有P(A)，即用户想输入”上班“二字的概率（以链接文章为例，其中提到所有英文文本里有7%的内容为单词”the“，所以认为P(the)=0.07）。

   经过简单分析后，我们可以从贝叶斯概率中推出一种非常符合直觉的方案：设计一张不含单字、仅含词语（二三四字皆可）的表，该表有多行，每一行包含一个词语和一个数字（表示该词语出现的频次），代码中用unordered_map来存储这张表，其中key为词语，value为一个包含了频次和词语首个字的struct。在输入法识别出字体后，将该字体作为输入在unordered_map中查询、取出、排序，取频次最高（代表P(A)最高）的数个查询结果作为联想的输出；

   另外自适应的含义是，在数个联想结果中，无论用户选择哪一个，都将该词语的频次+1，从而能使得符合用户习惯的词语在下一次联想时更有可能被优先展示



#### 二、亮点

1. 支持笔画撤销和重写，撤销过程中也能识别和联想
2. 支持非中文（英文、数字、符号）的小键盘以及手写输入
3. 支持在设置中更换笔迹颜色、粗细、字号、恢复默认设置
4. 支持单字手写和长句手写
5. 采用基于贝叶斯概率的自适应方法实现中文词组联想，联想结果的展示优先级会根据用户点击频次进行自适应调整



#### 三、经验收获

1. **中文编码很重要**：如无特殊要求，尽量所有代码文件和外部文本文件一律使用UTF8/UTF8 with bom，经实测和std::string、std::wstring、QString等的兼容性最好，乱码概率低，不要混用UTF8和GBK

2. **头文件重复编译问题**：理论上#pragma once 和 #ifndef都能解决这个问题，但实际中都可能产生一些问题：#ifndef XXX_H 需要保证XXX的名字和其它文件都不一致，否则会少编译一个头文件，找不到定义（本lab中就遇到过）；

   而#pragma once的原理是针对每一个物理意义的文件（不管是否重名，也不管内容）做记录，每个文件只编译一次，这就可能导致某些相同头文件重复编译，如自己引了库A和库B，结果库B中也引了库A，那仅用#pragma once就会重复编译了

   一个更好的编程实践是同时使用二者：

   ```cpp
   #pragma once
   
   #ifndef XXX_H
   #define XXX_H
   
   #endif
   ```

   这样做还有个好处，就是#pragma once会直接跳过编译过的文件，而#ifndef还会扫描整个文件去找#endif，所以在大型文件中#pragma once可加速编译，跳过重复文件



#### 四、TODO

1. 用tsf实现和外部程序通信，而不是Qlinedit输出识别内容
2. 中文联想方案的结果实用性十分依赖于词典txt文件是否真实可靠、有合理的大数据作为支撑，但本输入法使用的词典内容本身不是特别合理，没有标注高频用词，导致联想效果一般，暂时没找到更好的词典文件；如果要自己构建一个词典的话，思路大概是找几本内容足够多的书，python中文分词统计各个词语的出现次数、计算概率、形成词典