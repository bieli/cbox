# CBox — Generic Type Boxing for ANSI C

`CBox` is a lightweight, extensible system for boxing arbitrary data types in ANSI C. 
It provides runtime type tagging, trait-based polymorphism, and optional JSON serialization — all without relying on C++ features or external libraries.

## Motivation

`C` language lacks native support for generic containers, dynamic dispatch, or runtime type introspection.

`CBox` fills this gap by offering:
- Type-safe boxing of primitives and structs
- Trait-based cloning, printing, and serialization
- Debug macros for introspection
- Registry for dynamic type dispatch
- Clean separation of logic via modular headers

### Practical goals
Whether you're building interpreters, plugin systems, or just want a flexible way to manage heterogeneous data - CBox gives you the tools to do it in pure C99.

### RUST Box inspiration
Inspiration for this project was from `RUST` programming language.
You need to know, that not all `RUST` features were implemented yet.


## Features

- `CBox`: wraps any data with type info and trait behavior
- `CBoxTrait`: defines clone, destroy, print, and serialize functions
- `CBoxRegistry`: maps types to traits for dynamic dispatch
- *Debug macros*: inspect boxed values with one line
- *JSON serialization*: introspect values at runtime

## Installation

```bash
$ git clone https://github.com/yourname/cbox.git
$ cd cbox
$ make
```

This builds the static library libcbox.a and compiles examples in examples/.

## API Overview

### CBoxType
```c
typedef enum {
    CBOX_INT,
    CBOX_FLOAT,
    CBOX_DOUBLE,
    CBOX_STRUCT,
    CBOX_ARRAY
} CBoxType;
```

### CBox
```c
typedef struct CBox {
    void* data;
    size_t size;
    CBoxType type;
    CBoxTrait* trait;
    const struct CBox* parent; // optional: reference to original box
} CBox;
```

### CBoxTrait
```c
typedef struct CBoxTrait {
    void* (*clone)(const void* data);
    void  (*destroy)(void* data);
    void  (*print)(const void* data);
    void  (*serialize_json)(const void* data, char* out, size_t maxlen);
} CBoxTrait;
```

## Core Functions
```c
CBox* cbox_new_with_trait(const void* data, size_t size, CBoxType type, CBoxTrait* trait);
CBox* cbox_clone(const CBox* box);
void  cbox_free(CBox* box);
void  cbox_print(const CBox* box);
void  cbox_to_json(const CBox* box, char* out, size_t maxlen);
```

## Registry API
```c
void cbox_registry_init(CBoxRegistry* registry);
int  cbox_registry_register(CBoxRegistry* registry, CBoxType type, CBoxTrait* trait);
CBoxTrait* cbox_registry_get(CBoxRegistry* registry, CBoxType type);
```

## Debug Macros
```c
CBOX_DEBUG_INFO(box);        // prints type, size, pointer, and value
CBOX_DEBUG_JSON(box);        // prints JSON if available
CBOX_DEBUG_PRINT(box);       // prints value using trait->print
CBOX_DEBUG_ALL(box);         // prints everything above
CBOX_DEBUG_INFO_VERBOSE(box); // includes parent reference and JSON
```

## Supported Types

```bash
Type      | Trait Name          | JSON Support
-----------------------------------------------
int       | CBOX_INT_TRAIT      | Yes
float     | CBOX_FLOAT_TRAIT    | Yes
double    | CBOX_DOUBLE_TRAIT   | Yes
Person    | CBOX_PERSON_TRAIT   | Yes
IntArray  | CBOX_INTARRAY_TRAIT | Yes
```

You can define your own types and traits by implementing the four trait functions.


## Examples

### examples/cbox_demo.c

```bash
$ make
$ ./build/cbox_demo
== INT BOX ==
[CBox] type: 0, size: 4, data: 0x55ca186812d0
[CBox Value] CBox<int>: 42
CBox<int>: 42
[CBox JSON] {"type":"int","value":42,"address":"0x55ca186812d0"}
== INT CLONE ==
[CBox] type: 0, size: 4, data: 0x55ca18681730
[CBox] cloned from: 0x55ca186812a0
[CBox Value] CBox<int>: 42
CBox<int>: 42
[CBox JSON] {"type":"int","value":42,"address":"0x55ca18681730"}
== FLOAT BOX ==
[CBox] type: 1, size: 4, data: 0x55ca18681780
[CBox Value] CBox<float>: 3.140000
CBox<float>: 3.140000
[CBox JSON] {"type":"float","value":3.140000,"address":"0x55ca18681780"}
== PERSON BOX ==
[CBox] type: 3, size: 36, data: 0x55ca186817d0
[CBox Value] Person{name: Alice, age: 30}
Person{name: Alice, age: 30}
[CBox JSON] {"type":"Person","name":"Alice","age":30,"address":"0x55ca186817d0"}
```


### examples/cbox_debug.c

```bash
$ make
$ ./build/cbox_debug 
[CBox] type: 0, size: 4, data: 0x5575ab1862d0
[CBox Value] CBox<int>: 42
CBox<int>: 42
[CBox JSON] {"type":"int","value":42,"address":"0x5575ab1862d0"}
[CBox] type: 3, size: 36, data: 0x5575ab186730
[CBox Value] Person{name: Alice, age: 30}
Person{name: Alice, age: 30}
[CBox JSON] {"type":"Person","name":"Alice","age":30,"address":"0x5575ab186730"}
[CBox] type: 3, size: 16, data: 0x5575ab1867b0
[CBox Value] IntArray[3]: 1 2 3 
IntArray[3]: 1 2 3 
[CBox JSON] {"type":"IntArray","length":3,"values":[1,2,3],"address":"0x5575ab1867b0"}
```

### examples/cbox_registry.c

```bash
$ make
$ ./build/cbox_registry 
JSON: {"type":"int","value":42,"address":"0x5643e92982d0"}
```

### examples/cbox_task_queue.c

```bash
$ make
$ ./build/cbox_task_queue 
Processing item 0:
[CBox] type: 3, size: 17, data: 0x55efab2b4340
[CBox Value] Message: Hello from CBox!
Message: Hello from CBox!
[WARN] [CBox JSON] Function 'serialize_json' not exists!
Processing item 1:
[CBox] type: 0, size: 4, data: 0x55efab2b4390
[CBox Value] CBox<int>: 101
CBox<int>: 101
[CBox JSON] {"type":"int","value":101,"address":"0x55efab2b4390"}
Processing item 2:
[CBox] type: 3, size: 68, data: 0x55efab2b43e0
[CBox Value] Task[label: Download file, priority: 2]
Task[label: Download file, priority: 2]
[CBox JSON] {"type":"Task","label":"Download file","priority":2,"address":"0x55efab2b43e0"}
```


## Contributing

Pull requests are welcome!

If you want to add new trait types, registry extensions, or serialization formats (XML, YAML), feel free to fork and submit proposals.

