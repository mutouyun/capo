#  The Capo Library

C++标准库的轻量级拓展

* 支持的编译器(C++11以上): msvc(12.0ctp+), gcc(4.8.0+), clang(3.3.0+)
* 不需要编译, 每个功能都是独立的头文件(hpp).

##

    - src  -> 代码目录
    - test -> 测试目录, 包含: msvc项目文件(test.sln), qtcreator项目文件(test.pro)

##

    > algorithm     ...working      - 算法
    > chrono        ...working      - 时间操作【暂无】
    > finalizer     ...working      - 资源管理
    > macro         ...working      - 预处理宏定义
    > proofing      ...working      - 代码防呆（错误预防）
    > random        ...working      - 便捷的随机数工具
    > thread        ...working      - 多线程（spin_lock、semaphore、thread_pool、etc...）
    > type          ...working      - 类型操作
    > utility       ...working      - 实用小工具

### ToDoList

    finalizer.make_guard            - 用于创建局部guard（例如根据传入参数自动生成合适的std::unique_lock）
    algorithm.series                - 序列生成（可和algorithm.range搭配使用）
    chrono.stopwatch                - 秒表
    pattern.singleton               - 单例
    pattern.trackable               - 遍历出所有该类型尚未销毁的对象实例
    memory                          - capo的自定义allocator
    container                       - 默认使用capo.memory.allocator的std容器
    utility.optional                - Just like boost::optional
    utility.any                     - Just like boost::any
    string.string                   - std::string的功能拓展
    stream                          - 流对象
    proofing.trace                  - 调试输出
    thread.blocking_queue           - 这个没有什么好说明的。。
    delegate.function               - 默认使用capo.memory.allocator的std::function
    thread.thread_pool              - 线程池
    delegate.signal                 - 线程安全、支持多线程异步模式的信号/槽[(?)use message loop]
    finalizer.ref_counter           - 基于引用计数的资源管理基类
    coroutine                       - 协程库