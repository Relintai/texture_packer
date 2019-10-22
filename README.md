# Texture Packer for the Godot Engine

This is a texture packer engine module, for the Godot Engine.

Internally it uses the legacy version of [rectpack2D](https://github.com/TeamHypersomnia/rectpack2D/tree/legacy)

It *should* work on all platforms.

# Building

1. Get the source code for the Godot Engine.

```
git clone https://github.com/godotengine/godot.git godot
```

2. Go into Godot's modules directory.

```
cd ./godot/modules/
```

3. Clone this repository

```
git clone https://github.com/Relintai/texture_packer texture_packer
```

4. Build Godot. [Tutorial](https://docs.godotengine.org/en/latest/development/compiling/index.html)

# Features and Usage

## TexturePacker

This is the class that can merge textures. Add every texture you want into it using it's API (add_texture()), and then call merge().

add_texture() will return an AtlasTexture, this is the texture you want to use in your classes. It is immediately usable, it will just contain the original texture. Calling merge() will change it, to point to the new (merged) texture.

Supports filters, custom background color, margins.

### The keep_original_atlases option:

If you set this to true, and then add AtlasTexture(s), TexturePacker will change these ones (the ones you actually added) 
after the bake.

You can use this to bake gui textures together, without changing the resources everywhere at runtime.
Think of rpgs, when you have a huge number of potential icons that the player can put on his or her actionbars.
You can look look at Tales of Maj'Eyal or pretty much every actually complex MMORPGs as an example.

Note: Doing something like this in only recommended, if you can't pre-make the atlases (or it's really unfeasible), you are better off 
making the atlases yourself during development.

## TextureMerger

A Node that can bake textures for you. It uses TexturePacker internally.

It has an exposed Array, so you can assign textures to it in the editor.

## PackerImageResource

This is a simple Texture, which just contains an imported Image. It has no logic for drawing.

Useful for textures you only need for baking, as this class will not register it's data into the VisualServer.

The module also contains an editor plugin which can import textures as `PackerImageResource` Resource.

To access it, click on a texture, switch to the import tab, and in the "Import As" Dropdown, select "Packer Image Recource".
