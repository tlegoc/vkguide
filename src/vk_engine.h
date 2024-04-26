// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>
#include <utils.h>
#include <vk_descriptors.h>

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
	VkExtent2D _drawExtent;

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
	// ---

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

	//run main loop
	void run();

private:
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();
	void init_descriptors();
	void init_pipelines();
	void init_background_pipelines();

	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();
};
