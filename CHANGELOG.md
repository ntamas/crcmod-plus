# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## 2.3.3

- No user-facing changes. Version bump triggers a new release process
  after the failure of `2.3.2` due to the deprecated `macos-13` image on
  Github Actions.

## 2.3.2

- No user-facing changes. Removed obsolete code that was needed only for
  Python <3.9, thanks to [@alexrudd2](https://github.com/alexrudd2) in
  [#5](https://github.com/ntamas/crcmod-plus/pull/5).

## 2.3.1

### Changed

- No user-facing changes, only build infrastructure changes and fixes.

## 2.3.0

### Changed

- The `predefined` submodule is now auto-imported so we remain a drop-in
  replacement to `crcmod`, thanks to @jamienelson-cmd

- Dropped support for Python 3.7 and 3.8 as they have reached EOL.

## 2.2.0

### Added

- Added support for Python 3.13 and its free-threaded variant.

## 2.1.0

### Added

- Added type annotations.

## 2.0.0

This is a modernised re-release of `crcmod` 1.7 from PyPI with no changes in
API or behaviour.
