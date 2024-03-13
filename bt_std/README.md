## Backtrace std::thread

## 1. signal-slot try-catch

The try-catch in the 'invokeSlot' function consumes all exceptions preventing program termination.

## 2. No try-catch

Remove the try-catch code in 'invokeSlot' and recompile. The program now aborts.

The program output is:
```
terminate called after throwing an instance of 'std::bad_optional_access'
  what():  bad optional access
Aborted
```

The coredump backtrace is:
```
#0  __pthread_kill_implementation (no_tid=0, signo=6, threadid=140297491854912) at ./nptl/pthread_kill.c:44
#1  __pthread_kill_internal (signo=6, threadid=140297491854912) at ./nptl/pthread_kill.c:78
#2  __GI___pthread_kill (threadid=140297491854912, signo=signo@entry=6) at ./nptl/pthread_kill.c:89
#3  0x00007f998e3d1476 in __GI_raise (sig=sig@entry=6) at ../sysdeps/posix/raise.c:26
#4  0x00007f998e3b77f3 in __GI_abort () at ./stdlib/abort.c:79
#5  0x00007f998e67ab9e in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#6  0x00007f998e68620c in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#7  0x00007f998e686277 in std::terminate() () from /lib/x86_64-linux-gnu/libstdc++.so.6
#8  0x00007f998e6864d8 in __cxa_throw () from /lib/x86_64-linux-gnu/libstdc++.so.6
#9  0x000055c1b54026b4 in std::__throw_bad_optional_access () at /usr/include/c++/11/optional:102
#10 0x000055c1b54026fd in std::optional<std::basic_string_view<char, std::char_traits<char> > >::value() & (this=0x55c1b5407040 <(anonymous namespace)::state>) at /usr/include/c++/11/optional:960
#11 0x000055c1b540234d in (anonymous namespace)::bad () at /home/bob/dev/repos/cpp-backtrace-experiments/bt_std/main.cpp:22
#12 0x000055c1b5402362 in (anonymous namespace)::f () at /home/bob/dev/repos/cpp-backtrace-experiments/bt_std/main.cpp:27
#13 0x000055c1b5402372 in (anonymous namespace)::invokeSlot () at /home/bob/dev/repos/cpp-backtrace-experiments/bt_std/main.cpp:33
#14 0x000055c1b5402382 in (anonymous namespace)::asyncSignalEvent () at /home/bob/dev/repos/cpp-backtrace-experiments/bt_std/main.cpp:42
#15 0x000055c1b5403233 in std::__invoke_impl<void, void (*)()> (__f=@0x55c1b5c51eb8: 0x55c1b5402375 <(anonymous namespace)::asyncSignalEvent()>) at /usr/include/c++/11/bits/invoke.h:61
#16 0x000055c1b54031df in std::__invoke<void (*)()> (__fn=@0x55c1b5c51eb8: 0x55c1b5402375 <(anonymous namespace)::asyncSignalEvent()>) at /usr/include/c++/11/bits/invoke.h:96
#17 0x000055c1b5403180 in std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul> (this=0x55c1b5c51eb8) at /usr/include/c++/11/bits/std_thread.h:259
#18 0x000055c1b5403150 in std::thread::_Invoker<std::tuple<void (*)()> >::operator() (this=0x55c1b5c51eb8) at /usr/include/c++/11/bits/std_thread.h:266
#19 0x000055c1b5403130 in std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run (this=0x55c1b5c51eb0) at /usr/include/c++/11/bits/std_thread.h:211
#20 0x00007f998e6b4253 in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#21 0x00007f998e423ac3 in start_thread (arg=<optimized out>) at ./nptl/pthread_create.c:442
#22 0x00007f998e4b5850 in clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:81
```

The backtrace shows the original bad optional acces throw location.
