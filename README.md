# GSDK

*A modular C SDK with tons of generic data structures, abstractions for high-performance computing, and highly flexable reflection.*

<p align="center">
    <img src="https://img.shields.io/badge/language-C-blue.svg" alt="Language: C">
    <img src="https://img.shields.io/badge/build-make-green.svg" alt="Build: make">
    <img src="https://img.shields.io/badge/license-MIT-lightgrey.svg" alt="License: MIT">
    <br>
    <a href="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-build.yml"><img src="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-build.yml/badge.svg" alt="gsdk build" ></a>
    <a href="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-examples.yml"><img src="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-examples.yml/badge.svg" alt="gsdk examples" ></a>
    <a href="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-tests.yml"><img src="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-tests.yml/badge.svg" alt="gsdk tests" ></a>
</p>

> **Jump to:**  
> [Features](#features) • [Getting Started](#getting-started) • [Highlights](#highlights) • [Modules](#modules) • [Structure](#structure) • [Build](#build) • [Testing](#testing) • [License](#license)

---

## Features
- **Core**: Interfaces, logging, synchronization, binary packing, cryptography, sockets, and hashing
- **Data**: Arrays, maps, stacks, queues, sets, trees, graphs, and more
- **Reflection**: JSON and base64 
- **Examples**: Example programs for each module
- **Tests**: Test suites for many modules
- **Documentation**: Detailed READMEs and extensive inline comments

## Getting started
The fastest way to get started is with GitHub Codespaces. You'll be able to view, modify, and run the code without leaving your browser.

<a href="https://codespaces.new/Jacob-C-Smith/sync?quickstart=1"><img src="https://github.com/codespaces/badge.svg" alt="Open in GitHub Codespaces"></a>

## Modules

<b>key</b><br>
✔️ → Complete<br>
🧪 → Needs testing<br>
🏗️ → Work in progress<br>
❓ → Thinking about it
<details open>
    <summary><a href="documentation/core.md">core</a></summary>
    <ul>
        <li>✔️ <a href="./documentation/core/hash.md">hash</a></li>
        <li>✔️ <a href="./documentation/core/interfaces.md">interfaces</a></li>
        <li>✔️ <a href="./documentation/core/log.md">log</a></li>
        <li>✔️ <a href="./documentation/core/pack.md">pack</a></li>
        <li>✔️ <a href="./documentation/core/sync.md">sync</a></li>
        <li>🏗️ <a href="./documentation/core/crypto.md">crypto</a></li>
        <li>🏗️ <a href="./documentation/core/socket.md">socket</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/data.md">data</a></summary>
    <ul>
        <li>✔️ <a href="./documentation/data/array.md">array</a></li>
        <li>✔️ <a href="./documentation/data/circular_buffer.md">circular buffer</a></li>
        <li>✔️ <a href="./documentation/data/dict.md">dict</a></li>
        <li>✔️ <a href="./documentation/data/queue.md">queue</a></li>
        <li>✔️ <a href="./documentation/data/set.md">set</a></li>
        <li>✔️ <a href="./documentation/data/stack.md">stack</a></li>
        <li>✔️ <a href="./documentation/data/tuple.md">tuple</a></li>
        <li>🧪 <a href="./documentation/data/bitmap.md">bitmap</a></li>
        <li>🧪 <a href="#">cache</a></li>
        <li>🧪 <a href="./documentation/data/double_queue.md">double ended queue</a></li>
        <li>🧪 <a href="./documentation/data/node.md">node</a></li>
        <li>🧪 <a href="./documentation/data/priority_queue.md">priority queue</a></li>
        <li>🏗️ <a href="./documentation/data/graph.md">graph</a></li>
        <li>🏗️ <a href="./documentation/data/tree.md">tree</a></li>
        <li>❓ <a href="#">hash table</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/reflection.md">reflection</a></summary>
    <ul>
        <li>✔️ <a href="./documentation/reflection/base64.md">base64</a></li>
        <li>✔️ <a href="./documentation/reflection/json.md">json</a></li>
        <li>🏗️ <a href="./documentation/reflection/http.md">http</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/performance.md">performance</a></summary>
    <ul>
        <li>🧪 <a href="./doocumentation/performance/parallel.md">parallel</a></li>
        <li>🏗️ <a href="./doocumentation/performance/code_gen.md">code gen</a></li>
        <li>❓ <a href="./doocumentation/performance/allocator.md">allocator</a></li>
        <li>❓ <a href="./doocumentation/performance/distribute.md">distribute</a></li>
    </ul>
</details>

## Structure

| Folder                      | Description                        |
| --------------------------- | ---------------------------------- |
| [include](./include/)       | Interfaces for modules             |
| [src](./src)                | Implementation of modules          |
| [lib](./build/lib/)         | Shared objects                     |
| [examples](./src/examples/) | Examples of module usage           |
| [tests](./src/test/)        | Unit tests                         |
| [resources](./resources/)   | Sample data for examples           |

## Build
To build the GSDK, run
```bash
$ make
```

## Testing
To build the tests, run
```bash
$ make tests
```
## License

See [LICENSE](./LICENSE) for details.
