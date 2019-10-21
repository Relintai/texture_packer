#include "texture_merger.h"

int TextureMerger::get_texture_flags() const {
	return _packer->get_texture_flags();
}
void TextureMerger::set_texture_flags(const int flags) {
	_packer->set_texture_flags(flags);
}

int TextureMerger::get_max_atlas_size() const {
	return _packer->get_max_atlas_size();
}
void TextureMerger::set_max_atlas_size(const int size) {
	_packer->set_max_atlas_size(size);
}

bool TextureMerger::get_keep_original_atlases() const {
	return _packer->get_keep_original_atlases();
}
void TextureMerger::set_keep_original_atlases(const bool value) {
	_packer->set_keep_original_atlases(value);
}

Color TextureMerger::get_background_color() const {
	return _packer->get_background_color();
}
void TextureMerger::set_background_color(const Color color) {
	_packer->set_background_color(color);
}

int TextureMerger::get_margin() const {
	return _packer->get_margin();
}
void TextureMerger::set_margin(const int margin){
	_packer->set_margin(margin);
}

Ref<TexturePacker> TextureMerger::get_packer() const {
	return _packer;
}
void TextureMerger::set_packer(const Ref<TexturePacker> packer) {
	_packer = packer;
}

Ref<AtlasTexture> TextureMerger::add_texture(Ref<Texture> texture) {
	return _packer->add_texture(texture);
}

Ref<Texture> TextureMerger::get_original_texture(int index) {
	return _packer->get_original_texture(index);
}

Ref<AtlasTexture> TextureMerger::get_texture(int index) {
	return _packer->get_texture(index);
}

void TextureMerger::unref_texture_index(int index) {
	_packer->unref_texture_index(index);
}

void TextureMerger::unref_texture(Ref<Texture> texture) {
	_packer->unref_texture(texture);
}

void TextureMerger::remove_texture_index(int index) {
	_packer->remove_texture_index(index);
}

void TextureMerger::remove_texture(Ref<Texture> texture) {
	_packer->remove_texture(texture);
}

int TextureMerger::get_texture_count() {
	return _packer->get_texture_count();
}

void TextureMerger::clear() {
	_packer->clear();
}

Ref<ImageTexture> TextureMerger::get_generated_texture(int index) {
	return _packer->get_generated_texture(index);
}
int TextureMerger::get_generated_texture_count() {
	return _packer->get_generated_texture_count();
}

void TextureMerger::merge() {
	_packer->merge();
}

TextureMerger::TextureMerger() {
	_packer.instance();
	_packer->set_keep_original_atlases(true);
}

TextureMerger::~TextureMerger() {
	_packer.unref();
}

void TextureMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &TextureMerger::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &TextureMerger::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &TextureMerger::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &TextureMerger::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &TextureMerger::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &TextureMerger::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &TextureMerger::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &TextureMerger::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &TextureMerger::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &TextureMerger::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");

	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &TextureMerger::add_texture);
	ClassDB::bind_method(D_METHOD("get_texture", "index"), &TextureMerger::get_texture);
	ClassDB::bind_method(D_METHOD("get_original_texture", "index"), &TextureMerger::get_original_texture);

	ClassDB::bind_method(D_METHOD("unref_texture_index", "index"), &TextureMerger::unref_texture_index);
	ClassDB::bind_method(D_METHOD("unref_texture", "texture"), &TextureMerger::unref_texture);
	ClassDB::bind_method(D_METHOD("remove_texture_index", "index"), &TextureMerger::remove_texture_index);
	ClassDB::bind_method(D_METHOD("remove_texture", "texture"), &TextureMerger::remove_texture);

	ClassDB::bind_method(D_METHOD("get_texture_count"), &TextureMerger::get_texture_count);
	ClassDB::bind_method(D_METHOD("clear"), &TextureMerger::clear);

	ClassDB::bind_method(D_METHOD("get_generated_texture", "index"), &TextureMerger::get_generated_texture);
	ClassDB::bind_method(D_METHOD("get_generated_texture_count"), &TextureMerger::get_generated_texture_count);

	ClassDB::bind_method(D_METHOD("merge"), &TextureMerger::merge);
}