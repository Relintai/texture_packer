#ifndef MERGE_TEXTURE_H
#define MERGE_TEXTURE_H

#include "scene/resources/texture.h"

class MergeTexture : public ImageTexture {
	GDCLASS(MergeTexture, ImageTexture);

public:

	MergeTexture();
	~MergeTexture();

protected:
	static void _bind_methods();
};

#endif
