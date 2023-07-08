{
  description = "libuv-tutorial";

  inputs = {
    parts = {
      url = "github:hercules-ci/flake-parts";
      inputs.nixpkgs-lib.follows = "nixpkgs";
    };

    mini-compile-commands = {
      url = "github:danielbarter/mini_compile_commands";
      flake = false;
    };
  };

  outputs = inputs @ {
    self
    , nixpkgs
    , parts
    , mini-compile-commands
  }: parts.lib.mkFlake { inherit inputs; } (let
    inherit (builtins) attrValues;

    pkgAttrs = pkgs: {
      src = ./.;
      nativeBuildInputs = attrValues {
        inherit (pkgs)
          gnumake
        ;
      };
      buildInputs = attrValues {
        inherit (pkgs)
          libuv
        ;
      };
    };
  in {
    systems = ["x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin"];

    perSystem = { pkgs, system, ... }: let
      stdenv = pkgs.stdenv;
      devShellInputs = attrValues {
        inherit (pkgs)
          gdb
          valgrind
        ;
      };
    in {
      devShells.default = pkgs.mkShell.override { stdenv = (pkgs.callPackage mini-compile-commands { }).wrap stdenv; }
        (let
          attrs = pkgAttrs pkgs;
        in
          attrs // {
            nativeBuildInputs = attrs.nativeBuildInputs ++ devShellInputs;
          });
    };
  });
}
