# gsdk

*A modular C SDK with tons of generic data structures, abstractions for high-performance computing, and highly flexible reflection.*

<p align="center">
    <img src="https://img.shields.io/badge/language-C-blue.svg" alt="Language: C">
    <img src="https://img.shields.io/badge/build-make-green.svg" alt="Build: make">
    <img src="https://img.shields.io/badge/license-MIT-lightgrey.svg" alt="License: MIT">
    <br>
    <a href="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-build.yml"><img src="https://github.com/Jacob-C-Smith/gsdk/actions/workflows/gsdk-build.yml/badge.svg" alt="gsdk build" ></a>
</p>

> **Jump to:**  
> [Features](#features) • [Getting Started](#getting-started) • [Modules](#modules) • [Structure](#structure) • [Build](#build) • [Examples](#examples) • [Tests](#tests) • [License](#license)

---

## Features
- **Core**: Interfaces, logging, synchronization, binary packing, cryptography, sockets, and hashing
- **Data**: Arrays, maps, stacks, queues, sets, trees, and more
- **Reflection**: JSON and base64 
- **Examples**: Example programs for each module
- **Tests**: Test suites for many modules
- **Documentation**: Detailed READMEs and extensive inline comments

## Getting started
The fastest way to get started is with GitHub Codespaces. You'll be able to view, modify, and run the code without leaving your browser.

<a href="https://codespaces.new/Jacob-C-Smith/gsdk?quickstart=1"><img src="https://github.com/codespaces/badge.svg" alt="Open in GitHub Codespaces"></a>

## Modules

<b>key</b><br>
⭐ → <b>1.0 relese</b><br>
✔️ → <i>Needs interfaces</i><br>
🧪 → <i>Needs testing</i><br>
🏗️ → <i>Work in progress</i><br>
❓ → <i>Thinking about it</i>
<details open>
    <summary><a href="documentation/md/core.md">core</a></summary>
    <ul>
        <li>⭐ <a href="./documentation/md/core/hash.md"><b>
            hash
        </b></a></li>
        <li>⭐ <a href="./documentation/md/core/pack.md"><b>
            pack
        </b></a></li>
        <li>⭐ <a href="./documentation/md/core/sha.md"><b>
            sha
        </b></a></li>
        <li>⭐ <a href="./documentation/md/core/sync.md"><b>
            sync
        </b></a></li>
        <li>⭐ <a href="./documentation/md/core/log.md"><b>
            log
        </b></a></li>
        <li>⭐ <a href="./documentation/md/core/interfaces.md"><b>
            interfaces
        </b></a></li>
        <li>🧪 <a href="./documentation/md/core/digital_signature.md">
            digital signatures
        </a></li>
        <li>🧪 <a href="./documentation/md/core/rsa.md">
            rsa
        </a></li>
        <!--
        <li>🏗️ <a href="./documentation/md/core/aes.md">
            aes
        </a></li>
        <li>🏗️ <a href="./documentation/md/core/socket.md">
            socket
        </a></li>
        -->
    </ul>
</details>

<details open>
    <summary><a href="documentation/md/data.md">data</a></summary>
    <ul>
        <li>⭐ <a href="./documentation/md/data/array.md">
            <b>array</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/bitmap.md">
            <b>bitmap</b>
        </a></li>
        <li>⭐ <a href="#">
            <b>cache</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/circular_buffer.md">
            <b>circular buffer</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/dict.md">
            <b>dict</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/double_queue.md">
            <b>double ended queue</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/queue.md">
            <b>queue</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/set.md">
            <b>set</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/stack.md">
            <b>stack</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/tuple.md">
            <b>tuple</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/data/priority_queue.md">
            <b>priority queue</b>
        </a></li>
        <li>✔️ <a href="./documentation/md/data/tree.md">binary</a></li>
        <li>🏗️ <a href="#">hash table</a></li>
        <!--<li>🧪 <a href="./documentation/md/data/node.md">node</a></li>-->
        <!--<li>❓ <a href="#">adjacency list</a></li>-->
        <!--<li>❓ <a href="#">adjacency matrix</a></li>-->
        <!--<li>❓ <a href="./documentation/md/data/graph.md">graph</a></li>-->
        <!--<li>❓ <a href="#">edge list</a></li>-->
        <!--<li>❓ <a href="#">avl</a></li>-->
        <!--<li>❓ <a href="#">red black</a></li>-->
    </ul>
</details>

<details open>
    <summary><a href="documentation/md/reflection.md">reflection</a></summary>
    <ul>
        <li>⭐ <a href="./documentation/md/reflection/base64.md">
            <b>base64</b>
        </a></li>
        <li>⭐ <a href="./documentation/md/reflection/json.md">
            <b>json</b>
        </a></li>
        <!--<li>🏗️ <a href="./documentation/md/reflection/http.md">http</a></li>-->
    </ul>
</details>

<details open>
    <summary><a href="documentation/md/performance.md">performance</a></summary>
    <ul>
        <li>🧪 <a href="./doocumentation/performance/parallel.md">parallel</a></li>
        <!--<li>🏗️ <a href="./doocumentation/performance/distribute.md">distribute</a></li>-->
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
To build gsdk, run
```bash
$ make
```

## Examples
To build the examples, run
```bash
$ make examples
```

## Tests
To build the tests, run
```bash
$ make tests
```

## License
See [LICENSE](./LICENSE) for details.

### Made with ❤️ for 👥 across 🌎🌍🌏
 