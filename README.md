# Texture Packer for the Godot Engine

This is a texture packer engine module, for the Godot Engine.

Internally it uses the legacy version of rectpack2D. (https://github.com/TeamHypersomnia/rectpack2D)

# Building

1. Get the source code for the Godot Engine.

2. go into Godot's modules directory.

cd godot/modules/

3. clone this repository

git clone https://github.com/Relintai/texture_packer texture_packer

4. Build Godot. (https://docs.godotengine.org/en/latest/development/compiling/index.html)

# Features and Usage

The module contains:

## TexturePacker

This is the class that can merge textures. Add every thexture xou want into it using it's API, and then call merge().

Supports filters, custom background color, margins.

The keep_original_atlases option:

If you set this to true, and then add AtlasTextures, TexturePacker will change the added ones after the bake.
You can use this for example to bake gui textures together, without changing the resources everywhere at runtime.

Note: I only recommend using this if your ui textures change during runtime, if they don't just make an atlas yourself.

## TextureMerger

A Node that can bake textures for you. It uses TexturePacker internally.

## PackerImageResource

This is a simple Texture, which just cuntains an imported Image.

Useful for textures you only keep for baking, as this class will not register it's data into the VisualServer.

The module also contains an editor plugin which can import textures as this Resource.

To access it, click on a texture, switch to the import tab, and in the "Import As" Dropdown, select "Packer Image Recource".
