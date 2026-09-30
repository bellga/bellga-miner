# Bellga Miner

High-performance CPU miner for **VRSC** (VerusHash 2.2), **ZEPH** and **XMR**
(RandomX), built and tuned for [bellga.tech](https://bellga.tech) and its pool
(`pool.bellga.tech`: VRSC 3960, ZEPH 3961, XMR 3962) -- but it mines on any
compatible pool.

Bellga is a fork of [XMRig](https://github.com/xmrig/xmrig) 6.26.0
(GPL-3.0-or-later), with the VerusHash core ported from
[monkins1010/ccminer](https://github.com/monkins1010/ccminer). All the XMRig
algorithms (RandomX, KawPow, CryptoNight, GhostRider) are still here, as are
XMRig's config format and command-line options -- only the name changed:
the executable is now `bellga` (`bellga.exe` on Windows).

* **Config builder:** https://bellga.tech/config.html
* **CPU catalog, optimizations and roadmap:** https://bellga.tech/minerador.html
* **Pool stats:** https://bellga.tech/stats.html

### Coming from xmrig-vrsc?
Same code, new name. `config.json` works unchanged. The repository moved to
`bellga/bellga-miner` (GitHub redirects the old URL). Re-running
`./termux-build.sh` updates an existing `~/xmrig-vrsc` checkout in place and
leaves an `xmrig` link pointing to the new `bellga` binary, so old run
scripts keep working.

## VerusHash (VRSC)
This fork adds VerusHash 2.2 support for mining VRSC (Verus Coin), ported from
`monkins1010/ccminer`'s VerusHash core into XMRig's threading/pool
infrastructure, with a dedicated VerusCoin Stratum client. Supported on Linux
x86-64 (AES-NI/PCLMUL) and ARM64/ARMv7 (NEON, AES/PMULL crypto extension when
available -- see the Mobile section below).

## Mobile (Termux / UserLAnd, ARM64 and ARMv7)
The ARM port includes optional per-CPU-core tuning (`-mcpu`, see
`cmake/arm-cpu-tiers.cmake`, ARM64 only for now). To build and run directly
on an Android device via [Termux](https://termux.dev) or
[UserLAnd](https://github.com/CypherpunkArmory/UserLAnd) -- no file transfer
needed, just copy-paste this into the terminal:

```bash
curl -fsSL https://raw.githubusercontent.com/bellga/bellga-miner/master/termux-build.sh -o termux-build.sh
chmod +x termux-build.sh
./termux-build.sh
```

The script detects Termux vs. UserLAnd (and picks the right package manager
for each), installs build dependencies, detects your CPU core from
`/proc/cpuinfo` to pick a tuning tier automatically, and compiles natively
on-device. If it guesses the wrong core (see the script's own comments for
its confidence notes on this), override it explicitly:

```bash
./termux-build.sh cortex-a76      # force a specific -mcpu tier
ARM_CPU=none ./termux-build.sh    # skip per-core tuning, use the generic ARMv8-A+crypto build
```

On ARM64 with the crypto extension, VerusHash's clhash step uses a native
NEON/PMULL implementation (`src/crypto/verushash/verus_clhash_neon.cpp`,
bit-identical to the reference). To compare hashrate against the previous
sse2neon path on your device, rebuild with it turned off:

```bash
WITH_VERUS_NEON=OFF ./termux-build.sh   # old sse2neon clhash path (A/B benchmarking)
```

Confirmed working end-to-end on real hardware: ARM64 (Termux and UserLAnd,
2026-09-17) and 32-bit ARMv7 (UserLAnd on a Moto E7 Power, Cortex-A53,
2026-09-22 -- ~500-530 KH/s across 8 threads using the software AES/PMULL
fallback described in the warning above, mining VerusHash live against a
pool). If something looks wrong on your device, please open an issue with
the script's output.

**Note for UserLAnd on 32-bit ARM (armv7l) userlands specifically:** the
dependency-install step below works around a known `proot` limitation on
some vendor kernels, where `dpkg` fails to unpack a package with
`unable to read link '<path>': Invalid argument` while replacing a symlink
(hit repeatedly on Ubuntu's `perl` package during testing). The script now
detects that specific error and retries automatically; if it's a *different*
package failing the same way, it should still self-heal, but please open an
issue with the output if it doesn't.

## Hash tests
Run the offline CPU hash suite without pool, API, or miner network dependencies:

```bash
./tests/hash/check.sh
```

The script builds the standalone hash-test binary and runs both the regular known-answer suite and the full RandomX mode checks.

## Donations
* Default donation 5% (5 minutes in 100), adjustable with the `donate-level` option down to a minimum of 1%.
* When you mine VRSC, ZEPH or XMR, the donation minute goes to `pool.bellga.tech` in the same coin; other algorithms fall back to MoneroOcean.
* XMR: `89Qcz2NnSXtZf1NA5V8mt9DkswfbsN6HpaGaHbfnwwTuRwUDFVvgc7BZf1AKqPrmxzQktfB9hfLF8Znj8UwJxqFH4E5Nugc`

## Credits
* **[xmrig](https://github.com/xmrig)** -- upstream XMRig
* **[sech1](https://github.com/SChernykh)**

## Contacts
* Site: https://bellga.tech
* Issues: https://github.com/bellga/bellga-miner/issues
* Upstream XMRig: support@xmrig.com
