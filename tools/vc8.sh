#!/usr/bin/env bash
# Fetch the matching compiler: Visual C++ 2005 SP1 (cl 14.00.50727.762), x86.
#
# Source: Microsoft's own "Windows SDK Update for Windows Vista" (Feb 2007)
# DVD ISO, which ships the VS2005 SP1 compilers and the matching CRT DLLs.
# The ISO is verified against the SHA-1 Microsoft publishes, the extracted
# binaries against tools/compiler.sha256.
#
# One non-Microsoft file: msvcr80.dll is replaced by Wine's reimplementation
# (encounter/winedll, LGPL). Microsoft's msvcr80.dll refuses to load outside a
# side-by-side activation context (R6034), which wibo does not implement. The
# original is kept as Bin/msvcr80.dll.ms for use under real Wine.
#
# Needs: curl, 7z (p7zip), msiextract (msitools), cabextract, sha1sum,
# sha256sum. All in the nix devshell.
#
# Result (default: <repo>/toolchain, gitignored; override with LBTVG_TOOLCHAIN):
#   $PREFIX/dist/     the ISO, kept so re-runs do not re-download
#   $PREFIX/vc8/      Bin/ INCLUDE/ LIB/   (VC compiler, CRT headers + libs)
#   $PREFIX/winsdk6/  Include/ Lib/        (Win32 headers + import libs)
set -euo pipefail

HERE=$(cd "$(dirname "$0")" && pwd)
PREFIX=${LBTVG_TOOLCHAIN:-$(cd "$HERE/.." && pwd)/toolchain}
ISO_NAME=6.1.6000.16384.10.WindowsSDK_Vista_Feb2007Update_rtm.DVD.Rel.iso
ISO_URL="https://download.microsoft.com/download/4/2/6/42684501-9ec5-43dd-9dfe-c8c9dfa6a66f/$ISO_NAME"
ISO_SHA1=5d28463daaa755450d697c850dd622d1f8b580ae
CRT_CAB=Setup/WinSDK-WinSDK_BIN_VC8_Runtime_X86_CRT-common.0.cab
CRT_ID=98CB24AD_52FB_DB5F_FF1F_C8B3B9A1E18E
WINEDLL_URL=https://github.com/encounter/winedll/releases/download/2026-07-10/msvcr80.dll
WINEDLL_SHA256=0cb555399211443705cb5d5229029253954ba0062e10ce42d5d5028792d4c18e

mkdir -p "$PREFIX/dist"
iso="$PREFIX/dist/$ISO_NAME"
if [ ! -f "$iso" ]; then
  echo ">> downloading $ISO_NAME (1.2 GB) from microsoft.com"
  curl -L --retry 3 -C - -o "$iso" "$ISO_URL"
fi
echo ">> verifying ISO"
echo "$ISO_SHA1  $iso" | sha1sum -c -

work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
echo ">> extracting compiler, SDK and CRT installers"
7z x -y -o"$work" "$iso" "Setup/WinSDKCompiler*" "Setup/WinSDKBuild*" "$CRT_CAB" > /dev/null
( cd "$work/Setup"
  msiextract -C ../compiler WinSDKCompiler-x86.msi > /dev/null
  msiextract -C ../build    WinSDKBuild-x86.msi    > /dev/null )
cabextract -q -d "$work/crt" "$work/$CRT_CAB"

# msiextract names directories after the MSI directory table, with ":./"
# separators; locate the real trees instead of guessing the string.
vc=$(find "$work/compiler" -type d -name VC | head -1)
inc=$(find "$work/build" -type d -name Include | head -1)
lib=$(find "$work/build" -type d -name Lib | head -1)
[ -n "$vc" ] && [ -n "$inc" ] && [ -n "$lib" ] || { echo "layout not found" >&2; exit 1; }

rm -rf "$PREFIX/vc8" "$PREFIX/winsdk6"
mkdir -p "$PREFIX/vc8" "$PREFIX/winsdk6"
cp -r "$vc/." "$PREFIX/vc8/"
cp -r "$inc" "$lib" "$PREFIX/winsdk6/"

echo ">> installing CRT DLLs next to cl.exe"
cp "$work/crt/nosxs_msvcp80.dll.$CRT_ID" "$PREFIX/vc8/Bin/msvcp80.dll"
cp "$work/crt/nosxs_msvcm80.dll.$CRT_ID" "$PREFIX/vc8/Bin/msvcm80.dll"
cp "$work/crt/nosxs_msvcr80.dll.$CRT_ID" "$PREFIX/vc8/Bin/msvcr80.dll.ms"
curl -sSL --retry 3 -o "$PREFIX/vc8/Bin/msvcr80.dll" "$WINEDLL_URL"
echo "$WINEDLL_SHA256  $PREFIX/vc8/Bin/msvcr80.dll" | sha256sum -c -

echo ">> verifying compiler binaries"
( cd "$PREFIX/vc8" && sha256sum -c "$HERE/compiler.sha256" )

if command -v wibo > /dev/null; then
  echo ">> smoke test: compiling one function with wibo"
  printf 'int add3(int a, int b, int c) { return a + b + c; }\n' > "$work/t.c"
  wibo "$PREFIX/vc8/Bin/cl.exe" /nologo /c /O2 /I"Z:$PREFIX/vc8/INCLUDE" \
       /Fo"Z:$work/t.obj" "Z:$work/t.c" > /dev/null
  [ -s "$work/t.obj" ] && echo ">> cl.exe works"
fi
if command -v python3 >/dev/null; then
  echo ">> case-alias links for clangd (Windows.h vs windows.h)"
  LBTVG_TOOLCHAIN="$PREFIX" python3 "$HERE/casefold.py"
fi
echo ">> done: $PREFIX/vc8 and $PREFIX/winsdk6"
