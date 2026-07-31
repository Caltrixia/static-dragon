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
                CLI
                 │
                 ▼
          Project Loader
                 │
                 ▼
      Compilation Database
                 │
      (compile_commands.json)
                 │
                 ▼
          Clang Frontend
                 │
                 ▼
               AST
                 │
                 ▼
      Static Dragon Analyzers
        ├── Metrics
        ├── Complexity
        ├── Design Rules
        ├── Memory Rules
        ├── Call Graph
        ├── Dependencies
        └── Reports
