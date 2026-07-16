#!/usr/bin/env bash
set -euo pipefail

CONFIG_FILE="$HOME/.qmk-bkb.ini"

if [[ ! -f "$CONFIG_FILE" ]]; then
  echo "Missing config file: $CONFIG_FILE" >&2
  echo "Run: qmk --config-file \"$CONFIG_FILE\" config user.qmk_home=/Users/ryan.jette/bastardkb-qmk" >&2
  exit 1
fi

if [[ $# -eq 0 ]]; then
  cat <<'EOF'
Run QMK commands against the Bastardkb fork profile.

Usage:
  scripts/qmk-bkb.sh <qmk-subcommand> [args...]

Examples:
  scripts/qmk-bkb.sh env
  scripts/qmk-bkb.sh compile -kb bastardkb/charybdis/3x5 -km mineWorking
EOF
  exit 0
fi

exec qmk --config-file "$CONFIG_FILE" "$@"
