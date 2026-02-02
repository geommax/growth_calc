Growth Calc Build & Packaging Guide

This guide provides clear, numbered steps to build with CMake, install on Ubuntu, produce debug and release (stripped) binaries, and create a minimal `.deb` package. It applies to both the original app in [CMakeLists.txt](CMakeLists.txt) (`growth_calc`) and the modular app in [new_release/CMakeLists.txt](new_release/CMakeLists.txt) (`growth_calc_modular`).

## 1) CMake Workflow

### A. Original app (`growth_calc`)
1. Create build directory:
```bash
mkdir -p build
```
2. Configure the project:
```bash
cd build
cmake ..
```
3. Build the binary:
```bash
cmake --build . -- -j
```
4. Run a quick test:
```bash
./growth_calc 2 5
```

### B. Modular app (`growth_calc_modular`)
1. Create build directory:
```bash
mkdir -p new_release/build
```
2. Configure the project:
```bash
cd new_release/build
cmake ..
```
3. Build the binary:
```bash
cmake --build . -- -j
```
4. Run with repo config:
```bash
./growth_calc_modular --config ../config.txt
```
5. Verify results:
   - Console shows linear and exponential steps with 1s delays.
   - If logging is enabled, entries append to [logs/growth_calc.log](../logs/growth_calc.log).

## 2) Install to Ubuntu

### A. Original app install target
1. From the root build directory:
```bash
cd build
sudo cmake --install .
```
2. Verify installation:
```bash
which growth_calc
growth_calc 2 5
```

### B. Modular app installation
Option 1 — Manual install:
1. Copy the built binary:
```bash
sudo install -m 0755 new_release/build/growth_calc_modular /usr/local/bin/growth_calc_modular
```
2. Verify:
```bash
which growth_calc_modular
growth_calc_modular --config /path/to/config.txt
```

Option 2 — CMake install rule:
1. Add to [new_release/CMakeLists.txt](../new_release/CMakeLists.txt):
   `install(TARGETS growth_calc_modular DESTINATION /usr/local/bin)`
2. Install via CMake:
```bash
sudo cmake --install new_release/build
```

## 3) Debugging Version (gdb)
1. Create a Debug build:
```bash
mkdir -p new_release/build-debug
cd new_release/build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j
```
2. Confirm debug info:
```bash
file growth_calc_modular   # expect: not stripped, with debug info
```
3. Run with gdb:
```bash
gdb -q ./growth_calc_modular --args --config ../config.txt
(gdb) break WorkerThread::runExponential
(gdb) run
(gdb) bt
(gdb) continue
```
4. Smoke test without gdb:
```bash
./growth_calc_modular --config ../config.txt
```

## 4) Release Version (stripped)
1. Create a Release build:
```bash
mkdir -p new_release/build-release
cd new_release/build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -- -j
```
2. Strip symbols (optional, smaller size):
```bash
strip -s growth_calc_modular
```
3. Verify and run:
```bash
file growth_calc_modular   # expect: stripped
./growth_calc_modular --config ../config.txt
```

## 5) Create Ubuntu .deb Package (dpkg)
1. Prepare package layout:
```bash
cd new_release/build-release
mkdir -p pkg/usr/local/bin pkg/DEBIAN
cp growth_calc_modular pkg/usr/local/bin/
```
2. Write control file:
```bash
cat > pkg/DEBIAN/control << 'EOF'
Package: growth-calc-modular
Version: 1.0.0
Section: utils
Priority: optional
Architecture: amd64
Maintainer: Your Name <you@example.com>
Description: Growth Calculator (modular) with linear/exponential steps and logging
EOF
```
3. Build the package:
```bash
dpkg-deb --build pkg growth-calc-modular_1.0.0_amd64.deb
```
4. Install the package:
```bash
sudo apt install ./growth-calc-modular_1.0.0_amd64.deb
```
5. Validate installation:
```bash
which growth_calc_modular
growth_calc_modular --config /usr/local/etc/growth_calc_config.txt || growth_calc_modular --config /path/to/your/config.txt
```
6. Optional checks:
```bash
dpkg -L growth-calc-modular
sudo apt remove growth-calc-modular -y
```

## Notes & Tips
- Log file path in [config.txt](../config.txt) is relative; adjust if running from different directories.
- Use `-DCMAKE_BUILD_TYPE=RelWithDebInfo` to keep some debug info in optimized builds.
- To package the original app similarly, replace paths/binary names with `growth_calc` and build from the repo root `build` directory.

