# Build Wave Cursor on GitHub Actions

This project includes a GitHub Actions workflow that builds the Windows `.geode` package on GitHub's `windows-2025-vs2026` hosted runner. That runner provides Visual Studio 2026, so the build does not depend on the older Visual Studio 2022 compiler installed on your PC.

## Build steps

1. Create a new GitHub repository, or fork a repository you control.
2. Upload/commit the entire contents of this project, including `.github/workflows/multi-platform.yml`.
3. Open the repository's **Actions** tab.
4. Select **Build Wave Cursor (Windows)**.
5. Click **Run workflow** and choose the branch containing the fixed source.
6. Wait for the workflow to finish successfully.
7. Open that workflow run and, under **Artifacts**, download **WaveCursor-Windows**.
8. The downloaded artifact contains the compiled `.geode` file. Install that `.geode` package with Geode; do not put the source ZIP or `.patch` file in the Geode mods folder.

The workflow also runs automatically whenever you push a commit to the repository.
