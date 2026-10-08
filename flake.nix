{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    systems.url = "github:nix-systems/default";
  };

  outputs =
    {
      self,
      nixpkgs,
      systems,
    }:
    let
      eachSystem = nixpkgs.lib.genAttrs (import systems);
    in
    {
      devShells = eachSystem (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};

          ghidra = pkgs.ghidra.withExtensions (
            e: with e; [
              reva
              ghidra-delinker-extension
              machinelearning
            ]
          );

          # nixpkgs wibo is 0.6.14 and cannot run VC8 (missing kernel32 stubs);
          # upstream 1.2.0 builds need network at build time, so take the
          # static release binary. See docs/toolchain.md.
          wibo = pkgs.stdenvNoCC.mkDerivation {
            pname = "wibo";
            version = "1.2.0";
            src = pkgs.fetchurl {
              url = "https://github.com/decompals/wibo/releases/download/1.2.0/wibo-i686";
              hash = "sha256-JXXTsKL0CLLCsIUNtW8a9dAFoTg5Smd066d7ZwjswwQ=";
            };
            dontUnpack = true;
            installPhase = "install -Dm755 $src $out/bin/wibo";
          };

          python = pkgs.python3.withPackages (
            p: with p; [
              pefile
              lief
              capstone
            ]
          );
        in
        {
          default = pkgs.mkShell {
            name = "lbtvg";

            packages = [
              ghidra
              python
              wibo
            ]
            ++ (with pkgs; [
              gnumake
              curl
              file
              binutils # objdump/strings on PE
              llvmPackages.bintools-unwrapped # llvm-objdump/llvm-ar on COFF .obj and .lib
              clang-tools # clang-format
              objdiff
              depotdownloader
              p7zip # tools/vc8.sh
              msitools # tools/vc8.sh
              cabextract # tools/vc8.sh
            ]);

            env = {
              ANALYZE_HEADLESS = "ghidra-analyzeHeadless";
              GHIDRA_RUN = "ghidra";
            };

            # where tools/vc8.sh puts the compiler (repo-local, gitignored);
            # export LBTVG_TOOLCHAIN before nix develop to keep it elsewhere
            shellHook = ''
              export LBTVG_TOOLCHAIN="''${LBTVG_TOOLCHAIN:-$(git rev-parse --show-toplevel 2>/dev/null || pwd)/toolchain}"
              export VC8="$LBTVG_TOOLCHAIN/vc8"
              export WINSDK6="$LBTVG_TOOLCHAIN/winsdk6"
            '';
          };
        }
      );
    };
}
