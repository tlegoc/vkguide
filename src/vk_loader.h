#pragma once

#include <vk_types.h>
#include <unordered_map>
#include <filesystem>

struct GeoSurface
{
	uint32_t startIndex;
	uint32_t count;
};

struct MeshAsset
{
	std::string name;

	std::vector<GeoSurface> surfaces;
	GPUMeshBuffers meshBuffers;
};

//forward declaration
class VulkanEngine;

std::optional<std::vector<std::shared_ptr<MeshAsset>>> loadGltfMeshes(VulkanEngine* engine,
	std::filesystem::path filePath);
// Added for convenience
void unloadGltfMesh(VulkanEngine* engine, MeshAsset* mesh);
void unloadGltfMeshes(VulkanEngine* engine, MeshAsset* meshes, uint16_t count);