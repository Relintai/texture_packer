#include "packer_image_resource_importer.h"

String PackerImageResourceImporter::get_importer_name() const {
	return "packer_image_resource";
}

String PackerImageResourceImporter::get_visible_name() const {
	return "Packer Image Resource";
}

void PackerImageResourceImporter::get_recognized_extensions(List<String> *p_extensions) const {
	ImageLoader::get_recognized_extensions(p_extensions);
}

String PackerImageResourceImporter::get_save_extension() const {
	return "tres";
}

String PackerImageResourceImporter::get_resource_type() const {
	return "PackerImageResource";
}

float PackerImageResourceImporter::get_priority() const {
	return 1.0;
}

int PackerImageResourceImporter::get_preset_count() const {
	return 0;
}

String PackerImageResourceImporter::get_preset_name(int p_idx) const {
	return "";
}

void PackerImageResourceImporter::get_import_options(List<ImportOption> *r_options, int p_preset) const {
}

bool PackerImageResourceImporter::get_option_visibility(const String &p_option, const Map<StringName, Variant> &p_options) const {
	return true;
}

Error PackerImageResourceImporter::import(const String &p_source_file, const String &p_save_path, const Map<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata) {

	/*
	Ref<Image> image;
	image.instance();
	Error err = ImageLoader::load_image(p_source_file, image, NULL, hdr_as_srgb, scale);

	if (err != OK)
		return err;
		*/

	return Error::FAILED;
}

PackerImageResourceImporter::PackerImageResourceImporter() {
}

PackerImageResourceImporter::~PackerImageResourceImporter() {
}
