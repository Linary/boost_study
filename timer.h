/**
 *  timer library contains three components, they are "timer, progress_timer" and "progress_display".
 *  Below will be detailed.
 */


/***************************************************************************/
/* header and namespace
/***************************************************************************/
#include <boost/timer.hpp>  
#include <boost/progress.hpp>  
#include <iostream>  
#include <sstream>  
#include <fstream>  
#include <vector>  

using namespace std;   
using namespace boost; 

 
/***************************************************************************/
/* timer
/***************************************************************************/
void timer_test() {
    // timer 处理示例，timer 以处理几百小时之内，对精确度不高的时间统计。  
    timer t;  
    cout << "最大处理时间：" << t.elapsed_max() / 3600 << " h" << endl;  
    cout << "最小处理时间：" << t.elapsed_min() << " s" << endl;  
    cout << "已经逝去时间：" << t.elapsed() << " s" << endl;  
    system("pause");  
    cout << "暂停逝去时间:" << t.elapsed() << " s" << endl;  
    t.restart();  
    cout << "重启逝去时间:" << t.elapsed() << " s" << endl;  
    system("pause");  
    cout << "暂停逝去时间:" << t.elapsed() << " s" << endl;  
}
 
 
/***************************************************************************/
/*  progress_timer
/***************************************************************************/
void progress_timer_test() {
    // 示例1
    // progress_timer 继承了 timer 的全部能力 
    progress_timer t;    
    // 手动打印出流逝的时间
    cout << "progress_timer: " << t.elapsed() << endl;   

    // 示例2 
    // progress_timer 会在这里析构，自动输出时间
    {  
        progress_timer t;  
    } 
      
    // 示例3
    // progress_timer 输出重定向至 stringstream 中   
    stringstream ss;  
    {  
        // 使 progress_timer 输出到 ss
        progress_timer t(ss);  
    }  
    cout << "ss: " << ss.str() << endl;
}

/***************************************************************************/
/*  扩展计时精度
/*  progress_timer 的精度只有小数点后两位
/*  如果想有更高精度的计时器，可以仿照 progress_timer 自己写一个
/***************************************************************************/
template <int N = 2>
class new_progress_timer : public boost::timer {
public:
    new_progress_timer(std::ostream & os = std::cout) : m_os(os) {
        // 这里应该使用静态断言的
        assert(N >= 0 && N <= 10);
    }

    ~new_progress_timer() {
        try {
            // 保存流的状态
            std::istream::fmtflags old_flags = m_os.setf(std::istream::fixed,
                std::istream::floatfield);
            std::streamsize old_prec = m_os.precision(N);

            // 输出时间
            m_os << elapsed() << " s" << std::endl;

            // 恢复流的状态
            m_os.flags(old_flags);
            m_os.precision(old_prec);
        } catch (...) {}    // 析构函数绝不能抛出异常
    }
private:
    std::ostream m_os;
};

/** 
 *  使用模板特化
 */
template<>
class new_progress_timer<2> : public boost::progress_timer {}; 


/***************************************************************************/
/*  progress_display
/*  显示执行进度
/***************************************************************************/
void progress_display_test() {
    // 可在控制台显示程序的执行进度，但是无法把进度显示输出与程序的输出分离 
    vector<string> v(100);  
    ofstream fs("c:/test.txt");  
    // 初始化进度的基数，也就是总进度
    progress_display pd(v.size());  
      
    for (auto pos = v.begin(); pos != v.end(); ++pos) {  
        fs << *pos << endl;  
        // 更新进度显示
        ++pd;  
    }  
}
// 输出示例
// 0%   10   20   30   40   50   60   70   80   90   100% 
// |----|----|----|----|----|----|----|----|----|----| 
// ***************************************************