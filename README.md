# metalog

allows logging to stdout or stderr and/or an output file. levels of logging is defined using a custom struct that is passed as a template.


## Log
normal logging:
```cpp
Log(struct::type type, string format, args...)
```

## Timed Log
does not log the message with specific `id` unless `msec` has passed since the last time it was published. the `id` is an integer starting from `0`
and should be kept as small as possible (but not necessarily consecutive) since it's backed by a vector. it could've been a hash table but a vector is faster.
```cpp
TimedLog(struct::type type, id, msec, string format, args...)
```

## Unique Log
backed by a hashmap, it's designed to only go through for state changes.