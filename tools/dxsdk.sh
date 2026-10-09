#!/usr/bin/env bash
# Fetch the DirectX SDK the game was built against: August 2007 (the exe
# imports d3dx9_35.dll, which first shipped in this release).
#
# Microsoft no longer hosts pre-2008 DirectX SDKs. This downloads the
# original installer from the Internet Archive and refuses it unless its
# SHA-1 equals the one Microsoft published for dxsdk_aug2007.exe, i.e. it is
# byte-for-byte Microsoft's file. Nothing is redistributed by this repo; using
# the SDK means accepting Microsoft's DirectX SDK licence, as installing it
# on Windows would.
#
# Needs: curl, cabextract, unzip, sha1sum (all in the nix devshell).
# Result (default <repo>/toolchain, override with LBTVG_TOOLCHAIN):
#   $PREFIX/dist/dxsdk_aug2007.exe   kept so re-runs do not re-download
#   $PREFIX/dxsdk/Include            d3d9.h, d3dx9*.h, dsound.h, dinput.h, xinput.h, dxerr9.h ...
#   $PREFIX/dxsdk/Lib                x86 import/static libs (d3dx9.lib, dxerr9.lib, dxguid.lib ...)
set -euo pipefail

HERE=$(cd "$(dirname "$0")" && pwd)
PREFIX=${LBTVG_TOOLCHAIN:-$(cd "$HERE/.." && pwd)/toolchain}
NAME=dxsdk_aug2007.exe
URL=https://archive.org/download/dxsdk_aug2007/$NAME
SHA1=c812c18e2972bdb1d9cbb544be9ced9370a4656f   # Microsoft's published hash

mkdir -p "$PREFIX/dist"
exe="$PREFIX/dist/$NAME"
if [ ! -f "$exe" ]; then
  echo ">> downloading $NAME (469 MB) from the Internet Archive"
  curl -L --retry 3 -C - -o "$exe" "$URL"
fi
echo ">> verifying against Microsoft's SHA-1"
echo "$SHA1  $exe" | sha1sum -c -

work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
echo ">> unpacking (IExpress cab -> WinZip self-extractor -> zip)"
cabextract -q -d "$work" "$exe"
rm -rf "$PREFIX/dxsdk"
mkdir -p "$PREFIX/dxsdk"
unzip -q -o "$work/$NAME" 'Include/*' 'Lib/x86/*' -d "$work/sdk"
mv "$work/sdk/Include" "$PREFIX/dxsdk/Include"
mv "$work/sdk/Lib/x86" "$PREFIX/dxsdk/Lib"

if command -v python3 >/dev/null; then
  LBTVG_TOOLCHAIN="$PREFIX" python3 "$HERE/casefold.py" "$PREFIX/dxsdk/Include"
fi
echo ">> done: $PREFIX/dxsdk ($(ls "$PREFIX/dxsdk/Include" | wc -l) headers, $(ls "$PREFIX/dxsdk/Lib" | wc -l) libs)"
