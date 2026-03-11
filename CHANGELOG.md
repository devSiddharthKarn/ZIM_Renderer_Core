# Changelog

All notable changes to ZIM Core will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Linux support
- macOS support
- Flexbox-like layout system
- Grid layout system
- Animation system
- Theme support
- Built-in widget library
- More input device support

## [0.1.0] - 2026-03-11

### Added
- Initial release of ZIM Core
- Application class for managing application lifecycle
- Element class for UI components with hierarchical structure
- Window class for terminal window management
- Panel class for rectangular UI components
- Input handling for mouse and keyboard events
- Event system with event listeners
- Layout system with custom layout handlers
- Color support (RGB)
- Text styling support (bold, italic, underline, etc.)
- Windows platform support (Window_win32.cpp)
- Basic rendering with differential updates
- Buffer management
- Cursor control
- Element ID system for finding elements
- Test application (testui.cpp)

### Core Features
- **Element-Based Architecture**: DOM-like element structure
- **Event Handling**: Custom event listeners on elements
- **Flexible Layout**: Programmable layout handlers
- **Rich Styling**: Colors and text styles
- **Platform Support**: Windows (more platforms planned)

### Known Issues
- Limited to Windows platform currently
- No built-in widget library yet
- Layout system is manual (no auto-layout algorithms)
- Limited documentation (in progress)

### Breaking Changes
- None (initial release)

---

## Version History

### Version Numbering

ZIM Core follows Semantic Versioning:
- **Major version**: Incompatible API changes
- **Minor version**: New functionality (backwards compatible)
- **Patch version**: Bug fixes (backwards compatible)

### Release Notes Guide

Each release includes:
- **Added**: New features
- **Changed**: Changes in existing functionality
- **Deprecated**: Soon-to-be removed features
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Security fixes

---

## Upgrade Guides

### Upgrading to 0.1.0

Initial release - no upgrade path needed.

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to contribute to this project.

---

## Links

- [GitHub Repository](https://github.com/YOUR_USERNAME/ZIM_core)
- [Issue Tracker](https://github.com/YOUR_USERNAME/ZIM_core/issues)
- [Documentation](docs/)
