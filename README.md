#  The Capo Library

C++标准库的轻量级拓展

* 支持的编译器(C++14以上): msvc(14.0+), gcc(4.9.0+), clang(3.3.0+).
* 每个功能都是独立的头文件(hpp), 除了Win下的thread_local_ptr之外, 不需要编译.

##

    - build -> sln && Makefiles
    - capo  -> 所有的公开接口及实现
    - src   -> 需要编译的代码，及私有接口和实现
    - test  -> 测试用例
    - third -> 引入的第三方库

### ToDoList

    utility.optional                - Just like boost::optional
    utility.any                     - Just like boost::any
    string.string                   - std::string的功能拓展
    proofing.trace                  - 调试输出
    thread.blocking_queue           - 多线程下的blocking_queue
    thread.thread_pool              - 线程池
    coroutine                       - 协程库
    ......
