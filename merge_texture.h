#ifndef MERGE_TEXTURE_H
#define MERGE_TEXTURE_H

#include <vector>

#include "scene/resources/texture.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "rectpack2D/pack.h"

class MergeTexture : public ImageTexture {
	GDCLASS(MergeTexture, ImageTexture);

public:
	void add(Ref<Texture> texture);
	void get_texture(int index);
	int get_texture_count();
	void merge();

	String test();

	MergeTexture();
	~MergeTexture();

public:
	struct MergeTextureEntry {
		Ref<Texture> original_texture;
		Ref<AtlasTexture> texture;
		rect_xywhf rect;
	};

protected:
	static void _bind_methods();

private:
	bool _keep_original_atlases;
	std::vector<bin> _bins;
	Vector<MergeTextureEntry> _entries;
};

#endif
