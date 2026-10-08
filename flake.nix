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
            ]
            ++ (with pkgs; [
              gnumake
              binutils
              objdiff
              depotdownloader
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
