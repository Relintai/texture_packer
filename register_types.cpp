#include "register_types.h"

#include "texture_packer.h"
#include "texture_merger.h"

void register_texture_packer_types() {
	ClassDB::register_class<TexturePacker>();
	ClassDB::register_class<TextureMerger>();
}	

void unregister_texture_packer_types() {

}

