#ifndef MERGE_TEXTURE_H
#define MERGE_TEXTURE_H

#include "scene/resources/texture.h"
#include "core/ustring.h"

#include "rectpack2D/pack.h"

class MergeTexture : public ImageTexture {
	GDCLASS(MergeTexture, ImageTexture);

public:
	void test();

	MergeTexture();
	~MergeTexture();

protected:
	static void _bind_methods();
};

#endif
