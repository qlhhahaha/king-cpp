## LAB2：大文件搜索

#### 性能瓶颈分析和调试经验：

1. **windows API和操作系统的工程经验太差**

   使用windows原生库进行内存映射时犯了两个错： 一是没有用vs性能分析工具验证，想当然地觉得MapViewOfFile()把文件内容映射到内存这个操作会很耗时，企图把它修改为多线程版本实现分段映射，后来一测性能发现根本没必要

   ![](append.png)

   ![](map.png)

   绝大部分时间都耗在了文件内容从内存copy到string中这个操作上（即append()），MapViewOfFile()几乎没消耗什么性能，查资料分析原因，可能是因为现代OS的虚拟内存机制在处理内存映射时很高效，应该有类似于linux中**懒加载lazy copy**的操作，前期只建立映射关系、等程序真的访问到相应数据时才会把相应的文件页面加载到内存中（这也解释了为啥所有的性能消耗都集中在append()上，因为那会儿才真的去加载文件）

   二是设置内存映射的offset时出错，设为1、2、3全都bug，最后找到官方文档才知道要读取sysInfo.dwPageSize（一般为4096KB）并把offset设成其整数倍，但设成4096还是bug，思索半天发现是这两个函数的默认单位不统一，offset应该是4096*1024

   综上说明存在操作系统等理论知识在实际运用时不熟练；初次使用不熟悉的API时盲目相信gpt、而不是去查官方手册等问题

   

2. **控制多线程的锁消耗**

   开始的策略为，每个线程搜索一个文件分块chunk，搜索时对关键字的次数、位置统计加互斥锁mutex，避免计数错误，但发现两个问题，一是keyword出现次数过多时开销太大（如示例中出现600w次的那个关键字耗时2000ms，但其它的200ms就能搞定），二是增加线程耗时不减反增

   分析两个问题都是由**锁资源开销**导致的，频繁进入临界区代码(即修改keywordCount)或者盲目增大线程数量都会导致多线程竞争和阻塞更多、在用户态和内存态之间的切换更频繁，从而增大开销；此外线程增多还可能导致**cache被频繁地刷新**，从而命中率下降

   解决策略是使用无锁搜索，给每个chunk都设一个keywordCount，各线程统计各自的，最后再一次性合并，从而避免使用锁；如果没办法做到无锁，也至少尽量去**降低锁的颗粒度**



3. **资源回收问题**

   使用UnmapViewOfFile()和CloseHandle()对内存映射资源进行回收时，由于反复调整主函数和多线程函数中的内容，又忘了增删相应的资源回收，导致一堆异常（多线程先后顺序不一，所以还不是每次都会异常，时有时无），所以使用多线程时一定要格外关注资源回收问题，**最好使用RAII机制**



#### TODO：

1. 没想明白线程池和内存池怎么用进去
2. 搜索主要用的是std::string.find()，没有自己写字符串匹配算法，因为性能测试发现纯find()的耗时不超过5ms，感觉改成KMP等算法好像收益不是很大？
