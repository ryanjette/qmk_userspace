# QMK Dual-Repo Workflow (Bastardkb + Upstream)

This userspace builds against two different QMK repos:

- Bastardkb fork (for Bastardkb boards like Charybdis Splinky)
- Upstream qmk_firmware (for other keyboards)

This file is the quick-start and yearly refresh guide.

## Why this setup exists

Some Bastardkb targets are maintained in the Bastardkb fork first, while other keyboards should build from upstream QMK.
Using separate config files avoids constantly rewriting global QMK settings.

## One-time setup (already done)

Two config profiles:

- `~/.qmk-bkb.ini`
- `~/.qmk-upstream.ini`

Shell shortcuts in `~/.zshrc`:

- `qmk-bkb` -> run QMK against Bastardkb fork
- `qmk-up` -> run QMK against upstream
- `qmk-bkb-env` -> show active Bastardkb environment details
- `qmk-up-env` -> show active upstream environment details

## Daily usage

### Use helper scripts (recommended)

From the userspace root:

```sh
./scripts/qmk-bkb.sh compile -kb bastardkb/charybdis/3x5 -km mineWorking
./scripts/qmk-up.sh compile -kb <keyboard> -km <keymap>
```

Quick environment checks:

```sh
./scripts/qmk-bkb.sh env | grep -i QMK_HOME
./scripts/qmk-up.sh env | grep -i QMK_HOME
```

These wrappers enforce the right config file each time and reduce accidental wrong-repo builds.

### Build Bastardkb firmware

```sh
qmk-bkb compile -kb bastardkb/charybdis/3x5 -km mineWorking
```

### Build non-Bastardkb firmware (upstream)

```sh
qmk-up compile -kb <keyboard> -km <keymap>
```

### Build and flash Elite-C firmware

Elite-C controllers use the Atmel DFU bootloader. Set this in the keymap's
`rules.mk`:

```make
BOOTLOADER = atmel-dfu
```

For the Jetset Corne keymap, run the flash command and then press the
controller's reset button when QMK begins waiting for the DFU device:

```sh
qmk-up flash -kb crkbd -km Jetset
```

Flash each half separately. The reset button normally restarts the firmware;
while the flash command is waiting, it makes the Elite-C enumerate as the
Atmel DFU device. The command erases, flashes, validates, and resets the
controller automatically.

### Sanity check which repo a command will use

```sh
qmk-bkb-env
qmk-up-env
```

## Yearly refresh checklist

Run these when you update repositories after a long break.

1. Update Bastardkb fork:

```sh
cd /Users/ryan.jette/bastardkb-qmk
git fetch origin
git checkout origin/bkb-master
git submodule update --init --recursive
```

2. Update upstream repo:

```sh
cd /Users/ryan.jette/qmk_firmware
git pull --ff-only
git submodule update --init --recursive
```

3. Confirm profile settings did not drift:

```sh
qmk --config-file "$HOME/.qmk-bkb.ini" config
qmk --config-file "$HOME/.qmk-upstream.ini" config
```

Expected values:

- BKB profile `user.qmk_home = /Users/ryan.jette/bastardkb-qmk`
- Upstream profile `user.qmk_home = /Users/ryan.jette/qmk_firmware`
- Both profiles `user.overlay_dir = /Users/ryan.jette/Documents/GitHub/qmk_userspace`

4. Run one test compile for each profile:

```sh
qmk-bkb compile -kb bastardkb/charybdis/3x5 -km mineWorking
qmk-up compile -kb <known_good_keyboard> -km <known_good_keymap>
```

## Troubleshooting

### Error: invalid keyboard value for Bastardkb path

If `bastardkb/charybdis/3x5/v2/splinky_3` fails in Bastardkb fork, use:

```sh
qmk-bkb compile -kb bastardkb/charybdis/3x5 -km mineWorking
```

The Bastardkb fork currently exposes that board as `bastardkb/charybdis/3x5`.

### Error: missing RP2040 startup file / ChibiOS files

Run submodule sync/update in the target repo:

```sh
git submodule update --init --recursive
```

### Built with the wrong repo by accident

Use the prefixed commands only (`qmk-bkb` or `qmk-up`) and verify with:

```sh
qmk-bkb-env
qmk-up-env
```

### Elite-C is not detected for flashing

Start the upstream flash command first, then press reset on the connected
half. The expected bootloader device is `03EB:2FF4` (`ATm32U4DFU`). If using
the native macOS command-line flasher instead of QMK, use the ARM64 Homebrew
binary:

```sh
/opt/homebrew/bin/dfu-programmer atmega32u4 erase --force
/opt/homebrew/bin/dfu-programmer atmega32u4 flash --force \
	/Users/ryan.jette/Documents/GitHub/qmk_userspace/crkbd_rev1_Jetset.hex
```

QMK Toolbox's bundled `dfu-programmer` may be Intel-only on Apple Silicon and
can fail with `Bad CPU type in executable`; the Homebrew binary avoids that
problem.
