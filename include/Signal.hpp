#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>

#ifndef POINTER_BUFFER_SIZE
#define POINTER_BUFFER_SIZE=24
#endif

//自定义的储存函数指针的结构体
struct FuncPointerBuffer {
    char union_ptr[POINTER_BUFFER_SIZE];
    FuncPointerBuffer(){
        memset(union_ptr, 0, POINTER_BUFFER_SIZE);
    }
};

// 自定义哈希函数
struct FuncPointerBufferHash {
    size_t operator()(const FuncPointerBuffer& fpb) const {
        std::string str(fpb.union_ptr, sizeof(fpb.union_ptr));
        return std::hash<std::string>{}(str);
    }
};

// 自定义相等性比较函数
struct FuncPointerBufferEqual {
    bool operator()(const FuncPointerBuffer& lhs, const FuncPointerBuffer& rhs) const {
        return std::memcmp(lhs.union_ptr, rhs.union_ptr, sizeof(lhs.union_ptr)) == 0;
    }
};

template<typename... Args>
class SimpleSignal {

public:
    using SlotFunc = std::function<void(Args...)>;
    using SlotId = size_t;

    //处理lambda表达式的连接
    void connect(SlotFunc s)
    {
        m_lambda_slots.emplace_back(std::move(s));
    }

    //处理普通函数的连接
    template<typename Func>
    void connect(Func* f)
    {
        FuncPointerBuffer func_pointer;
        std::memcpy(&func_pointer, &f, sizeof(f));
        m_pointer_slots[static_cast<void *>(nullptr)].emplace(func_pointer, [=](Args... args){ f(args...); });
    }

    //处理成员函数的连接
    template<typename T>
    void connect(T* t, void(T::* func)(Args...)) {
        FuncPointerBuffer func_pointer;
        std::memcpy(&func_pointer, &func, sizeof(func));
        m_pointer_slots[reinterpret_cast<void *>(t)].emplace(func_pointer, [=](Args... args){ (t->*func)(args...); });
    }

    //断开指定普通函数的连接
    template<typename Func, std::enable_if_t<std::is_function_v<std::remove_pointer_t<Func>>>* = nullptr>
    void disconnect(Func* f)
    {
        FuncPointerBuffer slot_ptr;
        std::memcpy(&slot_ptr, &f, sizeof(f));
        m_pointer_slots[nullptr].erase(slot_ptr);

    }

    //断开指定成员函数的连接    
    template<typename T>
    void disconnect(T* t, void(T::* func)(Args...)) {
        FuncPointerBuffer slot_ptr;
        std::memcpy(&slot_ptr, &func, sizeof(func));
        if(m_pointer_slots.find(reinterpret_cast<void *>(t)) != m_pointer_slots.end())
        {
            m_pointer_slots[reinterpret_cast<void *>(t)].erase(slot_ptr);
        }
    }

    //断开与指定对象的所有连接，可用于Receiver的析构中，实现自动断开连接 
    template<typename T, std::enable_if_t<std::is_class_v<T>>* = nullptr>
    void disconnect(T* t) {
        m_pointer_slots.erase(reinterpret_cast<void *>(t));

    }

    //发出信号，触发槽
    void emit(Args... args)
    {
        for(auto& s : m_lambda_slots)
        {
            s(args...);
        }

        for (auto& slots : m_pointer_slots)
        {
            for(auto& s : slots.second)
            {
                s.second(args...);
            }
        }
    }

private:
    //管理函数指针的成员
    std::unordered_map<void*, std::unordered_map<FuncPointerBuffer, SlotFunc, 
                        FuncPointerBufferHash, FuncPointerBufferEqual>> m_pointer_slots;

    //管理lambda表达式的成员
    std::vector<SlotFunc> m_lambda_slots;
};

#endif
