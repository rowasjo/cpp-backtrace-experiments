## Backtrace QThread

## 1. Unhandled exception raised in QThread

The thrown std::bad_optional_access exception causes program termination.

The program output is:
```
terminate called after throwing an instance of 'std::bad_optional_access'
  what():  bad optional access
Aborted
```

The coredump backtrace is:
```
#0  __pthread_kill_implementation (no_tid=0, signo=6, threadid=139805011584576) at ./nptl/pthread_kill.c:44
#1  __pthread_kill_internal (signo=6, threadid=139805011584576) at ./nptl/pthread_kill.c:78
#2  __GI___pthread_kill (threadid=139805011584576, signo=signo@entry=6) at ./nptl/pthread_kill.c:89
#3  0x00007f26e66fb476 in __GI_raise (sig=sig@entry=6) at ../sysdeps/posix/raise.c:26
#4  0x00007f26e66e17f3 in __GI_abort () at ./stdlib/abort.c:79
#5  0x00007f26e69a4b9e in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#6  0x00007f26e69b020c in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#7  0x00007f26e69b0277 in std::terminate() () from /lib/x86_64-linux-gnu/libstdc++.so.6
#8  0x00007f26e6bd0401 in qTerminate() () from /lib/x86_64-linux-gnu/libQt6Core.so.6
#9  0x00007f26e6be4e62 in ?? () from /lib/x86_64-linux-gnu/libQt6Core.so.6
#10 0x00007f26e674dac3 in start_thread (arg=<optimized out>) at ./nptl/pthread_create.c:442
#11 0x00007f26e67df850 in clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:81
```

Exception handling in Qt Thread obscures the origin throw location by unwinding the stack.

Presumably this is due to the following code in qthread_unix.cpp:

```
namespace {
template <typename T>
void terminate_on_exception(T &&t)
{
#ifndef QT_NO_EXCEPTIONS
    try {
#endif
        std::forward<T>(t)();
#ifndef QT_NO_EXCEPTIONS
#ifdef __GLIBCXX__
    // POSIX thread cancellation under glibc is implemented by throwing an exception
    // of this type. Do what libstdc++ is doing and handle it specially in order not to
    // abort the application if user's code calls a cancellation function.
    } catch (abi::__forced_unwind &) {
        throw;
#endif // __GLIBCXX__
    } catch (...) {
        qTerminate();
    }
#endif // QT_NO_EXCEPTIONS
}
} // unnamed namespace

void *QThreadPrivate::start(void *arg)
{
#if !defined(Q_OS_ANDROID)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
#endif
    pthread_cleanup_push(QThreadPrivate::finish, arg);

    terminate_on_exception([&] {                              <-- exception handling
        QThread *thr = reinterpret_cast<QThread *>(arg);
        QThreadData *data = QThreadData::get2(thr);
...
```

## 2. Unhandled exception raised in QThread with noexcept

Add noexcept specifier to the 'asyncSignalEvent' function and recompile. 
The noexcept specifier takes precedence over the thread exception handler.

The program output is:
```
terminate called after throwing an instance of 'std::bad_optional_access'
  what():  bad optional access
Aborted
```

The coredump backtrace is:
```
#0  __pthread_kill_implementation (no_tid=0, signo=6, threadid=140516896118336) at ./nptl/pthread_kill.c:44
#1  __pthread_kill_internal (signo=6, threadid=140516896118336) at ./nptl/pthread_kill.c:78
#2  __GI___pthread_kill (threadid=140516896118336, signo=signo@entry=6) at ./nptl/pthread_kill.c:89
#3  0x00007fcca60fa476 in __GI_raise (sig=sig@entry=6) at ../sysdeps/posix/raise.c:26
#4  0x00007fcca60e07f3 in __GI_abort () at ./stdlib/abort.c:79
#5  0x00007fcca63a3b9e in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#6  0x00007fcca63af20c in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#7  0x00007fcca63ae1e9 in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#8  0x00007fcca63ae959 in __gxx_personality_v0 () from /lib/x86_64-linux-gnu/libstdc++.so.6
#9  0x00007fcca62f7884 in ?? () from /lib/x86_64-linux-gnu/libgcc_s.so.1
#10 0x00007fcca62f7f41 in _Unwind_RaiseException () from /lib/x86_64-linux-gnu/libgcc_s.so.1
#11 0x00007fcca63af4cb in __cxa_throw () from /lib/x86_64-linux-gnu/libstdc++.so.6
#12 0x000056346cf50630 in std::__throw_bad_optional_access () at /usr/include/c++/11/optional:102
#13 0x000056346cf50745 in std::optional<std::basic_string_view<char, std::char_traits<char> > >::value() & (this=0x56346cf53040 <(anonymous namespace)::state>) at /usr/include/c++/11/optional:960
#14 0x000056346cf5032e in (anonymous namespace)::bad () at /home/bob/dev/repos/cpp-backtrace-experiments/bt_qt/main.cpp:19
#15 0x000056346cf50343 in (anonymous namespace)::f () at /home/bob/dev/repos/cpp-backtrace-experiments/bt_qt/main.cpp:24
#16 0x000056346cf50353 in (anonymous namespace)::invokeSlot () at /home/bob/dev/repos/cpp-backtrace-experiments/bt_qt/main.cpp:28
#17 0x000056346cf50363 in (anonymous namespace)::asyncSignalEvent () at /home/bob/dev/repos/cpp-backtrace-experiments/bt_qt/main.cpp:32
#18 0x000056346cf5037b in (anonymous namespace)::WorkerThread::run (this=0x7ffe2f04c9d0) at /home/bob/dev/repos/cpp-backtrace-experiments/bt_qt/main.cpp:37
#19 0x00007fcca67eafcf in ?? () from /lib/x86_64-linux-gnu/libQt6Core.so.6
#20 0x00007fcca614cac3 in start_thread (arg=<optimized out>) at ./nptl/pthread_create.c:442
#21 0x00007fcca61de850 in clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:81
```

In this example, the backtrace shows the original bad optional acces throw location.

However, backtrace may not necessarily show the origin throw location. Noexcept "may or may not unwind the stack", it is an implementation datail.

According to [cppreference.com](https://en.cppreference.com/w/cpp/language/noexcept_spec):
```
noexcept is an improved version of throw(), which is deprecated in C++11. 
Unlike pre-C++17 throw(), noexcept will not call std::unexpected, 
may or may not unwind the stack, and will call std::terminate, 
which potentially allows the compiler to implement noexcept without the runtime overhead of throw().
```

Observed on gcc8 -- backtrace showing std::terminate called at the location of the noexcept specifier, thereby obscuring the origin throw location.
