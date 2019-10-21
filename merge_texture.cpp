#include "merge_texture.h"

Ref<AtlasTexture> MergeTexture::add_texture(Ref<Texture> texture) {

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

Ref<Texture> MergeTexture::get_original_texture(int index) {
	ERR_FAIL_INDEX_V(index, _rects.size(), Ref<Texture>());

	return _rects.get(index)->original_texture;
}

Ref<AtlasTexture> MergeTexture::get_texture(int index) {
	ERR_FAIL_INDEX_V(index, _rects.size(), Ref<AtlasTexture>());

	return _rects.get(index)->atlas_texture;
}

void MergeTexture::remove_texture_index(int index) {
	ERR_FAIL_INDEX(index, _rects.size());

	rect_xywhf *r = _rects.get(index);
	_rects.remove(index);

	r->atlas_texture.unref();
	r->atlas_texture.unref();

	memdelete(r);
}

void MergeTexture::remove_texture(Ref<Texture> texture) {

	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		if (r->original_texture == texture) {
			_rects.remove(i);
			return;
		}
	}
}

int MergeTexture::get_texture_count() {
	return _rects.size();
}

void MergeTexture::merge() {
	const int RECTS = 200;
	const bool ALLOW_FLIP = false;

	_bins.clear();
	_generated_textures.clear();

	if (pack(_rects.ptr(), _rects.size(), 400, ALLOW_FLIP, _bins)) {
		print_error("ok");

		for (int i = 0; i < _bins.size(); ++i) {
			

			bin b = _bins[i];

			PoolByteArray data;
			data.resize(b.size.w * b.size.h * 4);
			data.fill_with(0); // so it's transparent by default

			for (int j = 0; j < b.rects.size(); ++j) {
				rect_xywhf *r = b.rects[j];

				Ref<Texture> otext = r->original_texture;

				if (!otext.is_valid()) {
					print_error("MergeTexture->merge: !otext.is_valid()");
					continue;
				}

				Ref<Image> img = otext->get_data();

				if (!img.is_valid()) {
					print_error("MergeTexture->merge: !img.is_valid()");
					continue;
				}

				PoolByteArray image_data = img->get_data();

			}


			//Image(int p_width, int p_height, bool p_mipmaps, Format p_format, const PoolVector<uint8_t> &p_data);
			//Ref<Image> image;
			//image.instance();

			for (int j = 0; j < b.rects.size(); ++j) {
				rect_xywhf *r = b.rects[j];

				Ref<AtlasTexture> at = r->atlas_texture;

				//at->set_atlas(texture);
				at->set_region(Rect2(r->x, r->y, r->w, r->h));
			}
		}
	}
}

String MergeTexture::test() {
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

MergeTexture::MergeTexture() {
}

MergeTexture::~MergeTexture() {

	_bins.clear();

	for (int i = 0; i < _rects.size(); ++i) {
		rect_xywhf *r = _rects.get(i);

		r->atlas_texture.unref();
		r->original_texture.unref();

		memdelete(r);
	}

	_rects.clear();
}

void MergeTexture::_bind_methods() {
	ClassDB::bind_method(D_METHOD("test"), &MergeTexture::test);
}
