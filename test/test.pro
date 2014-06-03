TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp

HEADERS += \
    test_type.h \
    test_utility.h \
    test_algorithm.h \
    ../src/algorithm/max_min.hpp \
    ../src/macro/macro_func.hpp \
    ../src/type/type_list.hpp \
    ../src/type/type_name.hpp \
    ../src/utility/countof.hpp \
    ../src/utility/operator.hpp \
    ../src/macro/macro_pp.hpp \
    ../src/utility/inherit.hpp \
    ../src/utility/noncopyable.hpp \
    ../src/algorithm/range.hpp \
    ../src/proofing/printf.hpp \
    test_proofing.h \
    ../src/finalizer/scope_guard.hpp \
    ../src/proofing/assert.hpp \
    ../src/macro/macro_unused.hpp \
    ../src/thread/spin_lock.hpp \
    test_thread.h \
    ../src/thread/semaphore.hpp \
    ../src/thread/waiter.hpp \
    ../src/random/random.hpp \
    ../src/utility/make.hpp \
    ../src/pattern/singleton.hpp \
    ../src/chrono/stopwatch.hpp \
    test_chrono.h
