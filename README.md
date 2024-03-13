# C++ Backtrace Experiments

## Scenario

During stress test, unexpected exception is raised, exception is caught and error message is printed:
```
Consume std::exception raised by slot invocation: bad optional access
```

The error message does not include origin where it was raised. The exception could have been raised in many locations.

## Core Dumps

Check current core dump location:
```
cat /proc/sys/kernel/core_pattern
```

Expected result on Ubuntu LTS 22.04 on WSL 2 is '/mnt/wslg/dumps'.

Enable coredumps:
```
ulimit -c unlimited
```

## bt_std

1. try-catch obscures throw location, program running in potentially corrupt state
2. remove try-catch. std::terminate called immediately at bad optional access throw origin location
3. add back try-catch and add noexcept in f1. std::terminate called immediately at bad optional acces throw origin location
