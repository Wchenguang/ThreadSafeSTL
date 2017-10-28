#include <stack>
#include <exception>
#include <mutex>

//1.考虑在临界满载的状态下，临时变量无法构造而无法完成操作抛出异常，然而数据结构已经遭到破坏
//2.在多线程环境下，解决多个线程对共享资源的竞争

//栈为空的异常
struct EXP_emptyStack : std::exception{
    const char *what() const throw(){
        return "empty stack\n";
    }
};

//极简化后的操作 接口少以️避免错误
//封装的目的：

template <typename T>
class TS_Stack{
private:
    std::stack<T> mStack;
    mutable std::mutex mMutex;

public:

    TS_Stack() : mStack(std::stack<T>()) {};

    TS_Stack(const TS_Stack &otherStack){
        std::lock_guard<std::mutex> (otherStack.mMutex);
        mStack = otherStack.mStack;
    }

    //返回保护数据的引用 不安全
    TS_Stack &operator =(const TS_Stack &otherStack) = delete;

    //移动语义的push
    void push(const T &&element){
        std::lock_guard<std::mutex> guard(mMutex);
        mStack.push(element);
    }
    //常量引用版本的push
    void push(const T &element){
        std::lock_guard<std::mutex> guard(mMutex);
        mStack.push(element);
    }

    //临时变量无法构造会抛出异常但是不会影响数据结构
    T top(){
        return mStack.top();
    }

    //为了防止临界状态下的临时变量无法构造，导致栈已经删除栈顶元素却无法返回临时变量
    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> guard(mMutex);
        if(mStack.empty())
            throw EXP_emptyStack();
        std::shared_ptr<T> const ptr(std::make_shared<T>((mStack.top())));
        mStack.pop();
        return ptr;
    }

    void pop(T &elementRef){
        std::lock_guard<std::mutex> guard(mMutex);
        if(mStack.empty())
            throw EXP_emptyStack();
        elementRef = mStack.top();
        mStack.pop();
    }
    //上锁即可
    bool empty(){
        std::lock_guard<std::mutex> guard(mMutex);
        return mStack.empty();
    }

    size_t size(){
        std::lock_guard<std::mutex> guard(mMutex);
        return mStack.size();
    }

};