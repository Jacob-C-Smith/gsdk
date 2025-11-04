# GSDK

*A modular C SDK with tons of generic data structures, abstractions for performance computing, and highly flexable reflection.*

<p align="center">
    <img src="https://img.shields.io/badge/language-C-blue.svg" alt="Language: C">
    <img src="https://img.shields.io/badge/build-make-green.svg" alt="Build: make">
    <img src="https://img.shields.io/badge/license-MIT-lightgrey.svg" alt="License: MIT">
    <a href="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-build.yml"><img src="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-build.yml/badge.svg" alt="gsdk build" ></a>
</p>

> **Jump to:**  
> [Features](#features) • [Getting Started](#getting-started) • [Build](#build) • [Modules](#modules) • [Interfaces](#interfaces) • [Structure](#structure) • [Testing](#testing) • [License](#license)

---

## Features
- **Core**: Interfaces, logging, synchronization, binary packing, cryptography, sockets, and hashing
- **Data**: Arrays, maps, stacks, queues, sets, trees and more
- **Reflection**: json / base64 
- **Examples**: Example programs for each module
- **Tests**: Test suites for each modules
- **Documentation**: Detailed READMEs and extensive inline comments

## Getting started
The fastest way to get started is with GitHub Codespaces. You'll be able to view, modify, and run the examples without leaving your browser.

<a href="https://codespaces.new/Jacob-C-Smith/gsdk?quickstart=1"><img src="https://github.com/codespaces/badge.svg" alt="Open in GitHub Codespaces"></a>

## Build
To build the GSDK, run
```bash
$ make
```

## Modules

<b>key</b><br>
⭐ → **Standout**<br>
✅ → **Complete**<br>
✔️ → **Needs interfaces**<br>
🧪 → **Needs testing**<br>
🏗️ → **Work in progress**<br>
❓ → **Thinking about it**
<details open>
    <summary><a href="documentation/md/core.md"><b>core</b></a></summary>
    <ul>
        <li>⭐ <a href="./documentation/md/core/pack.md">pack</a></li>
        <li>✅ <a href="./documentation/md/core/hash.md">hash</a></li>
        <li>✅ <a href="./documentation/md/core/sha.md">sha</a></li>
        <li>✔️ <a href="./documentation/md/core/interfaces.md">interfaces</a></li>
        <li>✔️ <a href="./documentation/md/core/log.md">log</a></li>
        <li>🧪 <a href="./documentation/md/core/dsa.md">digital signature</a></li>
        <li>🧪 <a href="./documentation/md/core/rsa.md">rsa</a></li>
        <li>🧪 <a href="./documentation/md/core/sync.md">sync</a></li>
        <li>🏗️ <a href="./documentation/md/core/socket.md">socket</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/md/data.md"><b>data</b></a></summary>
    <ul>
        <li>✅ <a href="./documentation/md/data/array.md">array</a></li>
        <li>✅ <a href="./documentation/md/data/stack.md">stack</a></li>
        <li>✅ <a href="./documentation/md/data/tuple.md">tuple</a></li>
        <li>✔️ ⭐ <a href="./documentation/md/data/tree.md">binary</a></li>
        <li>✔️ <a href="./documentation/md/data/circular_buffer.md">circular buffer</a></li>
        <li>✔️ <a href="./documentation/md/data/dict.md">dict</a></li>
        <li>✔️ <a href="./documentation/md/data/queue.md">queue</a></li>
        <li>✔️ <a href="./documentation/md/data/set.md">set</a></li>
        <li>🧪 <a href="./documentation/md/data/bitmap.md">bitmap</a></li>
        <li>🧪 <a href="#">cache</a></li>
        <li>🧪 <a href="./documentation/md/data/double_queue.md">double ended queue</a></li>
        <li>🧪 <a href="./documentation/md/data/priority_queue.md">priority queue</a></li>
        <li>🏗️ <a href="./documentation/md/data/b.md">b</a></li>
        <li>❓ <a href="#">hash table</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/md/reflection.md"><b>reflection</b></a></summary>
    <ul>
        <li>⭐ <a href="./documentation/md/reflection/json.md">json</a></li>
        <li>✅ <a href="./documentation/md/reflection/base64.md">base64</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/md/performance.md"><b>performance</b></a></summary>
    <ul>
        <li>🧪 ⭐ <a href="./doocumentation/performance/parallel.md">parallel</a></li>
    </ul>
</details>

## Interfaces

<b>key</b><br>
✅ → Yes<br>
🏗️ → TODO<br>

<details>
    <summary>🏗️ <b>identity</b></summary>
    
|                   | `array`  | `b`   | `binary` | `cache` | `dict` | `graph` | `hash table` | `node` | `priority queue` | `red black` | `set` | `tree` |
|-------------------|----------|-------|----------|---------|--------|---------|--------------|--------|------------------|-------------|-------|--------|
| `equality`        | ✅      | ✅    | ✅       | ✅      |        |         | ✅           |        | ✅               | ✅         | ✅    | ✅     |
| `comparator`      | ✅      | ✅    | ✅       |         | ✅     |         |              |        | ✅               | ✅         | ✅    | ✅     |
| `key_accessor`    |         | ✅    | ✅       | ✅      | ✅     | ✅      | ✅           | ✅     | ✅               | ✅         | 🏗️    | ✅     |
</details>

<details>
    <summary>🏗️ <b>iterator</b></summary>

|                   | `array` | `bitmap` | `cache` | `circular buffer` | `dict` | `double queue` | `hash table` | `priority queue` | `queue` | `set` | `stack` | `tuple` |
|-------------------|---------|----------|---------|-------------------|--------|----------------|--------------|------------------|---------|-------|---------|---------|
| `fori`            | ✅      | 🏗️       | ✅      | 🏗️                | ✅     | ✅             |             | ✅               | ✅      |       | ✅      | ✅      |
| `foreach`         | ✅      |          | ✅      | 🏗️                | ✅     | ✅             | ✅          | ✅               | ✅      | ✅    | ✅      | ✅      |
| `map`             | ✅      |          | ✅      | 🏗️                | 🏗️     | 🏗️             | 🏗️          | 🏗️               | 🏗️      | 🏗️    | 🏗️      |         |

</details>

<details>
    <summary>🏗️ <b>reflection</b></summary>

|                   | `array` | `bitmap` | `cache` | `circular buffer` | `dict` | `double queue` | `graph` | `hash table` | `node` | `priority queue` | `queue` | `set` | `stack` | `tree` | `tuple` |
|-------------------|---------|----------|---------|-------------------|--------|----------------|---------|--------------|--------|------------------|---------|-------|---------|--------|---------|
| `pack`            | ✅      | ✅       | ✅      | 🏗️                | ✅     | 🏗️             | 🏗️      | 🏗️          | 🏗️     | 🏗️               | 🏗️      | 🏗️    | ✅      | 🏗️     | ✅      |
| `unpack`          | ✅      | ✅       | ✅      | 🏗️                | ✅     | 🏗️             | 🏗️      | 🏗️          | 🏗️     | 🏗️               | 🏗️      | 🏗️    | ✅      | 🏗️     | ✅      | 
| `hash64`          | ✅      | 🏗️       | 🏗️      | 🏗️                | 🏗️     | 🏗️             | 🏗️      | 🏗️           | 🏗️     | 🏗️               | 🏗️      | 🏗️   | ✅      | 🏗️     | ✅      |
</details>

<details>
    <summary>🏗️ <b>tree</b></summary>

|                   | `b` | `binary` |
|-------------------|-----|----------|
| `search`          | 🏗️  | ✅       |
| `insert`          | 🏗️  | ✅       |
| `remove`          | 🏗️  | ✅       |
| `pre-order`       | 🏗️  | ✅       |
| `in-order`        | 🏗️  | ✅       |
| `post-order`      | 🏗️  | ✅       |

</details>

## Examples
<b>key</b><br>
✅ → Complete<br>
✔️ → Incomplete<br>
❓ → Not implemented<br>

## Core
| example           | status |
|-------------------|--------|
| hash              | ✅     |
| pack              | ✔️     |
| sha               | ✅     |
| interfaces        | ❓     |
| log               | ✅     | 
| digital signature | ✔️     |
| rsa               | ✔️     |
| sync              | ✔️     |
| socket            | ❓     |

### Data
| example            | status |
|--------------------|--------|
| array              | ✅     |
| cache              | ✅     |
| stack              | ✅     |
| tuple              | ✅     |
| binary             | ✅     |
| circular buffer    | ✔️     |
| dict               | ✔️     |
| queue              | ✔️     |
| set                | ✔️     |
| bitmap             | ✔️     |
| double ended queue | ✔️     |
| priority queue     | ✔️     |
| hash table         | ✔️     |


### Reflection
| example            | status |
|--------------------|--------|
| base64             | ✅     |
| json               | ✅     |

### Performance
| example            | status |
|--------------------|--------|
| parallel           | ✅     |



## Structure

| Folder                      | Description                        |
| --------------------------- | ---------------------------------- |
| [include](./include/)       | Interfaces for modules             |
| [src](./src)                | Implementation of modules          |
| [lib](./build/lib/)         | Shared objects                     |
| [examples](./src/examples/) | Examples of module usage           |
| [tests](./src/test/)        | Unit tests                         |
| [resources](./resources/)   | Sample data for examples           |

## Testing
To build the tests, run
```bash
$ make tests
```
## License

See [LICENSE](./LICENSE) for details.

### Made with ❤️ for 👥 across 🌎🌍🌏
 