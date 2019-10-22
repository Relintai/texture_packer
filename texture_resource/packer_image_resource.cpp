#include "packer_image_resource.h"

void PackerImageResource::set_data(const Ref<Image> &p_image) {
	ERR_FAIL_COND(p_image.is_null());

	_image = p_image;
}

void PackerImageResource::_resource_path_changed() {

	String path = get_path();
}

Ref<Image> PackerImageResource::get_data() const {
	return _image;
}

int PackerImageResource::get_width() const {
	return w;
}

int PackerImageResource::get_height() const {
	return h;
}

void PackerImageResource::_set_data(Dictionary p_data) {

	Ref<Image> img = p_data["image"];
	ERR_FAIL_COND(!img.is_valid());
	uint32_t flags = p_data["flags"];
};

void PackerImageResource::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_data", "image"), &PackerImageResource::set_data);
}

PackerImageResource::PackerImageResource() {
	w = h = 0;
}

PackerImageResource::~PackerImageResource() {
	_image.unref();
}
