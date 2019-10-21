#ifndef TEXTURE_MERGER_H
#define TEXTURE_MERGER_H

#include "scene/main/node.h"

#include "core/vector.h"
#include "core/engine.h"

#include "texture_packer.h"

class TextureMerger : public Node {
	GDCLASS(TextureMerger, Node);

public:
	int get_texture_flags() const;
	void set_texture_flags(const int flags);

	int get_max_atlas_size() const;
	void set_max_atlas_size(const int size);

	bool get_keep_original_atlases() const;
	void set_keep_original_atlases(const bool value);

	Color get_background_color() const;
	void set_background_color(const Color color);

	int get_margin() const;
	void set_margin(const int margin);

	bool get_automatic_merge() const;
	void set_automatic_merge(const bool value);

	Ref<TexturePacker> get_packer() const;
	void set_packer(const Ref<TexturePacker> packer);

	Vector<Variant> get_textures();
	void set_textures(const Vector<Variant> &textures);

	Ref<AtlasTexture> add_texture(Ref<Texture> texture);
	Ref<AtlasTexture> get_texture(int index);
	Ref<Texture> get_original_texture(int index);
	bool contains_texture(Ref<Texture> texture);

	void unref_texture_index(int index);
	void unref_texture(Ref<Texture> texture);
	void remove_texture_index(int index);
	void remove_texture(Ref<Texture> texture);

	int get_texture_count();
	void clear();

	Ref<ImageTexture> get_generated_texture(int index);
	int get_generated_texture_count();

	void merge();

	TextureMerger();
	~TextureMerger();

protected:
	static void _bind_methods();

private:
	bool _automatic_merge;

	Ref<TexturePacker> _packer;
	Vector<Ref<Texture> > _textures;
};

#endif
