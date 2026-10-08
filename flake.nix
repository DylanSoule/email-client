{
  description = "Dev environment for email client c++ usage";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-26.05";
  };

  outputs = { self , nixpkgs ,... }: let
    system = "x86_64-linux";
  in {
    devShells."${system}".default = let
      pkgs = import nixpkgs { inherit system; };
    in pkgs.mkShell {
      # include necessary packages 
      packages = with pkgs; [
	cmake
	pkg-config
	ninja
	ccache
	gcc
      ];

      buildInputs = with pkgs; [
	# Libraries
        curl
        vmime
      ];
    };
  };
}
