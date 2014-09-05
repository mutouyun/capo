TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

CONFIG(release, debug|release){
    DEFINES += NDEBUG
}

SOURCES += main.cpp \
    ../src/thread_local_ptr.cpp

HEADERS += \
    test_type.h \
    test_utility.h \
    test_algorithm.h \
    ../include/algorithm/max_min.hpp \
    ../include/macro/macro_func.hpp \
    ../include/type/type_list.hpp \
    ../include/type/type_name.hpp \
    ../include/utility/countof.hpp \
    ../include/utility/operator.hpp \
    ../include/macro/macro_pp.hpp \
    ../include/utility/inherit.hpp \
    ../include/utility/noncopyable.hpp \
    ../include/algorithm/range.hpp \
    ../include/proofing/printf.hpp \
    test_proofing.h \
    ../include/finalizer/scope_guard.hpp \
    ../include/proofing/assert.hpp \
    ../include/macro/macro_unused.hpp \
    ../include/thread/spin_lock.hpp \
    test_thread.h \
    ../include/thread/semaphore.hpp \
    ../include/thread/waiter.hpp \
    ../include/random/random.hpp \
    ../include/utility/make.hpp \
    ../include/pattern/singleton.hpp \
    ../include/chrono/stopwatch.hpp \
    test_chrono.h \
    ../include/type/type_concept.hpp \
    ../include/algorithm/sequence.hpp \
    ../include/memory/construct.hpp \
    test_memory.h \
    ../include/container/deque.hpp \
    ../include/container/forward_list.hpp \
    ../include/container/hash_map.hpp \
    ../include/container/hash_set.hpp \
    ../include/container/list.hpp \
    ../include/container/map.hpp \
    ../include/container/queue.hpp \
    ../include/container/set.hpp \
    ../include/container/stack.hpp \
    ../include/container/vector.hpp \
    ../include/memory/allocator_wrapper.hpp \
    ../include/memory/alloc_standard.hpp \
    ../include/memory/chunk.hpp \
    ../include/memory/fixed_pool.hpp \
    ../include/type/constant_array.hpp \
    ../include/memory/variable_pool.hpp \
    ../include/memory/alloc_thread_cache.hpp \
    ../include/memory/cache_pool.hpp \
    ../include/thread/thread_wrapper.hpp \
    ../include/memory/allocator.hpp \
    ../include/thread/thread_local_ptr.hpp
