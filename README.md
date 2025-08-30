# NullShell (nullsh) v0.1.0

```plain
  _   _       _ _  _____ _          _ _ 
 | \ | |     | | |/ ____| |        | | |
 |  \| |_   _| | | (___ | |__   ___| | |
 | . ` | | | | | |\___ \| '_ \ / _ \ | |
 | |\  | |_| | | |____) | | | |  __/ | |
 |_| \_|\__,_|_|_|_____/|_| |_|\___|_|_|
```

> **The intentional shell for a noisy world.**  
> *Embrace the void. Execute with purpose.*

[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![GitHub Issues](https://img.shields.io/github/issues/sebdevnull/nullsh.svg)](https://github.com/sebdevnull/nullsh/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/sebdevnull/nullsh.svg)](https://github.com/sebdevnull/nullsh/pulls)

**nullsh** is a minimalist Linux shell designed for focus. It runs commands silently by default, printing output only on error or when you intentionally ask for it. Reduce cognitive load and execute with purpose.

For a full list of changes in each release, check out the [CHANGELOG](CHANGELOG.md).

---

## ❓ Why Nullsh?

Nullsh exists to simplify terminal workflows:

- Reduces clutter by showing output only when needed  
- Forces intentionality with operator syntax (`!`, `?`, `$?`, `$$?`)  
- Ideal for repetitive tasks, scripts, and focused development sessions

These practical benefits are all guided by our core [philosophy](#-philosophy) of intentionality.

---

## 🖤 Philosophy

`nullsh` is built on a few core principles:

- **Silence is Golden:** Successful commands execute quietly. Your terminal stays clean.
- **Embrace the Ephemeral:** Sessions are transient by nature, encouraging a focus on the present task.
- **Intentionality Over Habit:** Operators ensure every output or error you see is a conscious choice.

---

## ⚡ Features

- **Minimalist Interface:** A clean prompt free of distractions.
- **Silent by Default:** Commands that succeed do not print output.
- **Ephemeral Sessions:** No history or state persists by default.
- **Powerful Operators:** Control output and inspect state with `!`, `?`, `$?`, and `$$?`.
- **Essential Built-ins:** Includes `cd`, `pwd`, `echo`, and `exit`.
- **Runs Any Command:** Seamlessly executes all your existing external tools (`ls`, `grep`, `vim`, etc.).
- **Flexible Execution:** Support for both interactive sessions and one-off commands.

---

## 🛠️ Installation

**Requirements:** A C++23 compiler (GCC 13.3.0+ or Clang 16+) and CMake 3.27+.

```bash
# 1. Clone the repository
git clone https://github.com/sebdevnull/nullsh.git
cd nullsh

# 2. Build with CMake (using the release preset for optimized builds)
cmake --preset release
cmake --build --preset release

# 3. Install globally (optional, requires sudo)
sudo cmake --build --preset install-release
```

The `nullsh` binary will be installed to `/usr/local/bin/nullsh`.

For information on how to use nullsh and its command-line options, see the [Command-Line Interface](#-command-line-interface) section.

---

## 🖥️ Command-Line Interface

NullShell provides several options for different use cases. The basic syntax is:

**Usage:** `nullsh [OPTIONS]`

### Options

| Option | Short | Description |
| :--- | :--- | :--- |
| `--help` | `-h` | Show help message and exit. |
| `--version` | `-v` | Show version information and exit. |
| `--build-info` | | Show build information (compiler, flags, etc.). |
| `--command` | `-c` | Execute a single command and exit. |
| `--spawn` | `-s` | Launch NullShell in a new terminal window. |

### Examples

**Execute a command without entering the interactive shell:**

```bash
nullsh -c 'ls /tmp !'
```

**Launch in a new terminal emulator window:**

```bash
nullsh --spawn
```

**Start interactive session (default behavior):**

```bash
nullsh
```

---

## 🚀 Quick Start

The simplest way to understand `nullsh` is to see it in action. **Its core rule is: no news is good news.**

```bash
# Start a session
nullsh

# This will print nothing. The command succeeded, and that's all you need to know.
nullsh> echo "Hello Void"

# This will print an error message (from the 'ls' command). Failures are never silent.
nullsh> ls /a/path/that/does/not/exist
ls: cannot access '/a/path/that/does/not/exist': No such file or directory

# Force output with the '!' operator
nullsh> echo "Hello Void" !
Hello Void

# Run a command silently, even if it fails, with the '?' operator
nullsh> ls /a/path/that/does/not/exist ?
# (Absolutely nothing is printed, not even the error)

# Check the return code of the last command
nullsh> echo "All good" $?
0
nullsh> ls /bad/path $?
2

# Get a verbose explanation of the return code
nullsh> ls /bad/path $$?
2 (failure)

# Exit the shell
nullsh> exit
```

---

## 📖 Usage

### Built-in Commands

`nullsh` provides a few essential **built-in** commands:

- **`cd [dir]`** - Change directory.
- **`pwd`** - Print working directory.
- **`echo [args]`** - Print arguments. (Silent without `!`).
- **`exit [code]`** - Exit the shell.

### External Commands

Any other command (e.g., `ls`, `cat`, `git`, `python`) is an **external command** and is executed just like in any other shell.

### Operator Reference

Operators in nullsh allow you to **modify command output or inspect return codes**:

| Operator | Description | Example |
| :--- | :--- | :--- |
| (none) | **Silent Success:** default behavior. No output on success, errors are shown. | `echo "void"`|
| `!` | **Force Output:** print both stdout and stderr | `echo "void" !` |
| `?` | **Silent Run:** suppress all output, even on failure | `ls /tmp ?` |
| `$?` | **Return Code:** print numeric exit code of previous command | `ls /tmp $?` → `0` |
| `$$?`| **Verbose Return Code:** print exit code with success/failure message | `ls /bad $$?` → `2 (failure)` |

---

## 🛤️ Roadmap

Planned features for future releases of nullsh:

- [ ] Configuration file (`~/.nullshrc`) for user preferences.
- [ ] Enhanced prompt customization.
- [ ] `nshctl` utility for runtime configuration.
- [ ] Command history.
- [ ] Advanced scripting support.
- [ ] "Focus mode" for an even more minimal experience.
- [ ] Anonymous session mode.

---

## 🧩 Contributing

We welcome contributions that align with the `nullsh` philosophy of intentionality and minimalism.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

Please discuss significant changes via [GitHub Issues](https://github.com/sebdevnull/nullsh/issues) first.

---

## 🐞 Feedback / Reporting

Encounter issues or unexpected behavior? Report via [GitHub Issues](https://github.com/sebdevnull/nullsh/issues).

---

## 📜 License

Distributed under the GNU GPLv3 License. See the [LICENSE](LICENSE) file for more information.

---

*Embrace the void. Execute with purpose.*
