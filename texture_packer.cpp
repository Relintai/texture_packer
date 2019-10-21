#include "texture_packer.h"

int TexturePacker::get_texture_flags() const {
	return _texture_flags;
}
void TexturePacker::set_texture_flags(const int flags) {
	_texture_flags = flags;
}

int TexturePacker::get_max_atlas_size() const {
	return _max_atlas_size;
}
void TexturePacker::set_max_atlas_size(const int size) {
	_max_atlas_size = size;
}

bool TexturePacker::get_keep_original_atlases() const {
	return _keep_original_atlases;
}
void TexturePacker::set_keep_original_atlases(const bool value) {
	_keep_original_atlases = value;
}

Ref<AtlasTexture> TexturePacker::add_texture(Ref<Texture> texture) {
	Ref<AtlasTexture> atlas_text = texture;

	if (atlas_text.is_valid()) {
		//If the supplied texture is an AtlasTexture, we set it as the target, and we create the original

		//we need to check differently this case
		for (int i = 0; i < _rects.size(); ++i) {
			rect_xywhf *r = _rects.get(i);

			if (r->atlas_texture == texture) {
				++(r->refcount);

				return r->atlas_texture;
			}
		}

		Ref<AtlasTexture> tex;
		tex.instance();

		tex->set_atlas(atlas_text->get_atlas());
		tex->set_region(atlas_text->get_region());

		rect_xywhf *rect = memnew(rect_xywhf);

		rect->refcount = 1;
		rect->w = atlas_text->get_region().size.x;
		rect->h = atlas_text->get_region().size.y;

		_rects.push_back(rect);

		if (_keep_original_atlases) {
			rect->original_texture = tex;
			rect->atlas_texture = atlas_text;

			return atlas_text;
		} else {
			rect->original_texture = atlas_text;
			rect->atlas_texture = tex;

			return tex;
		}
	}

	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		if (r->original_texture == texture) {
			++(r->refcount);

			return r->atlas_texture;
		}
	}

	Ref<AtlasTexture> tex;
	tex.instance();

	//Temp setup, so the texture is usable even while the atlases are generating.
	tex->set_atlas(texture);
	tex->set_region(Rect2(0, 0, texture->get_width(), texture->get_height()));

	rect_xywhf *rect = memnew(rect_xywhf);

	rect->refcount = 1;
	rect->original_texture = texture;
	rect->atlas_texture = tex;

	rect->w = texture->get_width();
	rect->h = texture->get_height();

	_rects.push_back(rect);

	return tex;
}

Ref<Texture> TexturePacker::get_original_texture(int index) {
	ERR_FAIL_INDEX_V(index, _rects.size(), Ref<Texture>());

	return _rects.get(index)->original_texture;
}

Ref<AtlasTexture> TexturePacker::get_texture(int index) {
	ERR_FAIL_INDEX_V(index, _rects.size(), Ref<AtlasTexture>());

	return _rects.get(index)->atlas_texture;
}

void TexturePacker::unref_texture_index(int index) {
	ERR_FAIL_INDEX(index, _rects.size());

	rect_xywhf *r = _rects.get(index);

	int rc = --(r->refcount);

	if (rc <= 0) {
		_rects.remove(index);

		r->atlas_texture.unref();
		r->atlas_texture.unref();

		memdelete(r);
	}
}

void TexturePacker::unref_texture(Ref<Texture> texture) {
	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		if (r->original_texture == texture) {

			int rc = --(r->refcount);

			if (rc <= 0) {
				_rects.remove(i);

				r->atlas_texture.unref();
				r->atlas_texture.unref();

				memdelete(r);
			}

			return;
		}
	}
}

void TexturePacker::remove_texture_index(int index) {
	ERR_FAIL_INDEX(index, _rects.size());

	rect_xywhf *r = _rects.get(index);

	r->atlas_texture.unref();
	r->atlas_texture.unref();

	memdelete(r);
}

void TexturePacker::remove_texture(Ref<Texture> texture) {
	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		if (r->original_texture == texture) {

			_rects.remove(i);

			r->atlas_texture.unref();
			r->atlas_texture.unref();

			memdelete(r);

			return;
		}
	}
}

int TexturePacker::get_texture_count() {
	return _rects.size();
}

void TexturePacker::clear() {
	_bins.clear();

	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		r->atlas_texture.unref();
		r->original_texture.unref();

		memdelete(r);
	}

	_rects.clear();

	for (int i = 0; i < _generated_textures.size(); ++i) {
		_generated_textures.get(i).unref();
	}

	_generated_textures.clear();
}

Ref<ImageTexture> TexturePacker::get_generated_texture(int index) {
	ERR_FAIL_INDEX_V(index, _generated_textures.size(), Ref<ImageTexture>());

	return _generated_textures.get(index);
}
int TexturePacker::get_generated_texture_count() {
	return _generated_textures.size();
}

void TexturePacker::merge() {
	_bins.clear();

	if (pack(_rects.ptr(), _rects.size(), _max_atlas_size, false, _bins)) {
		_generated_textures.clear();

		_generated_textures.resize(_bins.size());

		for (int i = 0; i < _bins.size(); ++i) {
			bin b = _bins[i];

			PoolByteArray data;
			data.resize(b.size.w * b.size.h * 4);

			// so it's transparent by default
			for (int j = 0; j < data.size(); ++j) {
				data.set(j, 0);
			}

			for (int j = 0; j < b.rects.size(); ++j) {
				rect_xywhf *r = b.rects[j];

				Ref<Texture> otext = r->original_texture;
				Ref<AtlasTexture> aotext = otext;

				int rect_pos_x = 0;
				int rect_pos_y = 0;
				int img_width = otext->get_size().x;

				if (aotext.is_valid()) {
					otext = aotext->get_atlas();

					Rect2 rect = aotext->get_region();

					rect_pos_x = rect.position.x + 0.5;
					rect_pos_y = rect.position.y + 0.5;
				}

				ERR_CONTINUE(!otext.is_valid());

				Ref<Image> img = otext->get_data();

				ERR_CONTINUE(!img.is_valid());

				PoolByteArray image_data = img->get_data();

				for (int y = 0; y < r->h; ++y) {
					int indx = (rect_pos_y + y) * img_width * 4 + rect_pos_x * 4;
					int start_indx = (r->y + y) * b.size.w * 4 + r->x * 4;

					int row_width = r->w * 4;
					for (int x = 0; x < row_width; ++x) {
						data.set(start_indx + x, image_data[indx + x]);
					}
				}
			}

			Ref<Image> image;
			image.instance();
			image->create(b.size.w, b.size.h, false, Image::FORMAT_RGBA8, data);

			Ref<ImageTexture> texture;
			texture.instance();
			texture->create_from_image(image, _texture_flags);

			_generated_textures.set(i, texture);

			for (int j = 0; j < b.rects.size(); ++j) {
				rect_xywhf *r = b.rects[j];

				Ref<AtlasTexture> at = r->atlas_texture;

				at->set_atlas(texture);
				at->set_region(Rect2(r->x, r->y, r->w, r->h));
			}
		}
	}
}

TexturePacker::TexturePacker() {
	_texture_flags = Texture::FLAGS_DEFAULT;
	_max_atlas_size = 1024;
	_keep_original_atlases = false;
}

TexturePacker::~TexturePacker() {
	clear();
}

void TexturePacker::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &TexturePacker::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &TexturePacker::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &TexturePacker::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &TexturePacker::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &TexturePacker::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &TexturePacker::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &TexturePacker::add_texture);
	ClassDB::bind_method(D_METHOD("get_texture", "index"), &TexturePacker::get_texture);
	ClassDB::bind_method(D_METHOD("get_original_texture", "index"), &TexturePacker::get_original_texture);

	ClassDB::bind_method(D_METHOD("unref_texture_index", "index"), &TexturePacker::unref_texture_index);
	ClassDB::bind_method(D_METHOD("unref_texture", "texture"), &TexturePacker::unref_texture);
	ClassDB::bind_method(D_METHOD("remove_texture_index", "index"), &TexturePacker::remove_texture_index);
	ClassDB::bind_method(D_METHOD("remove_texture", "texture"), &TexturePacker::remove_texture);

	ClassDB::bind_method(D_METHOD("get_texture_count"), &TexturePacker::get_texture_count);
	ClassDB::bind_method(D_METHOD("clear"), &TexturePacker::clear);

	ClassDB::bind_method(D_METHOD("get_generated_texture", "index"), &TexturePacker::get_generated_texture);
	ClassDB::bind_method(D_METHOD("get_generated_texture_count"), &TexturePacker::get_generated_texture_count);

	ClassDB::bind_method(D_METHOD("merge"), &TexturePacker::merge);
}
