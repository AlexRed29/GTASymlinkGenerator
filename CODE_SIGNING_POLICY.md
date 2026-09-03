# Code Signing Policy

## Project
**GTA Symlink Generator**
GTA Symlink Generator is an open-source Windows utility for creating symbolic links and copying required files for supported Grand Theft Auto installations and mods.

## Maintainer
GTA Symlink Generator is maintained by **AlexRed29X**.

The project maintainer is responsible for the project's source code, build configuration, release process, and official release artifacts.

## Code Signing
**Free code signing provided by SignPath.io, certificate by SignPath Foundation**
The SignPath Foundation code-signing certificate is used exclusively for official releases of GTA Symlink Generator.
The private signing key is managed by SignPath Foundation and is not stored in the source code repository.

## Source Code
The complete source code and build configuration used to produce official binaries are publicly available in this repository.
The project uses Visual Studio and GitHub Actions to build the application.
Signed binaries must be verifiably built from the source code contained in this repository.

## Build and Release Process
The project uses GitHub Actions for automated builds.

The build workflow:
1. Checks out the project's source code.
2. Builds the Release Win32 configuration using Microsoft MSBuild.
3. Produces the GTA Symlink Generator executable.
4. Stores the resulting executable as a GitHub Actions artifact.

Official releases are published through GitHub Releases.
Each release submitted for code signing is reviewed before signing.
Only binaries produced from the project's authorized source code and build process may be submitted for signing.

## Signing Scope
The SignPath Foundation certificate is used only for official GTA Symlink Generator releases.
The project maintainer does not distribute third-party software under the project's signing identity.

## Version Information
Official binaries contain product and version metadata identifying:
* Product name: GTA Symlink Generator
* Product version: the corresponding project release version
* Company/Maintainer: AlexRed29X

## Repository
The official source repository is:
https://github.com/AlexRed29/GTASymlinkGenerator

The repository contains the project's source code, build configuration, documentation, license, and GitHub Actions workflow.

## License
GTA Symlink Generator is distributed under the **MIT License**.

The project's license is available in the repository as `LICENSE`.

## Privacy
GTA Symlink Generator is a local Windows utility.
The application does not intentionally collect, transmit, or sell personal information.
The project does not require users to create an account or provide personal information to use the application.

## Security and Secrets
Private keys, code-signing certificates containing private keys, passwords, access tokens, and other confidential credentials must not be stored in the public source repository.
Repository and build-system secrets are kept outside the source code and are not included in released source files.

## Changes to the Signing Process
Significant changes to the project's source code, build configuration, repository, or release process that may affect the code-signing process will be documented in the repository.

## Official Releases
Only releases published through the project's official GitHub repository are considered official GTA Symlink Generator releases.

Users should obtain official releases from:
https://github.com/AlexRed29/GTASymlinkGenerator/releases

## Team Roles
### Maintainer
**AlexRed29X**

Responsible for maintaining the source code, build configuration, documentation, and releases.
Additional project roles may be added if the project develops additional maintainers or contributors.

## Code Signing Policy Location
This document is the official **Code Signing Policy** for GTA Symlink Generator and is publicly available in the project's GitHub repository.
