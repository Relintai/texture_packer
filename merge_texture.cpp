#include "merge_texture.h"

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
}

void MergeTexture::_bind_methods() {
	ClassDB::bind_method(D_METHOD("test"), &MergeTexture::test);
}
