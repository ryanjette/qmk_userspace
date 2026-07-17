#!/usr/bin/env bash
set -euo pipefail

CONFIG_FILE="$HOME/.qmk-upstream.ini"

if [[ ! -f "$CONFIG_FILE" ]]; then
  echo "Missing config file: $CONFIG_FILE" >&2
  echo "Run: qmk --config-file \"$CONFIG_FILE\" config user.qmk_home=/Users/ryan.jette/qmk_firmware" >&2
  exit 1
fi

if [[ $# -eq 0 ]]; then
  cat <<'EOF'
Run QMK commands against the upstream qmk_firmware profile.

Usage:
  scripts/qmk-up.sh <qmk-subcommand> [args...]

Examples:
  scripts/qmk-up.sh env
  scripts/qmk-up.sh compile -kb <keyboard> -km <keymap>
EOF
  exit 0
fi

exec qmk --config-file "$CONFIG_FILE" "$@"
