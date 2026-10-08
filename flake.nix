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
              binutils
              objdiff
              depotdownloader
              p7zip
              msitools
              cabextract
            ]);

            env = {
              ANALYZE_HEADLESS = "ghidra-analyzeHeadless";
              GHIDRA_RUN = "ghidra";
            };
          };
        }
      );
    };
}
