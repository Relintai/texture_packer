#ifndef TEXTURE_PACKER_H
#define TEXTURE_PACKER_H

#include "core/reference.h"
#include "core/image.h"
#include "core/color.h"
#include "core/ustring.h"
#include "core/vector.h"
#include "scene/resources/texture.h"
#include <vector>

#include "rectpack2D/pack.h"

class TexturePacker : public Reference {
	GDCLASS(TexturePacker, Reference);

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

	Ref<AtlasTexture> add_texture(Ref<Texture> texture);
	Ref<AtlasTexture> get_texture(int index);
	Ref<Texture> get_original_texture(int index);
	bool contains_texture(Ref<Texture> texture);

	bool unref_texture_index(int index);
	bool unref_texture(Ref<Texture> texture);
	void remove_texture_index(int index);
	void remove_texture(Ref<Texture> texture);

	int get_texture_count();
	void clear();

	Ref<ImageTexture> get_generated_texture(int index);
	int get_generated_texture_count();

	void merge();

	TexturePacker();
	~TexturePacker();

protected:
	static void _bind_methods();

private:
	int _texture_flags;
	int _max_atlas_size;
	bool _keep_original_atlases;
	Color _background_color;
	int _margin;

	Vector<rect_xywhf *> _rects;
	Vector<Ref<ImageTexture> > _generated_textures;
};

#endif
