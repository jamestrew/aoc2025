{
  description = "C";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {

        buildInputs = [
          pkgs.gcc
          pkgs.clang-tools
          pkgs.bear
          pkgs.gdb
          pkgs.pkg-config
        ];

      };
    };
}
