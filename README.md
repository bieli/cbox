# CBox - Generic Type Boxing for ANSI C

![CI status](https://github.com/bieli/cbox/actions/workflows/ci.yml/badge.svg)
![github_tag](https://img.shields.io/github/v/tag/bieli/cbox)


`CBox` is a lightweight, extensible system for boxing arbitrary data types in ANSI C. 
It provides runtime type tagging, trait-based polymorphism, and optional JSON serialization — all without relying on C++ features or external libraries.

```c                                            
        CCCCCCCCCCCCCBBBBBBBBBBBBBBBBB        OOOOOOOOO     XXXXXXX       XXXXXXX
     CCC::::::::::::CB::::::::::::::::B     OO:::::::::OO   X:::::X       X:::::X
   CC:::::::::::::::CB:::::::::::::::::B  OO:::::::::::::OO X:::::X       X:::::X
  C:::::CCCCCCCC::::CBB:::::BBBBBB::::::BO:::::::OOO:::::::OX::::::X     X::::::X
 C:::::C       CCCCCC  B::::B     B:::::BO::::::O   O::::::OXXX:::::X   X:::::XXX
C:::::C                B::::B     B:::::BO:::::O     O:::::O   X:::::X X:::::X   
C:::::C                B::::B     B:::::BO:::::O     O:::::O    X:::::X:::::X    
C:::::C                B::::BBBBBB:::::B O:::::O     O:::::O     X:::::::::X     
C:::::C                B:::::::::::::BB  O:::::O     O:::::O     X:::::::::X     
C:::::C                B::::BBBBBB:::::B O:::::O     O:::::O    X:::::X:::::X    
C:::::C                B::::B     B:::::BO:::::O     O:::::O   X:::::X X:::::X   
 C:::::C       CCCCCC  B::::B     B:::::BO::::::O   O::::::OXXX:::::X   X:::::XXX
  C:::::CCCCCCCC::::CBB:::::B     B:::::BO:::::::OOO:::::::OX::::::X     X::::::X
   CC:::::::::::::::CB::::::BBBBBB:::::B  OO:::::::::::::OO X:::::X       X:::::X
     CCC::::::::::::CB::::::::::::::::B     OO:::::::::OO   X:::::X       X:::::X
        CCCCCCCCCCCCCBBBBBBBBBBBBBBBBB        OOOOOOOOO     XXXXXXX       XXXXXXX
```

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
$ git clone https://github.com/bieli/cbox.git
$ cd cbox
$ make
```

This builds:
- the static library `libcbox.a` inside created `lib/` directory
- the shared library `libcbox.so` inside created `lib/` directory
- and compiles all examples from `examples/` to `build/` directory

## API Overview

### CBoxType
```c
typedef enum {
    CBOX_BOOL,
    CBOX_INT,
    CBOX_FLOAT,
    CBOX_DOUBLE,
    CBOX_DECIMAL,
    CBOX_DATE,
    CBOX_CURRENCY,
    CBOX_STRUCT,
    CBOX_ARRAY,
    CBOX_CONTAINER,
    CBOX_CSV
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
Type          | Trait Name           | JSON Support
-------------------------------------------------------
bool          | CBOX_BOOL_TRAIT      | Yes
int           | CBOX_INT_TRAIT       | Yes
float         | CBOX_FLOAT_TRAIT     | Yes
double        | CBOX_DOUBLE_TRAIT    | Yes
decimal       | CBOX_DECIMAL_TRAIT   | Yes
data          | CBOX_DATE_TRAIT      | Yes
currency      | CBOX_CURRENCY_TRAIT  | Yes
struct        | CBOX_STRUCT_TRAIT    | Yes
Person        | CBOX_PERSON_TRAIT    | Yes
Product       | CBOX_PRODUCT_TRAIT   | Yes
IntArray      | CBOX_INTARRAY_TRAIT  | Yes
CBoxContainer | CBOX_CONTAINER_TRAIT | Yes
CBoxCsv       | CBOX_CSV_TRAIT       | Yes
```

You can define your own types and traits by implementing the four trait functions.


## Examples

### [examples/cbox_demo.c](examples/cbox_demo.c)

```bash
$ make
$ ./build/cbox_demo
== INT BOX ==
[CBox] type: 0 (int), size: 4, data: 0x564814bbb2d0
[CBox Value] CBox<int>: 42
CBox<int>: 42
[CBox JSON] {"type":"int","value":42,"address":"0x564814bbb2d0"}
== INT CLONE ==
[CBox] type: 0 (int), size: 4, data: 0x564814bbb730
[CBox] cloned from: 0x564814bbb2a0
[CBox Value] CBox<int>: 42
CBox<int>: 42
[CBox JSON] {"type":"int","value":42,"address":"0x564814bbb730"}
== FLOAT BOX ==
[CBox] type: 1 (float), size: 4, data: 0x564814bbb780
[CBox Value] CBox<float>: 3.140000
CBox<float>: 3.140000
[CBox JSON] {"type":"float","value":3.140000,"address":"0x564814bbb780"}
== PERSON BOX ==
[CBox] type: 4 (struct), size: 36, data: 0x564814bbb7d0
[CBox Value] Person{name: Alice, age: 30}
Person{name: Alice, age: 30}
[CBox JSON] {"type":"Person","name":"Alice","age":30,"address":"0x564814bbb7d0"}
== DECIMAL BOX ==
[CBox] type: 3 (decimal), size: 8, data: 0x564814bbb830
[CBox Value] CBox<decimal>: 12.34
CBox<decimal>: 12.34
[CBox JSON] {"type":"Decimal","value":"12.34"}
```

### [examples/cbox_debug.c](examples/cbox_debug.c)

```bash
$ make
$ ./build/cbox_debug 
[CBox] type: 0 (int), size: 4, data: 0x5575ab1862d0
[CBox Value] CBox<int>: 42
CBox<int>: 42
[CBox JSON] {"type":"int","value":42,"address":"0x5575ab1862d0"}
[CBox] type: 3 (struct), size: 36, data: 0x5575ab186730
[CBox Value] Person{name: Alice, age: 30}
Person{name: Alice, age: 30}
[CBox JSON] {"type":"Person","name":"Alice","age":30,"address":"0x5575ab186730"}
[CBox] type: 3 (struct), size: 16, data: 0x5575ab1867b0
[CBox Value] IntArray[3]: 1 2 3 
IntArray[3]: 1 2 3 
[CBox JSON] {"type":"IntArray","length":3,"values":[1,2,3],"address":"0x5575ab1867b0"}
```

### [examples/cbox_registry.c](examples/cbox_registry.c)

```bash
$ make
$ ./build/cbox_registry 
JSON: {"type":"int","value":42,"address":"0x5643e92982d0"}
```

### [examples/cbox_container_demo.c](examples/cbox_container_demo.c)

```bash
$ make
$ ./build/cbox_container_demo
CBoxContainer[3]:
  [0] CBox<decimal>: 1.99
  [1] IntArray[2]: 10 20 
  [2] IntArray[4]: 101 102 103 104 
== JSON summary: {"type":"CBoxContainer","count":3,"items":[{"type":"Decimal","value":"1.99"},{"type":"IntArray","length":2,"values":[10,20],"address":"0x558df07653a0"},{"type":"IntArray","length":4,"values":[101,102,103,104],"address":"0x558df0765430"}]}
```

### [examples/cbox_currency_demo.c](examples/cbox_currency_demo.c)

```bash
$ make
$ ./build/cbox_currency_demo
Printing currency object:
CBox<currency>: CBox<decimal>: 8425.95
 CBox<string>: "EUR"
JSON output:
{"type":"currency","amount":{"type":"Decimal","value":"8425.95"},"code":{"type":"string","value":"EUR"}}

CBox<currency>: CBox<decimal>: 10.12
 CBox<string>: "USD"
JSON output:
{"type":"currency","amount":{"type":"Decimal","value":"10.12"},"code":{"type":"string","value":"USD"}}
```

### [examples/cbox_task_queue.c](examples/cbox_task_queue.c)

```bash
$ make
$ ./build/cbox_task_queue 
Processing item 0:
[CBox] type: 3 (struct), size: 17, data: 0x55efab2b4340
[CBox Value] Message: Hello from CBox!
Message: Hello from CBox!
[WARN] [CBox JSON] Function 'serialize_json' not exists!
Processing item 1:
[CBox] type: 0 (int), size: 4, data: 0x55efab2b4390
[CBox Value] CBox<int>: 101
CBox<int>: 101
[CBox JSON] {"type":"int","value":101,"address":"0x55efab2b4390"}
Processing item 2:
[CBox] type: 3 (struct), size: 68, data: 0x55efab2b43e0
[CBox Value] Task[label: Download file, priority: 2]
Task[label: Download file, priority: 2]
[CBox JSON] {"type":"Task","label":"Download file","priority":2,"address":"0x55efab2b43e0"}
```

### [examples/cbox_product_demo.c](examples/cbox_product_demo.c)

```bash
$ make
$ ./build/cbox_product_demo 
[CBox] type: 3 (struct), size: 80, data: 0x55e9a6c7a2d0
[CBox Value] Product{name: Laptop, price: 1499.99, quantity: 3}
Product{name: Laptop, price: 1499.99, quantity: 3}
[CBox JSON] {"type":"Product","name":"Laptop","price":1499.99,"quantity":3,"address":"0x55e9a6c7a2d0"}
```

### [examples/cbox_array_demo.c](examples/cbox_array_demo.c)

```bash
$ make
$ ./build/cbox_array_demo 
[CBox] type: 5 (array), size: 16, data: 0x56003bdad310
[CBox Value] IntArray[3]: 1 2 3 
IntArray[3]: 1 2 3 
[CBox JSON] {"type":"IntArray","length":3,"values":[1,2,3],"address":"0x56003bdad310"}
[CBox] type: 5 (array), size: 16, data: 0x56003bdad770
[CBox] cloned from: 0x56003bdad2e0
[CBox Value] IntArray[3]: 1 2 3 
IntArray[3]: 1 2 3 
[CBox JSON] {"type":"IntArray","length":3,"values":[1,2,3],"address":"0x56003bdad770"}
[CBox] type: 5 (array), size: 16, data: 0x56003bdad820
[CBox Value] IntArray[4]: 41 42 43 44 
IntArray[4]: 41 42 43 44 
[CBox JSON] {"type":"IntArray","length":4,"values":[41,42,43,44],"address":"0x56003bdad820"}
```

### [examples/cbox_registry_pipeline.c](examples/cbox_registry_pipeline.c)

```bash
$ make
$ ./build/cbox_registry_pipeline 
== Pipeline Output ==
[CBox] type: 0 (int), size: 4, data: 0x5622d0d1a2d0
[CBox Value] CBox<int>: 42
CBox<int>: 42
[CBox JSON] {"type":"int","value":42,"address":"0x5622d0d1a2d0"}
[CBox] type: 1 (float), size: 4, data: 0x5622d0d1a320
[CBox Value] CBox<float>: 23.500000
CBox<float>: 23.500000
[CBox JSON] {"type":"float","value":23.500000,"address":"0x5622d0d1a320"}
[CBox] type: 3 (struct), size: 40, data: 0x5622d0d1a370
[CBox Value] SensorReading{id: sensor-A1, value: 78.90, time: 169765}
SensorReading{id: sensor-A1, value: 78.90, time: 169765}
[CBox JSON] {"type":"SensorReading","sensor_id":"sensor-A1","value":78.90,"timestamp":169765,"address":"0x5622d0d1a370"}
```

### [examples/cbox_nested_container_demo.c](examples/cbox_nested_container_demo.c)

```bash
$ make
$ ./build/cbox_nested_container_demo 
== Nested container structure:
CBoxContainer[2]:
  [0] CBoxContainer[2]:
  [0] CBox<decimal>: 1.11
  [1] CBox<decimal>: 2.22
  [1] CBoxContainer[2]:
  [0] IntArray[3]: 100 200 300 
  [1] CBox<decimal>: 3.33
== JSON summary: {"type":"CBoxContainer","count":2,"items":[{"type":"CBoxContainer","count":2,"items":[{"type":"Decimal","value":"1.11"},{"type":"Decimal","value":"2.22"}]},{"type":"CBoxContainer","count":2,"items":[{"type":"IntArray","length":3,"values":[100,200,300],"address":"0x55a16f98d480"},{"type":"Decimal","value":"3.33"}]}]}
```

### [examples/cbox_csv_demo.c](examples/cbox_csv_demo.c)

```bash
$ make
$ ./build/cbox_csv_demo 
CBoxCSV: 4 rows x 5 columns
Columns: Name, Age, Height, Member, JoinDate
Row 0: 
 Col 0: CBox<string>: "Alice"
 Col 1: CBox<int>: 30
 Col 2: CBox<double>: 1.650000
 Col 3: CBox<bool>: true
 Col 4: CBox<date>: 2021-06-15

Row 1: 
 Col 0: CBox<string>: "Bob"
 Col 1: CBox<int>: 25
 Col 2: CBox<double>: 1.800000
 Col 3: CBox<bool>: false
 Col 4: CBox<date>: 2022-01-10

Row 2: 
 Col 0: CBox<string>: "Charlie"
 Col 1: CBox<int>: 40
 Col 2: CBox<double>: 1.750000
 Col 3: CBox<bool>: true
 Col 4: CBox<date>: 2020-09-05

Row 3: 
 Col 0: CBox<string>: "Diana"
 Col 1: CBox<int>: 35
 Col 2: CBox<double>: 1.600000
 Col 3: CBox<bool>: false
 Col 4: CBox<date>: 2023-03-22

JSON:
{"type":"CBoxCSV","rows":[[{"type":"string","value":"Alice"},{"type":"int","value":30,"address":"0x559c1dc229d0"},{"type":"double","value":1.650000,"address":"0x559c1dc22a40"},{"type":"bool","value":true},{"type":"date","value":"2021-06-15"}],[{"type":"string","value":"Bob"},{"type":"int","value":25,"address":"0x559c1dc22c50"},{"type":"double","value":1.800000,"address":"0x559c1dc22cc0"},{"type":"bool","value":false},{"type":"date","value":"2022-01-10"}],[{"type":"string","value":"Charlie"},{"type":"int","value":40,"address":"0x559c1dc22ed0"},{"type":"double","value":1.750000,"address":"0x559c1dc22f40"},{"type":"bool","value":true},{"type":"date","value":"2020-09-05"}],[{"type":"string","value":"Diana"},{"type":"int","value":35,"address":"0x559c1dc23150"},{"type":"double","value":1.600000,"address":"0x559c1dc231c0"},{"type":"bool","value":false},{"type":"date","value":"2023-03-22"}]]}
```

### [examples/cbox_csv_demo1.c](examples/cbox_csv_demo1.c)

```bash
$ make
$ ./build/cbox_csv_demo1
CBoxCSV: 5 rows x 11 columns
Columns: seq, name/first, name/last, age, street, city, state, zip, dollar, pick(RED|BLUE|YELLOW|GREEN|WHITE), date
Row 0: 
 Col 0: CBox<int>: 1
 Col 1: CBox<string>: "Sallie"
 Col 2: CBox<string>: "Montgomery"
 Col 3: CBox<int>: 54
 Col 4: CBox<string>: "Wiok Loop"
 Col 5: CBox<string>: "Ritpeho"
 Col 6: CBox<string>: "AZ"
 Col 7: CBox<int>: 33871
 Col 8: CBox<currency>: CBox<decimal>: 4060.48
 CBox<string>: "USD"
 Col 9: CBox<string>: "GREEN"
 Col 10: CBox<date>: 1910-07-18

Row 1: 
 Col 0: CBox<int>: 2
 Col 1: CBox<string>: "Ella"
 Col 2: CBox<string>: "Padilla"
 Col 3: CBox<int>: 23
 Col 4: CBox<string>: "Himi Pike"
 Col 5: CBox<string>: "Wajohwac"
 Col 6: CBox<string>: "ID"
 Col 7: CBox<int>: 91024
 Col 8: CBox<currency>: CBox<decimal>: 120.17
 CBox<string>: "USD"
 Col 9: CBox<string>: "RED"
 Col 10: CBox<date>: 2014-02-10

Row 2: 
 Col 0: CBox<int>: 3
 Col 1: CBox<string>: "Rebecca"
 Col 2: CBox<string>: "Wilkins"
 Col 3: CBox<int>: 36
 Col 4: CBox<string>: "Tagijo Heights"
 Col 5: CBox<string>: "Ficrugur"
 Col 6: CBox<string>: "CA"
 Col 7: CBox<int>: 83410
 Col 8: CBox<currency>: CBox<decimal>: 6524.96
 CBox<string>: "USD"
 Col 9: CBox<string>: "WHITE"
 Col 10: CBox<date>: 1902-10-16

Row 3: 
 Col 0: CBox<int>: 4
 Col 1: CBox<string>: "Nettie"
 Col 2: CBox<string>: "Farmer"
 Col 3: CBox<int>: 63
 Col 4: CBox<string>: "Capva Square"
 Col 5: CBox<string>: "Masiive"
 Col 6: CBox<string>: "MD"
 Col 7: CBox<int>: 11605
 Col 8: CBox<currency>: CBox<decimal>: 7114.29
 CBox<string>: "USD"
 Col 9: CBox<string>: "YELLOW"
 Col 10: CBox<date>: 2023-08-11

Row 4: 
 Col 0: CBox<int>: 5
 Col 1: CBox<string>: "Cornelia"
 Col 2: CBox<string>: "Rodriquez"
 Col 3: CBox<int>: 18
 Col 4: CBox<string>: "Fetger Street"
 Col 5: CBox<string>: "Baibepic"
 Col 6: CBox<string>: "NC"
 Col 7: CBox<int>: 2907
 Col 8: CBox<currency>: CBox<decimal>: 8425.95
 CBox<string>: "USD"
 Col 9: CBox<string>: "GREEN"
 Col 10: CBox<date>: 1954-03-26

JSON:
{"type":"CBoxCSV","rows":[[{"type":"int","value":1,"address":"0x55e93e913be0"},{"type":"string","value":"Sallie"},{"type":"string","value":"Montgomery"},{"type":"int","value":54,"address":"0x55e93e913d30"},{"type":"string","value":"Wiok Loop"},{"type":"string","value":"Ritpeho"},{"type":"string","value":"AZ"},{"type":"int","value":33871,"address":"0x55e93e913ef0"},{"type":"currency","amount":{"type":"Decimal","value":"4060.48"},"code":{"type":"string","value":"USD"}},{"type":"string","value":"GREEN"},{"type":"date","value":"1910-07-18"}],[{"type":"int","value":2,"address":"0x55e93e914250"},{"type":"string","value":"Ella"},{"type":"string","value":"Padilla"},{"type":"int","value":23,"address":"0x55e93e9143a0"},{"type":"string","value":"Himi Pike"},{"type":"string","value":"Wajohwac"},{"type":"string","value":"ID"},{"type":"int","value":91024,"address":"0x55e93e914560"},{"type":"currency","amount":{"type":"Decimal","value":"120.17"},"code":{"type":"string","value":"USD"}},{"type":"string","value":"RED"},{"type":"date","value":"2014-02-10"}],[{"type":"int","value":3,"address":"0x55e93e9148c0"},{"type":"string","value":"Rebecca"},{"type":"string","value":"Wilkins"},{"type":"int","value":36,"address":"0x55e93e914a10"},{"type":"string","value":"Tagijo Heights"},{"type":"string","value":"Ficrugur"},{"type":"string","value":"CA"},{"type":"int","value":83410,"address":"0x55e93e914bd0"},{"type":"currency","amount":{"type":"Decimal","value":"6524.96"},"code":{"type":"string","value":"USD"}},{"type":"string","value":"WHITE"},{"type":"date","value":"1902-10-16"}],[{"type":"int","value":4,"address":"0x55e93e914f30"},{"type":"string","value":"Nettie"},{"type":"string","value":"Farmer"},{"type":"int","value":63,"address":"0x55e93e915080"},{"type":"string","value":"Capva Square"},{"type":"string","value":"Masiive"},{"type":"string","value":"MD"},{"type":"int","value":11605,"address":"0x55e93e915240"},{"type":"currency","amount":{"type":"Decimal","value":"7114.29"},"code":{"type":"string","value":"USD"}},{"type":"string","value":"YELLOW"},{"type":"date","value":"2023-08-11"}],[{"type":"int","value":5,"address":"0x55e93e9155a0"},{"type":"string","value":"Cornelia"},{"type":"string","value":"Rodriquez"},{"type":"int","value":18,"address":"0x55e93e9156f0"},{"type":"string","value":"Fetger Street"},{"type":"string","value":"Baibepic"},{"type":"string","value":"NC"},{"type":"int","value":2907,"address":"0x55e93e9158b0"},{"type":"currency","amount":{"type":"Decimal","value":"8425.95"},"code":{"type":"string","value":"USD"}},{"type":"string","value":"GREEN"},{"type":"date","value":"1954-03-26"}]]}
```

## Unit tests run

```bash
$ make test
```

## Contributing

Pull requests are welcome!

If you want to add new trait types, registry extensions, or serialization formats (XML, YAML), feel free to fork and submit proposals.

