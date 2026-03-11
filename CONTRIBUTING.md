# Contributing to ZIM Core

Thank you for your interest in contributing to ZIM Core! This document provides guidelines and instructions for contributing to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Submitting Changes](#submitting-changes)
- [Reporting Bugs](#reporting-bugs)
- [Requesting Features](#requesting-features)

## Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

## Getting Started

### Prerequisites

- C++ compiler with C++11 support or higher (MSVC, GCC, Clang)
- Git for version control
- Basic understanding of terminal/console programming
- Familiarity with C++ and object-oriented programming

### Setting Up Development Environment

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/ZIM_core.git
   cd ZIM_core
   ```
3. **Add upstream remote**:
   ```bash
   git remote add upstream https://github.com/ORIGINAL_OWNER/ZIM_core.git
   ```
4. **Create a development branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

## How to Contribute

### Types of Contributions

We welcome various types of contributions:

- **Bug Fixes**: Fix issues reported in GitHub Issues
- **New Features**: Implement new functionality
- **Documentation**: Improve or add documentation
- **Tests**: Add or improve test coverage
- **Performance**: Optimize existing code
- **Platform Support**: Add support for Linux, macOS, etc.
- **Examples**: Create example applications

## Development Workflow

### 1. Create an Issue First

Before starting work on a significant change:
- Check existing issues to avoid duplication
- Create a new issue describing your proposed changes
- Wait for feedback from maintainers
- Get assigned to the issue before starting work

### 2. Branch Naming Conventions

Use descriptive branch names:
- `feature/add-linux-support` - New features
- `bugfix/window-resize-crash` - Bug fixes
- `docs/api-reference-update` - Documentation changes
- `refactor/panel-cleanup` - Code refactoring
- `test/element-layout-tests` - Test additions

### 3. Making Changes

1. Keep changes focused and atomic
2. Write clear, self-documenting code
3. Add comments for complex logic
4. Update documentation when needed
5. Add tests for new functionality

### 4. Testing Your Changes

Before submitting:
```bash
# Build the test application
g++ -std=c++11 tests/testui.cpp zim/*.cpp -o test_app

# Run and verify functionality
./test_app

# Test on different scenarios
# - Different terminal sizes
# - Different color schemes
# - Various input combinations
```

## Coding Standards

### C++ Style Guide

#### Naming Conventions

- **Classes**: PascalCase - `class Application`, `class Element`
- **Functions/Methods**: PascalCase - `void Execute()`, `Vector2D GetDimensions()`
- **Variables**: camelCase - `int bufferSize`, `Vector2D currentPosition`
- **Constants**: UPPER_SNAKE_CASE - `const int MAX_BUFFER_SIZE`
- **Namespaces**: lowercase - `namespace zim`
- **Private Implementation**: `Impl_` prefix - `struct Impl_Window`

#### Code Formatting

```cpp
// Header guards
#ifndef CLASS_NAME_HPP
#define CLASS_NAME_HPP

// Includes
#include "Types.hpp"
#include <string>

// Namespace
namespace zim
{
    // Class definition
    class ClassName
    {
    private:
        // Private members first
        int privateVariable;
        
    public:
        // Public interface
        void PublicMethod();
        
        // Constructor/Destructor
        ClassName();
        ~ClassName();
    };
}

#endif
```

#### Best Practices

1. **Use RAII**: Manage resources properly with constructors/destructors
2. **Const Correctness**: Use `const` where appropriate
3. **Forward Declarations**: Use when possible to reduce compile times
4. **Pimpl Idiom**: Use for implementation hiding (as seen in existing code)
5. **Avoid Naked Pointers**: Prefer smart pointers when possible
6. **Include Guards**: Use `#ifndef` guards in all headers

### File Organization

- **Header files (.hpp)**: Interface declarations only
- **Implementation files (.cpp)**: Implementation details
- **One class per file**: Unless tightly coupled classes
- **Private implementation**: Use Pimpl pattern for complex classes

## Testing Guidelines

### Writing Tests

Create test files in the `tests/` directory:

```cpp
#include "../zim/zim.hpp"
#include <cassert>

void TestElementCreation() {
    using namespace zim;
    
    Element element;
    element.Position() = MakeVector2D(5, 5);
    
    // Add assertions
    assert(element.Position().x == 5);
    assert(element.Position().y == 5);
}

int main() {
    TestElementCreation();
    // Add more tests
    return 0;
}
```

### Test Coverage

- Test all public APIs
- Test edge cases and error conditions
- Test platform-specific functionality
- Verify memory management (no leaks)

## Submitting Changes

### Pull Request Process

1. **Update your branch** with latest upstream:
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

3. **Create Pull Request** on GitHub:
   - Use a clear, descriptive title
   - Reference related issues (e.g., "Fixes #123")
   - Provide detailed description of changes
   - List any breaking changes
   - Add screenshots/videos for UI changes

### Pull Request Checklist

- [ ] Code follows the style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex code
- [ ] Documentation updated
- [ ] Tests added/updated
- [ ] All tests pass
- [ ] No compiler warnings
- [ ] Commits are logical and well-described
- [ ] Branch is up-to-date with main

### Commit Messages

Write clear commit messages:

```
Add Linux platform support for Window class

- Implement Window_linux.cpp for Unix terminals
- Add ANSI escape sequence handlers
- Update build instructions for Linux
- Add Linux-specific testing

Fixes #42
```

Format:
- **First line**: Short summary (50 chars max)
- **Blank line**
- **Body**: Detailed explanation (wrap at 72 chars)
- **Footer**: Reference issues/PRs

## Reporting Bugs

### Before Submitting

- Check if bug is already reported
- Use latest version of the code
- Verify it's actually a bug, not a usage issue

### Bug Report Template

```markdown
**Bug Description**
Clear description of what the bug is

**To Reproduce**
Steps to reproduce:
1. Create an element with '...'
2. Set position to '...'
3. Call Execute()
4. See error

**Expected Behavior**
What you expected to happen

**Actual Behavior**
What actually happened

**Environment**
- OS: Windows 10
- Compiler: MSVC 2019
- C++ Standard: C++17
- ZIM Version: commit hash or version

**Code Sample**
```cpp
// Minimal reproducible code
```

**Additional Context**
Any other relevant information
```

## Requesting Features

### Feature Request Template

```markdown
**Feature Description**
Clear description of the feature

**Use Case**
Why is this feature needed? What problem does it solve?

**Proposed Solution**
How would you implement this?

**Alternatives Considered**
Any alternative solutions or features you've considered

**Additional Context**
Mockups, examples, or references
```

## Code Review Process

### What to Expect

- Maintainers will review your PR within a few days
- Feedback may request changes or improvements
- Be responsive to comments and questions
- Iterate based on feedback
- Once approved, a maintainer will merge your PR

### Review Criteria

- **Functionality**: Does it work as intended?
- **Code Quality**: Is it well-written and maintainable?
- **Performance**: Does it introduce performance issues?
- **Testing**: Is it adequately tested?
- **Documentation**: Is it properly documented?
- **Compatibility**: Does it break existing code?

## Communication

### Where to Ask Questions

- **GitHub Issues**: For bug reports and feature requests
- **GitHub Discussions**: For general questions and discussions
- **Pull Request Comments**: For code-specific discussions

### Response Times

- Issues: We aim to respond within 3-5 days
- Pull Requests: Initial review within 5-7 days
- Questions: Usually within 2-3 days

## Recognition

All contributors will be:
- Listed in the project's contributors page
- Mentioned in release notes for their contributions
- Credited in relevant documentation

## License

By contributing to ZIM Core, you agree that your contributions will be licensed under the same license as the project (MIT License).

---

Thank you for contributing to ZIM Core! Your efforts help make this project better for everyone.
