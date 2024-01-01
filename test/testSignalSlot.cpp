#include "Signal.hpp"
#include <iostream>
#include <functional>

class Sender{
public:
    SimpleSignal<int> m_sig; //转递int类型值的信号，使用emit方法发出信号，发送值。

    void setValue(int a) noexcept
    {
        if(m_value != a){
            m_value = a;
            m_sig.emit(m_value);
        }
    }

private:
    int m_value = 0;
};

class Receiver{
public:
    void valueChanged(int a)
    {
        std::cout << "Member function received value: " << a << std::endl;
    }
};

void printValue(int a)
{
    std::cout << "Regular function received value: " << a << std::endl;
}

int main(){
    Sender s;
    Receiver r;
    s.m_sig.connect(&r, &Receiver::valueChanged);
    s.m_sig.connect(&printValue);
    s.m_sig.connect([](int a){ std::cout << "Lambda expression received value: " << a << std::endl; }); //注意lambda表达式不能disconnect。

    s.setValue(1); //会触发槽函数

    //断开连接
    //不建议用很多的disconnect
    //s.m_sig.disconnect(&r); 
    s.m_sig.disconnect(&r, &Receiver::valueChanged);
    s.m_sig.disconnect(&printValue);

    s.setValue(2);
}
