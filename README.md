# C++ Backtrace Experiments

## Scenario

During stress test, unexpected exception is raised, exception is caught and error message is printed:
```
Consume std::exception raised by slot invocation: bad optional access
```

The error message does not include origin where it was raised. The exception could have been raised in many locations.

Not handling the exception allowing program to terminate with full backtrace of origin throw location may provide better diagnostics.

## Core Dumps -- Ubuntu LTS 22.04 on WSL 2

Check current core dump location:
```
cat /proc/sys/kernel/core_pattern
```

Expected result is '/mnt/wslg/dumps'.

Enable coredumps:
```
ulimit -c unlimited
```

## Testing

Run:
```
<build_dir>/bt_std/bt_std
```

Load coredump and print backtrace:
```
gdb <build_dir>/bt_std/bt_std <coredump>
bt
```
