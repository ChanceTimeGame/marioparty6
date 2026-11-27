# Mario Party 6 (US) decompilation

A matching decompilation of Mario Party 6 (GameCube, US, `GP6E01`). Built with the original
compilers, it produces a `main.dol` and RELs byte-identical to the retail game.

**Progress:** main.dol 35.2%, RELs 1.0%; overall 7.0% of code bytes. Only units with C appear in the objdiff report, so
[OPEN_FUNCTIONS.md](OPEN_FUNCTIONS.md) lists those; every unit without source is open too.

This repository is a fork of [mariopartyrd/marioparty6](https://github.com/mariopartyrd/marioparty6).
Thanks to everyone who worked on it, and to
[decomp-toolkit](https://github.com/encounter/decomp-toolkit), whose project template it is built on.

## Why this fork exists: Chance Time

This fork is the decompilation behind Mario Party 6 in **Chance Time**, a native PC port of the
N64 and GameCube Mario Party games. Chance Time ships no game data: players supply their own ROM
or disc.

How the two fit together:

- **This repository is a pure matching decompilation.** It holds decompiled game code only, with
  no PC-port code, no `#ifdef` for a PC build and no host types. Every commit passes the
  `build.sha1` check against the retail disc.
- **Chance Time will be built from it.** Chance Time's Mario Party 6 port has not started yet; when
  it does, it takes this repository as a submodule and keeps its own changes as a patch series on
  top, as it already does for [Mario Party 4](https://github.com/ChanceTimeGame/marioparty4). So
  the port is always built from code that is provably the original game logic, and the
  decompilation never carries port changes.
- **Non-matching C must still be correct.** The PC port will compile every unit's C, including units
  still marked `NonMatching`, so a function is only left short of 100% once its remaining
  difference is register allocation, instruction order or a symbol name: same calls, same
  constants, same stores.
- **The Mario Party games share an engine.** Mario Party 4 is fully matched
  ([ChanceTimeGame/marioparty4](https://github.com/ChanceTimeGame/marioparty4)) and Mario Party 5
  is over 80% ([ChanceTimeGame/marioparty5](https://github.com/ChanceTimeGame/marioparty5)). A
  large share of this game's code is identical or close to a function already decompiled there,
  so a twin is often the fastest start.

Once Mario Party 6 reaches 100% this repository is finished: it takes genuine decompilation fixes
only.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for the workflow, the checks a change must pass and the
compiler details that matter, and [OPEN_FUNCTIONS.md](OPEN_FUNCTIONS.md) for where to start. Pull
requests are welcome.

## Building

This repository contains no game assets or assembly. You need your own copy of the game.

1. Install [Python 3](https://www.python.org/downloads/) and
   [ninja](https://github.com/ninja-build/ninja/releases) (`pip install ninja` works). On Linux
   x86-64, [wibo](https://github.com/decompals/wibo) is downloaded for the Windows compilers; on
   other Linux platforms and on macOS, install Wine. On Windows, use native tools: WSL is not
   needed.
2. `git clone https://github.com/ChanceTimeGame/marioparty6`.
3. Put your disc image (ISO, RVZ, WIA, WBFS, CISO, NFS, GCZ or TGC) in `orig/GP6E01/`. After the
   first build it can be deleted.
4. `python configure.py`, then `ninja`. The build checks every output against
   `config/GP6E01/build.sha1` and writes the totals to `build/GP6E01/progress.json`.

To compare functions, open the repository in [objdiff](https://github.com/encounter/objdiff): it
reads `objdiff.json`, written by the first build, and rebuilds on every change.
