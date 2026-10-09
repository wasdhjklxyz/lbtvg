#!/usr/bin/env bash
# Run the game under wine from a private test copy, outside Steam (no DRM).
#
#   tools/play.sh setup     build play/game (symlinks to GAME_DIR, a copy of
#                           orig/LEGOBatman.exe) and the wine prefix play/prefix
#   tools/play.sh run [...] start the game. Direct3D goes through DXVK (Vulkan),
#                           installed into the prefix once via winetricks;
#                           DXVK=0 uses wine's OpenGL path instead, which cannot
#                           reach the NVIDIA driver from a nix wine on NixOS (the
#                           game then exits with "failed to create d3d device")

#
# The exe and anything you drop next to it (a proxy dinput8.dll for testing
# functions) live only in play/game; the Steam install is never written.
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/.." && pwd)
GAME_DIR=$(readlink -f "${GAME_DIR:-$ROOT/game}")
PLAY=${PLAY:-$ROOT/play}
export WINEPREFIX="$PLAY/prefix" WINEDEBUG=${WINEDEBUG:--all}

if ! command -v wine >/dev/null; then
  command -v nix >/dev/null || { echo "play: need wine (and winetricks) on PATH" >&2; exit 1; }
  exec nix develop "$ROOT#play" --command "$0" "$@"
fi

reg() {   # 32-bit view: the game is a 32-bit exe
  wine reg add 'HKLM\SOFTWARE\Warner Bros Interactive Entertainment\LEGOBatman' /reg:32 /f "$@" >/dev/null
}

setup() {
  [ -f "$GAME_DIR/GAME.DAT" ] || { echo "play: GAME_DIR=$GAME_DIR has no GAME.DAT" >&2; exit 1; }
  sums=$(cd "$ROOT" && sha1sum -c orig/checksum.sha1 2>/dev/null || true)
  [[ $sums == *"orig/LEGOBatman.exe: OK"* ]] ||
    { echo "play: orig/LEGOBatman.exe missing or not verified (make verify)" >&2; exit 1; }
  mkdir -p "$PLAY/game"
  for f in "$GAME_DIR"/*; do
    n=$(basename "$f")
    case "$n" in *.exe|*.EXE|dinput8.dll) continue ;; esac
    ln -sfn "$f" "$PLAY/game/$n"
  done
  cp -f "$ROOT/orig/LEGOBatman.exe" "$PLAY/game/LEGOBatman.exe"
  echo "play: game copy in $PLAY/game (data symlinked from $GAME_DIR)"
  wineboot -u >/dev/null 2>&1 || true
  # what the Steam install script (21000_install.vdf) sets up
  reg /v PathVal /t REG_SZ /d "$(winepath -w "$PLAY/game")"
  reg /v SilentVal /t REG_DWORD /d 0
  reg /v LanguageVal /t REG_DWORD /d 1033
  # Microsoft's d3dx9_35 from the game's own DirectX redist (what Steam's
  # DXSETUP /silent installs): wine's builtin cannot compile the game's HLSL
  # shaders, so the screen stays mostly black without it.
  cab=$(ls "$GAME_DIR"/DirectX/*d3dx9_35_x86.cab 2>/dev/null | head -1)
  if [ -n "$cab" ]; then
    tmp=$(mktemp -d)
    cabextract -q -d "$tmp" "$cab"
    cp -f "$tmp/d3dx9_35.dll" "$WINEPREFIX/drive_c/windows/syswow64/d3dx9_35.dll"
    rm -rf "$tmp"
    wine reg add 'HKCU\Software\Wine\DllOverrides' /v d3dx9_35 /t REG_SZ /d native /f >/dev/null
    echo "play: native d3dx9_35 installed from $(basename "$cab")"
  else
    echo "play: no d3dx9_35 cab in $GAME_DIR/DirectX; shaders will likely fail" >&2
  fi
  echo "play: prefix ready in $WINEPREFIX"
}

run() {
  [ -f "$PLAY/game/LEGOBatman.exe" ] || setup
  if [ "${DXVK:-1}" = 1 ] && [ ! -f "$PLAY/.dxvk" ]; then
    winetricks -q dxvk && touch "$PLAY/.dxvk"
  fi
  cd "$PLAY/game"
  exec wine LEGOBatman.exe "$@"
}

case "${1:-run}" in
  setup) setup ;;
  run) shift || true; run "$@" ;;
  *) sed -n '2,12p' "$0"; exit 1 ;;
esac
