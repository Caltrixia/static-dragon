# Static Dragon
- A tool to analyze your C/C++ code

## 1. Setup environment and build tool

### 1. install clang 
chmod +x setup.sh
setup.sh

- If you encouter this error /bin/sh^M: bad interpreter, you need to change from CRLF -> LF
dos2unix setup.sh

### 2. configure project with cmakelists.txt
chmod +x configure.sh
./configure.sh

### 3. build project
chmod +x build.sh
./build.sh


## 2. Test with simple project
cmake -S test -B test/build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
./build/static-dragon -p test/build test/*.cpp


## Architecture

```text
                +----------------+
                |   C++ Source   |
                +----------------+
                        |
                        v
              +-------------------+
              |   Clang LibTooling|
              +-------------------+
                        |
                        v
              +-------------------+
              |   AST Traversal   |
              | (RecursiveVisitor)|
              +-------------------+
                        |
                        v
              +-------------------+
              | Metric Collection |
              +-------------------+
                        |
                        v
              +-------------------+
              | Report Generator  |
              +-------------------+
                        |
        +---------------+---------------+
        |       |        |       |      |
        v       v        v       v      v
    Terminal   TXT      CSV    JSON   HTML
```

## Components

- **Clang LibTooling** – Parses C++ source code and builds the AST.
- **AST Traversal** – Visits functions and extracts information.
- **Metric Collection** – Computes software metrics for each function.
- **Report Generator** – Exports results to multiple report formats.

## Workflow

```text
Source Code
    ↓
Parse AST
    ↓
Analyze Functions
    ↓
Compute Metrics
    ↓
Generate Reports
```