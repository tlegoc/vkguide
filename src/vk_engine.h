// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>
#include <utils.h>
#include <vk_descriptors.h>
#include <vk_loader.h>

struct FrameData
{
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;

	// NOTE : Wait for swapchain image to be ready
	VkSemaphore _swapchainSemaphore, _renderSemaphore;
	// NOTE : Wait for rendering to be finished
	VkFence _renderFence;

	DeletionQueue _deletionQueue;
};

constexpr unsigned int FRAME_OVERLAP = 2;

// Used for tutorial, TODO Remove
struct ComputePushConstants
{
	glm::vec4 data1;
	glm::vec4 data2;
	glm::vec4 data3;
	glm::vec4 data4;
};

struct ComputeEffect
{
	const char* name;

	VkPipeline pipeline;
	VkPipelineLayout layout;

	ComputePushConstants data;
};

class VulkanEngine
{
public:

	// Instance
	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkPhysicalDevice _physicalDevice;
	VkDevice _device;
	VkSurfaceKHR _surface;

	// Swapchain
	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;
	bool m_resize_requested;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

	FrameData _frames[FRAME_OVERLAP];

	FrameData& get_current_frame()
	{
		return _frames[_frameNumber % FRAME_OVERLAP];
	};

	//draw resources
	AllocatedImage _drawImage;
	AllocatedImage _depthImage;
	VkExtent2D _drawExtent;
	float m_renderScale = 1.f;

	// Queue
	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	// VMA
	VmaAllocator _allocator;

	// Tutorial compute shader related, TODO Remove ?
	DescriptorAllocator globalDescriptorAllocator;
	VkDescriptorSet _drawImageDescriptors;
	VkDescriptorSetLayout _drawImageDescriptorLayout;
	VkPipeline _gradientPipeline;
	VkPipelineLayout _gradientPipelineLayout;
	std::vector<ComputeEffect> backgroundEffects;
	int currentBackgroundEffect{ 0 };

	// ---

	// immediate submit structures (also used for imgui)
	VkFence _immFence;
	VkCommandBuffer _immCommandBuffer;
	VkCommandPool _immCommandPool;

	// Engine data
	bool _isInitialized{ false };
	int _frameNumber{ 0 };
	bool stop_rendering{ false };
	VkExtent2D _windowExtent{ 1700, 900 };

	struct SDL_Window* _window{ nullptr };

	DeletionQueue _mainDeletionQueue;

	static VulkanEngine& Get();

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	void draw_background(VkCommandBuffer cmd);

	void draw_geometry(VkCommandBuffer cmd);

	void draw_imgui(VkCommandBuffer cmd, VkImageView targetImageView);

	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

	//run main loop
	void run();

	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

	void destroy_buffer(const AllocatedBuffer& buffer);

	GPUMeshBuffers uploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

	void destroyMesh(GPUMeshBuffers* meshBuffer);

private:
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();
	void init_descriptors();
	void init_pipelines();
	void init_background_pipelines();
	void init_imgui();

	VkPipelineLayout _trianglePipelineLayout;
	VkPipeline _trianglePipeline;

	void init_triangle_pipeline();

	VkPipelineLayout _meshPipelineLayout;
	VkPipeline _meshPipeline;

	GPUMeshBuffers rectangle;

	void init_mesh_pipeline();

	void init_default_data();

	std::vector<std::shared_ptr<MeshAsset>> testMeshes;

	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();
	void resize_swapchain();
};
