#!/usr/bin/env bash
# Run the game under wine from a private test copy, outside Steam (no DRM).
# Needs your own copy of the game: orig/LEGOBatman.exe (verified) and the
# installed game data in GAME_DIR. Nothing from the game is in this repo.
#
#   tools/play.sh setup     build play/game (symlinks to GAME_DIR, a copy of
#                           orig/LEGOBatman.exe) and the wine prefix
#   tools/play.sh run [...] start the game (extra args go to the game)
#
# Settings (environment):
#   WINDOW=1280x720    window size (default), the game's own -Windowed mode:
#                      title bar with maximize/fullscreen, quick to close
#   FULLSCREEN=1       -Fullscreen inside a wine virtual desktop of RES
#   RES=1920x1080      fullscreen resolution; without the virtual desktop the
#                      game sees no display modes (0x0) and drops to 640x480
#   DXVK=0             don't install DXVK (Direct3D 9 over Vulkan); wine's
#                      OpenGL d3d9 can't reach NVIDIA from a nix wine on NixOS
#   WINE=/path/wine    a specific wine (default: `wine` on PATH, else nix)
#   WINETRICKS=...     a specific winetricks (only used to install DXVK)
#   D3DX9_DLL=...      your own d3dx9_35.dll instead of the one extracted
#                      from the game's DirectX redist cab
#   PLAY_PREFIX=...    wine prefix to use (default play/prefix)
#   GAME_DIR=...       installed game directory (default ./game)
#
# The exe and anything you drop next to it (e.g. a test dinput8.dll) live
# only in play/game; the Steam install is never written.
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/.." && pwd)
PLAY=${PLAY:-$ROOT/play}
GAME_DIR=$(readlink -f "${GAME_DIR:-$ROOT/game}")
export WINEPREFIX=${PLAY_PREFIX:-$PLAY/prefix}
export WINEDEBUG=${WINEDEBUG:--all}
WINE=${WINE:-wine}
WINETRICKS=${WINETRICKS:-winetricks}

if ! command -v "$WINE" >/dev/null; then
  command -v nix >/dev/null || { echo "play: no wine; install it or set WINE=/path/to/wine" >&2; exit 1; }
  exec nix develop "$ROOT#play" --command "$0" "$@"
fi

reg() {   # the game is a 32-bit exe: write the 32-bit registry view
  "$WINE" reg add 'HKLM\SOFTWARE\Warner Bros Interactive Entertainment\LEGOBatman' /reg:32 /f "$@" >/dev/null
}

install_d3dx9() {
  # Microsoft's d3dx9_35 (what Steam's DXSETUP installs): wine's builtin
  # cannot compile the game's HLSL shaders, so the screen stays mostly black.
  local dst="$WINEPREFIX/drive_c/windows/syswow64/d3dx9_35.dll" cab tmp
  if [ -n "${D3DX9_DLL:-}" ]; then
    cp -f "$D3DX9_DLL" "$dst"
    echo "play: d3dx9_35 from $D3DX9_DLL"
  else
    cab=$(ls "$GAME_DIR"/DirectX/*d3dx9_35_x86.cab 2>/dev/null | head -1)
    if [ -z "$cab" ]; then
      echo "play: no d3dx9_35 cab in $GAME_DIR/DirectX and no D3DX9_DLL; shaders will likely fail" >&2
      return
    fi
    command -v cabextract >/dev/null || { echo "play: need cabextract (or set D3DX9_DLL)" >&2; exit 1; }
    tmp=$(mktemp -d)
    cabextract -q -d "$tmp" "$cab"
    cp -f "$tmp/d3dx9_35.dll" "$dst"
    rm -rf "$tmp"
    echo "play: d3dx9_35 from the game's $(basename "$cab")"
  fi
  "$WINE" reg add 'HKCU\Software\Wine\DllOverrides' /v d3dx9_35 /t REG_SZ /d native /f >/dev/null
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
  "$WINE" wineboot -u >/dev/null 2>&1 || true
  # what the Steam install script (21000_install.vdf) sets up
  reg /v PathVal /t REG_SZ /d "$("$WINE" winepath -w "$PLAY/game")"
  reg /v SilentVal /t REG_DWORD /d 0
  reg /v LanguageVal /t REG_DWORD /d 1033
  install_d3dx9
  echo "play: prefix ready in $WINEPREFIX"
}

config() {   # pcconfig.txt: the game writes it on first run; pin our sizes in it
  local cfg
  cfg=$(find "$WINEPREFIX/drive_c/users" -path '*LEGO Batman/pcconfig.txt' 2>/dev/null | head -1)
  [ -n "$cfg" ] || return 0
  sed -i -E "s/^($1 +)[0-9]+/\\1$2/" "$cfg"
}

run() {
  [ -f "$PLAY/game/LEGOBatman.exe" ] || setup
  [ -f "$PLAY/.dxvk" ] && mv -f "$PLAY/.dxvk" "$WINEPREFIX/.lbtvg-dxvk"   # older marker location
  if [ "${DXVK:-1}" = 1 ] && [ ! -f "$WINEPREFIX/.lbtvg-dxvk" ]; then
    command -v "$WINETRICKS" >/dev/null || { echo "play: need winetricks for DXVK (or DXVK=0)" >&2; exit 1; }
    WINE="$WINE" "$WINETRICKS" -q dxvk && touch "$WINEPREFIX/.lbtvg-dxvk"
  fi
  cd "$PLAY/game"
  if [ "${FULLSCREEN:-0}" = 1 ]; then
    res=${RES:-1920x1080}
    config ScreenWidth "${res%x*}"; config ScreenHeight "${res#*x}"
    exec "$WINE" explorer "/desktop=lbtvg,$res" LEGOBatman.exe -Fullscreen "$@"
  fi
  size=${WINDOW:-1280x720}   # not SIZE: stdenv exports SIZE=size
  config WindowWidth "${size%x*}"; config WindowHeight "${size#*x}"
  exec "$WINE" LEGOBatman.exe -Windowed "$@"
}

case "${1:-run}" in
  setup) setup ;;
  run) shift || true; run "$@" ;;
  *) sed -n '2,28p' "$0"; exit 1 ;;
esac
