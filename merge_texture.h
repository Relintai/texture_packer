#ifndef MERGE_TEXTURE_H
#define MERGE_TEXTURE_H

#include "core/image.h"
#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"
#include "scene/resources/texture.h"
#include <vector>

#include "rectpack2D/pack.h"

//Rename to TexturePacker
class MergeTexture : public Reference {
	GDCLASS(MergeTexture, Reference);

public:
	Ref<AtlasTexture> add_texture(Ref<Texture> texture);
	Ref<AtlasTexture> get_texture(int index);
	Ref<Texture> get_original_texture(int index);
	void remove_texture_index(int index);
	void remove_texture(Ref<Texture> texture);
	int get_texture_count();

	void merge();

	String test();

	MergeTexture();
	~MergeTexture();

protected:
	static void _bind_methods();

private:
	bool _keep_original_atlases;

	std::vector<bin> _bins;

	Vector<rect_xywhf *> _rects;
	Vector<Ref<ImageTexture> > _generated_textures;
};

#endif
