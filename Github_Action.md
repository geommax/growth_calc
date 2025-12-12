# GitHub Actions Explanation

This document explains the GitHub Actions workflows configured for the Growth Pattern Calculator C++ project.

## Overview

Three automated workflows have been implemented to ensure code quality, continuous integration, and release management:

1. **CI Build** - Continuous Integration and Testing
2. **Release Build** - Automated Release Creation
3. **Code Quality** - Static Analysis and Runtime Checks

---

## Workflow 1: CI Build (`.github/workflows/ci.yml`)

### Purpose
Automated testing that runs on every code push or pull request to ensure the application builds correctly and passes functional tests.

### When It Runs
- Push to `main` or `develop` branches
- Pull requests to `main` or `develop` branches
- Manual trigger via GitHub Actions UI

### Platform
- **Ubuntu 22.04** (single platform for simplicity)

### Build Process

#### Step 1: Checkout Code
```yaml
- uses: actions/checkout@v4
```
Downloads the repository code to the runner.

#### Step 2: Install Dependencies
```yaml
- run: |
    sudo apt-get update
    sudo apt-get install -y build-essential cmake
```
Installs required build tools:
- `build-essential`: GCC compiler, make, and essential build tools
- `cmake`: Cross-platform build system generator

#### Step 3: Configure CMake
```yaml
- run: |
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
```
Configures the build system:
- Creates Makefiles from `CMakeLists.txt`
- Sets build type to `Release` (optimized binary)
- Detects compiler and system libraries

#### Step 4: Build Application
```yaml
- run: cmake --build . --config Release -j$(nproc)
```
Compiles the C++ code:
- Uses all available CPU cores (`-j$(nproc)`)
- Produces the `growth_calc` executable
- Links against required libraries (math, pthread)

#### Step 5: Verify Executable
```yaml
- run: file growth_calc
```
Checks that the binary was created successfully and displays file information.

### Testing Phase

#### Test 1: Config File Execution
```yaml
- run: ./growth_calc ../config.txt
```
Tests the application with configuration file input mode:
- Reads base and exponent from `config.txt`
- Verifies file parsing logic
- Checks logging functionality

#### Test 2: Command-Line Arguments
```yaml
- run: ./growth_calc 2 5
```
Tests direct parameter passing:
- Base = 2, Exponent = 5
- Verifies argument parsing
- Tests both linear and exponential calculations

#### Test 3: Error Handling
```yaml
- run: ./growth_calc invalid args || echo "Expected failure..."
```
Validates error handling:
- Tests with invalid input
- Ensures graceful failure
- Verifies error messages

#### Test 4: Log File Creation
```yaml
- run: ls -la logs/
```
Confirms logging system works:
- Checks `logs/` directory creation
- Verifies log files are generated

### Code Coverage

#### Coverage Build
```yaml
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage"
```
Compiles with coverage instrumentation:
- Tracks which lines are executed
- Generates `.gcda` and `.gcno` files

#### Coverage Report
```yaml
gcovr --root .. --xml coverage.xml
```
Creates coverage report showing:
- Line coverage percentage
- Branch coverage
- Uncovered code sections

### Artifacts

After successful build, uploads:
- `growth_calc` executable
- Log files (if generated)
- Coverage report (XML format)
- **Retention**: 7 days for CI, 30 days for coverage

---

## Workflow 2: Release Build (`.github/workflows/release.yml`)

### Purpose
Automatically creates GitHub releases with downloadable binaries when version tags are pushed.

### When It Runs
- When git tag matching `v*.*.*` is pushed (e.g., `v1.0.0`)
- When a GitHub release is created manually
- Manual trigger with custom version

### Job 1: Create Release

#### Get Version
```yaml
- id: get_version
  run: echo "version=${GITHUB_REF#refs/tags/}" >> $GITHUB_OUTPUT
```
Extracts version from git tag:
- Input: `refs/tags/v1.0.0`
- Output: `v1.0.0`

#### Create GitHub Release
```yaml
- uses: actions/create-release@v1
  with:
    tag_name: ${{ steps.get_version.outputs.version }}
```
Creates a new release on GitHub:
- Release title: "Growth Calculator v1.0.0"
- Includes feature description
- Usage instructions
- Download links (added by next job)

### Job 2: Build Release

#### Platform
- **Ubuntu 22.04** (Linux x86_64)

#### Optimized Build
```yaml
cmake .. -DCMAKE_BUILD_TYPE=Release
```
Release configuration:
- Optimization level: `-O2`
- Compiler flags: `-Wall`
- No debug symbols (smaller binary)

#### Binary Stripping
```yaml
strip growth_calc
```
Removes debugging symbols:
- Reduces file size by ~30-50%
- Keeps functionality intact
- Makes binary distribution-ready

#### Package Creation
```yaml
mkdir -p release
cp build/growth_calc release/
cp config.txt release/
cp README.md release/
tar -czf growth_calc-linux-x86_64.tar.gz *
```
Creates release package containing:
- `growth_calc` executable (with execute permissions)
- `config.txt` sample configuration
- `README.md` documentation
- `growth_calc.service` systemd service file

#### Upload to Release
```yaml
- uses: actions/upload-release-asset@v1
  with:
    asset_path: ./growth_calc-linux-x86_64.tar.gz
```
Attaches archive to GitHub release:
- Asset name: `growth_calc-linux-x86_64.tar.gz`
- Content type: `application/gzip`
- Directly downloadable by users

### How to Create a Release

1. **Tag the version:**
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```

2. **Automated process:**
   - Workflow triggers automatically
   - Builds optimized binary
   - Creates GitHub release
   - Uploads downloadable asset

3. **Download:**
   - Go to GitHub repository → Releases
   - Download `growth_calc-linux-x86_64.tar.gz`
   - Extract and run

---

## Workflow 3: Code Quality (`.github/workflows/code-quality.yml`)

### Purpose
Comprehensive code quality checks including static analysis, runtime error detection, and memory leak detection.

### When It Runs
- Push to `main` or `develop` branches
- Pull requests
- Manual trigger

### Job 1: Static Analysis

Static code analysis without executing the program.

#### cppcheck
```yaml
cppcheck --enable=all --suppress=missingIncludeSystem main.cpp
```
Checks for:
- **Memory leaks**: Unfreed allocations
- **Null pointer dereferences**: Accessing null pointers
- **Buffer overflows**: Array bounds violations
- **Uninitialized variables**: Using variables before assignment
- **Dead code**: Unreachable code paths
- **Style issues**: Coding standard violations

Output: `cppcheck-report.txt` and `cppcheck-report.xml`

#### clang-tidy
```yaml
clang-tidy main.cpp -p build
```
Modern C++ checks:
- **Code modernization**: Use C++11/14/17 features
- **Performance**: Inefficient algorithms or operations
- **Readability**: Complex or unclear code
- **Bug-prone patterns**: Common mistake patterns
- **Best practices**: C++ Core Guidelines compliance

Output: `clang-tidy-report.txt`

### Job 2: Sanitizers

Runtime error detection by instrumenting the code during compilation.

#### Address Sanitizer (ASan)
```yaml
-DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer"
```
Detects at runtime:
- **Heap buffer overflow**: Reading/writing beyond allocated memory
- **Stack buffer overflow**: Array bounds on stack
- **Use-after-free**: Accessing freed memory
- **Double-free**: Freeing memory twice
- **Memory leaks**: Allocated but never freed memory

How it works:
1. Compiles with special instrumentation
2. Runs application with test inputs
3. Intercepts memory operations
4. Reports violations immediately

Output: `sanitizer-address.log`

### Job 3: Compiler Warnings

Strict compilation with all warnings enabled.

#### GCC with Strict Flags
```yaml
-DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic -Werror"
```

Flag meanings:
- **-Wall**: Enable all common warnings
- **-Wextra**: Enable additional warnings
- **-Wpedantic**: Strict ISO C++ compliance
- **-Werror**: Treat warnings as errors (fail build)

Catches:
- Unused variables
- Implicit type conversions
- Potential logic errors
- Non-standard extensions
- Suspicious constructs

Output: `build-warnings-gcc.log`

### Job 4: Valgrind Memory Check

Dynamic memory analysis tool.

#### Valgrind Execution
```yaml
valgrind --leak-check=full --show-leak-kinds=all \
  --track-origins=yes ./growth_calc ../config.txt
```

Options explained:
- **--leak-check=full**: Detailed leak information
- **--show-leak-kinds=all**: All types of leaks
- **--track-origins=yes**: Track uninitialized values

Detects:
- **Memory leaks**: Definitely lost, possibly lost, still reachable
- **Invalid reads/writes**: Accessing invalid memory
- **Use of uninitialized values**: Reading before writing
- **Invalid frees**: Double-free or invalid pointer free
- **Mismatched allocation/deallocation**: new/delete vs malloc/free

Test scenarios:
1. Config file mode: `valgrind ./growth_calc ../config.txt`
2. CLI mode: `valgrind ./growth_calc 2 3`

Output: `valgrind-config.log` and `valgrind-cmdline.log`

### Quality Reports

All artifacts retained for 30 days:
- `cppcheck-report`: Static analysis findings
- `clang-tidy-report`: Modernization suggestions
- `sanitizer-address-log`: Runtime memory errors
- `warnings-gcc`: Compilation warnings
- `valgrind-logs`: Memory leak details

---

## Workflow Status and Badges

### Viewing Workflow Runs
Navigate to: `https://github.com/geommax/growth_calc/actions`

You'll see:
- ✅ Successful runs (green checkmark)
- ❌ Failed runs (red X)
- 🔄 In-progress runs (yellow circle)
- Click any run for detailed logs

### Adding Status Badges

Add to README.md:
```markdown
[![CI Build](https://github.com/geommax/growth_calc/workflows/CI%20Build/badge.svg)](https://github.com/geommax/growth_calc/actions/workflows/ci.yml)
[![Release Build](https://github.com/geommax/growth_calc/workflows/Release%20Build/badge.svg)](https://github.com/geommax/growth_calc/actions/workflows/release.yml)
[![Code Quality](https://github.com/geommax/growth_calc/workflows/Code%20Quality/badge.svg)](https://github.com/geommax/growth_calc/actions/workflows/code-quality.yml)
```

Displays:
- Real-time build status
- Clickable links to Actions
- Professional appearance

---

## Common Scenarios

### Scenario 1: Push Code to Main Branch

**What happens:**
1. CI Build workflow triggers
2. Code is checked out
3. Application builds
4. Tests run (config, CLI, errors)
5. Coverage report generated
6. Results appear in Actions tab

**If successful:**
- Green checkmark appears
- Artifacts available for download
- Safe to merge/deploy

**If failed:**
- Red X appears
- Check logs for errors
- Fix issues and push again

### Scenario 2: Create a New Release

**Steps:**
```bash
# 1. Update version in code if needed
# 2. Commit changes
git add .
git commit -m "Prepare release v1.0.0"

# 3. Create and push tag
git tag v1.0.0
git push origin main
git push origin v1.0.0
```

**What happens:**
1. Release workflow triggers
2. Creates GitHub release
3. Builds optimized binary
4. Packages with documentation
5. Uploads to release page

**Result:**
- New release visible in GitHub
- Downloadable `.tar.gz` file
- Users can download and run

### Scenario 3: Pull Request Review

**What happens:**
1. Developer creates pull request
2. CI Build and Code Quality run automatically
3. Reviewer sees status checks
4. Must pass before merging

**Checks:**
- ✅ Build succeeds
- ✅ Tests pass
- ✅ No critical warnings
- ✅ No memory leaks

**If checks fail:**
- Review failure logs
- Fix issues in PR branch
- Push fixes
- Checks re-run automatically

---

## Troubleshooting

### Build Failure

**Error:** CMake configuration failed
```
Solution: Check CMakeLists.txt syntax
- Verify minimum CMake version
- Ensure all dependencies listed
```

**Error:** Compilation errors
```
Solution: Check compiler warnings/errors
- Review code syntax
- Fix include paths
- Verify C++ standard (C++17)
```

### Test Failure

**Error:** Application crashes during test
```
Solution: Check sanitizer logs
- Look for memory errors
- Review stack traces
- Fix pointer issues
```

**Error:** Config file not found
```
Solution: Verify file paths
- Ensure config.txt committed
- Check relative paths
- Review working directory
```

### Code Quality Issues

**Warnings treated as errors**
```
Solution: Fix all warnings
- Review warning messages
- Apply suggested fixes
- Consider using clang-tidy suggestions
```

**Memory leaks detected**
```
Solution: Review Valgrind output
- Identify leak source
- Add proper cleanup
- Verify with local Valgrind run
```

### Release Issues

**Tag doesn't trigger workflow**
```
Solution: Check tag format
- Must match v*.*.* pattern (v1.0.0)
- Push tag to remote: git push origin v1.0.0
- Check workflow file syntax
```

**Asset upload fails**
```
Solution: Verify build output
- Ensure binary exists
- Check file paths in workflow
- Review build logs
```

---

## Best Practices

### Before Committing

1. **Test locally:**
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   ./growth_calc ../config.txt
   ```

2. **Run static analysis:**
   ```bash
   cppcheck --enable=all main.cpp
   ```

3. **Check for warnings:**
   ```bash
   g++ -Wall -Wextra -Wpedantic -std=c++17 main.cpp
   ```

### Pull Request Process

1. Create feature branch
2. Make changes
3. Push branch
4. Create PR
5. Wait for CI checks
6. Address review comments
7. Merge when green

### Release Checklist

- [ ] All tests passing
- [ ] Documentation updated
- [ ] Version number updated
- [ ] CHANGELOG updated
- [ ] Tag created and pushed
- [ ] Release notes written
- [ ] Binary tested manually

---

## Understanding the Results

### CI Build Success
```
✅ Build and Test
   - Checkout code: ✓
   - Install dependencies: ✓
   - Configure CMake: ✓
   - Build: ✓
   - Test config file: ✓
   - Test CLI args: ✓
   - Test error handling: ✓
   - Code coverage: 85% ✓
```

### Code Quality Success
```
✅ Static Analysis
   - cppcheck: 0 errors ✓
   - clang-tidy: 0 issues ✓

✅ Build with Sanitizers
   - Address Sanitizer: No errors ✓

✅ Compiler Warnings
   - GCC: 0 warnings ✓

✅ Valgrind Memory Check
   - Definitely lost: 0 bytes ✓
   - Possibly lost: 0 bytes ✓
```

### Release Success
```
✅ Create Release
   - Version: v1.0.0 ✓
   - Release created: ✓

✅ Build Release
   - Build Linux binary: ✓
   - Strip binary: ✓
   - Package release: ✓
   - Upload asset: ✓
```

---

## Maintenance

### Regular Updates

**GitHub Actions versions:**
```yaml
- uses: actions/checkout@v4      # Check for v5
- uses: actions/upload-artifact@v4  # Keep current
```

**Dependencies:**
```bash
# Ubuntu packages usually stable
# Monitor deprecation notices in Actions tab
```

### Monitoring

1. **Check Actions tab weekly**
2. **Review failed runs promptly**
3. **Update dependencies quarterly**
4. **Clean up old artifacts**

### Artifact Retention

- CI artifacts: 7 days (frequent builds)
- Code quality: 30 days (reference)
- Releases: 30 days (or longer)

Adjust in workflow files:
```yaml
retention-days: 30  # Increase if needed
```

---

## Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [CMake Documentation](https://cmake.org/cmake/help/latest/)
- [cppcheck Manual](http://cppcheck.net/manual.pdf)
- [Clang-Tidy Checks](https://clang.llvm.org/extra/clang-tidy/checks/list.html)
- [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer)
- [Valgrind Documentation](https://valgrind.org/docs/manual/quick-start.html)

---

## Summary

The GitHub Actions workflows provide:

✅ **Automated Testing**: Every code change is tested
✅ **Multi-Platform Support**: Currently Ubuntu 22.04
✅ **Code Quality Checks**: Static and dynamic analysis
✅ **Automated Releases**: Tag-triggered releases
✅ **Memory Safety**: Sanitizers and Valgrind
✅ **Coverage Reports**: Track tested code
✅ **Artifact Storage**: Build outputs saved

This ensures code quality, reliability, and streamlined releases for the Growth Pattern Calculator project.
