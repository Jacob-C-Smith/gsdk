# GSDK

*A modular C SDK for high-performance computing, data structures, and reflection utilities.*

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
- **Core**: Logging, synchronization, sockets, hashing, and caching
- **Data**: Arrays, maps, stacks, queues, sets, trees, graphs, and more
- **Reflection**: JSON, base64, HTTP, and raw binart packing
- **Examples**: Example programs for each component
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
        <li>✔️ <a href="./documentation/core/sync.md">sync</a></li>
        <li>🧪 <a href="./documentation/core/pack.md">pack</a></li>
        <li>🏗️ <a href="./documentation/core/crypto.md">crypto</a></li>
        <li>🏗️ <a href="./documentation/core/socket.md">socket</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/data.md">data</a></summary>
    <ul>
        <li>✔️ <a href="./src/data/array/README.md">array</a></li>
        <li>✔️ <a href="./src/data/circular_buffer/README.md">circular buffer</a></li>
        <li>✔️ <a href="./src/data/dict/README.md">dict</a></li>
        <li>✔️ <a href="./src/data/queue/README.md">queue</a></li>
        <li>✔️ <a href="./src/data/set/README.md">set</a></li>
        <li>✔️ <a href="./src/data/stack/README.md">stack</a></li>
        <li>✔️ <a href="./src/data/tuple/README.md">tuple</a></li>
        <li>🧪 <a href="./src/data/bitmap/README.md">bitmap</a></li>
        <li>🧪 <a href="./src/data/double_queue/README.md">double ended queue</a></li>
        <li>🧪 <a href="./src/data/node/README.md">node</a></li>
        <li>🧪 <a href="./src/data/priority_queue/README.md">priority queue</a></li>
        <li>🏗️ <a href="./src/data/graph/README.md">graph</a></li>
        <li>🏗️ <a href="./src/data/table/README.md">table</a></li>
        <li>🏗️ <a href="./src/data/tree/README.md">tree</a></li>
        <li>❓ <a href="#">cache</a></li>
        <li>❓ <a href="#">hash table</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/reflection.md">reflection</a></summary>
    <ul>
        <li>✔️ <a href="./src/reflection/base64/README.md">base64</a></li>
        <li>✔️ <a href="./src/reflection/json/README.md">json</a></li>
        <li>🏗️ <a href="./src/reflection/http/README.md">http</a></li>
        <li>❓ <a href="./src/reflection/csv/README.md">csv</a></li>
    </ul>
</details>

<details open>
    <summary><a href="documentation/performance.md">performance</a></summary>
    <ul>
        <li>🧪 <a href="./src/performance/parallel/README.md">parallel</a></li>
        <li>🏗️ <a href="./src/lang/code_gen/README.md">code gen</a></li>
        <li>❓ <a href="./src/performance/allocator/README.md">allocator</a></li>
        <li>❓ <a href="./src/performance/distribute/README.md">distribute</a></li>
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
