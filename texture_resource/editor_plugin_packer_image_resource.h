
#ifndef EDITOR_PLUGIN_PACKER_IMAGE_RESOURCE_H
#define EDITOR_PLUGIN_PACKER_IMAGE_RESOURCE_H

#include "editor/editor_plugin.h"
#include "core/ustring.h"

#include "packer_image_resource_importer.h"

class EditorPluginPackerImageResource : public EditorPlugin {

	GDCLASS(EditorPluginPackerImageResource, EditorPlugin);

public:
	EditorPluginPackerImageResource(EditorNode *node);

protected:
	void _notification(int p_what);

private:
	EditorNode *_node;
	Ref<PackerImageResourceImporter> _importer;
};

#endif
