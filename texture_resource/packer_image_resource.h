#ifndef PACKER_IMAGE_REOURCE_H
#define PACKER_IMAGE_REOURCE_H

#include "core/image.h"
#include "scene/resources/texture.h"

class PackerImageResource : public Texture {

	GDCLASS(PackerImageResource, Texture);

	RES_BASE_EXTENSION("tres");

public:
	void set_data(const Ref<Image> &p_image);
	Ref<Image> get_data() const;

	int get_width() const;
	int get_height() const;

	ImageTexture();
	~ImageTexture();

protected:
	static void _bind_methods();

private:
	Ref<Image> _image;
};

#endif
