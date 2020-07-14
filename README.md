# metalog

allows logging to stdout or stderr and/or an output file. levels of logging is defined using a custom struct that is passed as a template.

## Log Types
metalog supports 3 kinds of logging:
### Log
normal logging:
```cpp
Log(struct::type type, string format, args...)
```

### Timed Log
does not log the message with specific `id` unless `msec` milliseconds have passed since the last time it was published. the `id` is an integer starting from `0` and should be kept as small as possible (but not necessarily consecutive) since it's backed by a vector. it could've been a hash table but a vector is faster.
```cpp
TimedLog(struct::type type, id, msec, string format, args...)
```

### Unique Log
backed by a hashed, it's designed to only go through for new state changes.
```cpp
UniqueLog(type, string format, args)
```

## Basic Usage
the default log struct (`deflog`) can be used as such. by default the output is set to stdout (i.e. no logfile) & maximum log level set to `deflog::VERBOSE`
```cpp
// using Log = MetaLog<deflog> in the main header allows this:
auto& metalog_instance = Log::GetInstance();
metalog_instance.Init(deflog::ERROR, output::out | output::logfile, "/home/hosein/test_log"); // could also be skipped for default settings
metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "A"); // prints
metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "A"); // nope
metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "A"); // nope
metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "B"); // prints
metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "A"); // nope
for (size_t i = 0; i < 50000000; ++i) {
    metalog_instance.TimedLog(deflog::ERROR, 0, 200, ">>>>>>>>>>>> timed log %\n", i); // prints 5 times/second
    metalog_instance.Log(deflog::ERROR, "<<<<<<<<<<<< normal log %\n", i);             // prints for all iterations
}
```
### Custom Log Structs
```cpp
struct new_log_struct {
    enum type : int {
        VERBOSE = 0,
        BASIC = 1,
        INTERMEDIATE = 2,
        ADVANCED = 3
    };
    std::string ToString(type tp) {
        switch (tp)
        {
        case VERBOSE:
            return "verbose";
        case BASIC:
            return "basic";
        case INTERMEDIATE:
            return "intermediate";
        case ADVANCED:
            return "advanced";
        }
    }
};
auto& metalog_instance = MetaLog<new_log_struct>::GetInstance();
metalog_instance.Init(new_log_struct::BASIC, output::out | output::logfile, "/home/usr/test_log");
metalog_instance.UniqueLog(new_log_struct::VERBOSE, "the state is %\n", "A");   // log level too low
metalog_instance.UniqueLog(new_log_struct::BASIC, "the state is %\n", "A");     // prints
metalog_instance.UniqueLog(new_log_struct::BASIC, "the state is %\n", "A");     // nope
metalog_instance.UniqueLog(new_log_struct::BASIC, "the state is %\n", "B");     // prints
metalog_instance.UniqueLog(new_log_struct::BASIC, "the state is %\n", "A");     // nope
```