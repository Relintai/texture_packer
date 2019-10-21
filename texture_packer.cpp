#include "texture_packer.h"

Ref<AtlasTexture> TexturePacker::add_texture(Ref<Texture> texture) {

	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		if (r->original_texture == texture)
			return r->atlas_texture;
	}

	Ref<AtlasTexture> tex;
	tex.instance();
	//Temp setup, so the texture is usable even while the atlases are generating.
	tex->set_atlas(texture);
	tex->set_region(Rect2(0, 0, texture->get_width(), texture->get_height()));

	rect_xywhf *rect = memnew(rect_xywhf);

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

void TexturePacker::remove_texture_index(int index) {
	ERR_FAIL_INDEX(index, _rects.size());

	rect_xywhf *r = _rects.get(index);
	_rects.remove(index);

	r->atlas_texture.unref();
	r->atlas_texture.unref();

	memdelete(r);
}

void TexturePacker::remove_texture(Ref<Texture> texture) {

	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		if (r->original_texture == texture) {
			_rects.remove(i);
			return;
		}
	}
}

int TexturePacker::get_texture_count() {
	return _rects.size();
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

	if (pack(_rects.ptr(), _rects.size(), 1024, false, _bins)) {
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

				ERR_CONTINUE(!otext.is_valid());

				Ref<Image> img = otext->get_data();

				ERR_CONTINUE(!img.is_valid());

				PoolByteArray image_data = img->get_data();

				int indx = 0;
				for (int y = 0; y < r->h; ++y) {
					int start_indx = r->y + y * (b.size.w) * 4 + (r->x * 4);

					int row_width = r->w * 4;
					for (int x = 0; x < row_width; ++x) {
						data.set(start_indx + x, image_data[indx]);
						++indx;
					}
				}
			}

			Ref<Image> image;
			image.instance();
			image->create(b.size.w, b.size.h, false, Image::FORMAT_RGBA8, data);

			Ref<ImageTexture> texture;
			texture.instance();
			texture->create_from_image(image, 0);

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

String TexturePacker::test() {
	String res = "";

	const int RECTS = 200;
	const bool ALLOW_FLIP = false;

	rect_xywhf rects[RECTS], *ptr_rects[RECTS];

	for (int i = 0; i < RECTS; ++i) {
		rects[i] = rect_xywhf(0, 0, 7 + rand() % 50, 7 + rand() % 50);
		ptr_rects[i] = rects + i;
	}

	std::vector<bin> bins;

	if (pack(ptr_rects, RECTS, 400, ALLOW_FLIP, bins)) {
		res += "bins: " + String::num(bins.size()) + "\n";

		for (int i = 0; i < bins.size(); ++i) {
			res += "bin: " + String::num(i) + ", rects: " + String::num(bins[i].size.w) + " " + String::num(bins[i].size.h) + " " + String::num(bins[i].rects.size()) + "\n";

			for (int r = 0; r < bins[i].rects.size(); ++r) {
				rect_xywhf *rect = bins[i].rects[r];

				res += "rect " + String::num(r) + ": x: " + String::num(rect->x) + ", y: " + String::num(rect->y) + ", w: " + String::num(rect->w) + ", h: " + String::num(rect->h) + ", was flipped: " + (rect->flipped ? "yes" : " no") + "\n";
			}
		}
	} else {
		res += "failed: there's a rectangle with width/height bigger than max_size!\n";
	}

	return res;
}

TexturePacker::TexturePacker() {
}

TexturePacker::~TexturePacker() {

	_bins.clear();

	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		r->atlas_texture.unref();
		r->original_texture.unref();

		memdelete(r);
	}

	_rects.clear();
}

void TexturePacker::_bind_methods() {
	ClassDB::bind_method(D_METHOD("test"), &TexturePacker::test);

	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &TexturePacker::add_texture);
	ClassDB::bind_method(D_METHOD("get_texture", "index"), &TexturePacker::get_texture);
	ClassDB::bind_method(D_METHOD("get_original_texture", "index"), &TexturePacker::get_original_texture);
	ClassDB::bind_method(D_METHOD("remove_texture_index", "index"), &TexturePacker::remove_texture_index);
	ClassDB::bind_method(D_METHOD("remove_texture", "texture"), &TexturePacker::remove_texture);
	ClassDB::bind_method(D_METHOD("get_texture_count"), &TexturePacker::get_texture_count);

	ClassDB::bind_method(D_METHOD("get_generated_texture", "index"), &TexturePacker::get_generated_texture);
	ClassDB::bind_method(D_METHOD("get_generated_texture_count"), &TexturePacker::get_generated_texture_count);

	ClassDB::bind_method(D_METHOD("merge"), &TexturePacker::merge);
}
