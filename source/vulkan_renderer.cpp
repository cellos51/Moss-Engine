#include "moss_renderer.hpp"

#include "moss_utility.hpp"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include <VkBootstrap.h>
#include <SDL_vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

// Public functions
bool VulkanRenderer::init(SDL_Window* window)
{
	this->window = window;

	if (!init_device()) { return false; }
	if (!create_swapchain()) { return false; }
	if (!get_queues()) { return false; }
	if (!create_graphics_pipeline()) { return false; }
	if (!create_command_pool()) { return false; }
    if (!create_command_buffers()) { return false; }
    if (!create_sync_objects()) { return false; }

	return true;
}

bool VulkanRenderer::draw()
{
    disp.waitForFences(1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

    uint32_t image_index = 0;
    disp.acquireNextImageKHR(swapchain, UINT64_MAX, available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

    if (image_in_flight[image_index] != VK_NULL_HANDLE) 
    {
        disp.waitForFences(1, &image_in_flight[image_index], VK_TRUE, UINT64_MAX);
    }
    image_in_flight[image_index] = in_flight_fences[current_frame];

    disp.resetCommandBuffer(command_buffers[image_index], 0);

    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (disp.beginCommandBuffer(command_buffers[image_index], &begin_info) != VK_SUCCESS) 
    {
        std::cout << "Failed to begin recording command buffer.\n";
        return false;
    }

    transition_image_layout(command_buffers[image_index], swapchain_images[image_index], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    draw_geometry(command_buffers[image_index], swapchain_image_views[image_index]);

    transition_image_layout(command_buffers[image_index], swapchain_images[image_index], VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    if (disp.endCommandBuffer(command_buffers[image_index]) != VK_SUCCESS) 
    {
        std::cout << "Failed to record command buffer.\n";
        return false;
    }

    disp.resetFences(1, &in_flight_fences[current_frame]);

    VkCommandBufferSubmitInfo command_buffer_info{};
    command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
	command_buffer_info.commandBuffer = command_buffers[image_index];

    VkSemaphoreSubmitInfo wait_semaphore_info{};
	wait_semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
	wait_semaphore_info.semaphore = available_semaphores[current_frame];
	wait_semaphore_info.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR;
	wait_semaphore_info.value = 1;

    VkSemaphoreSubmitInfo signal_semaphore_info{};
    signal_semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
	signal_semaphore_info.semaphore = finished_semaphore[current_frame];
	signal_semaphore_info.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
	signal_semaphore_info.value = 1;

    VkSubmitInfo2 submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submit_info.waitSemaphoreInfoCount = 1;
    submit_info.pWaitSemaphoreInfos = &wait_semaphore_info;
    submit_info.signalSemaphoreInfoCount = 1;
    submit_info.pSignalSemaphoreInfos = &signal_semaphore_info;
    submit_info.commandBufferInfoCount = 1;
    submit_info.pCommandBufferInfos = &command_buffer_info;

    if (disp.queueSubmit2(graphics_queue, 1, &submit_info, in_flight_fences[current_frame]) != VK_SUCCESS) 
    {
        std::cout << "Failed to submit draw command buffer.\n";
        return false;
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &finished_semaphore[current_frame];
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain.swapchain;
    present_info.pImageIndices = &image_index;    

    VkResult result = disp.queuePresentKHR(present_queue, &present_info);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) 
    {
        return recreate_swapchain();
    } 
    else if (result != VK_SUCCESS) 
    {
        std::cout << "Failed to present swapchain image.\n";
        return false;
    }

    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;

    return true;
}

void VulkanRenderer::cleanup()
{
    disp.deviceWaitIdle();

    vmaDestroyAllocator(allocator);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
        disp.destroySemaphore(finished_semaphore[i], nullptr);
        disp.destroySemaphore(available_semaphores[i], nullptr);
        disp.destroyFence(in_flight_fences[i], nullptr);
    }

    disp.destroyCommandPool(command_pool, nullptr);

    disp.destroyPipeline(graphics_pipeline, nullptr);
    disp.destroyPipelineLayout(pipeline_layout, nullptr);
    disp.destroyRenderPass(render_pass, nullptr);

    swapchain.destroy_image_views(swapchain_image_views);

    vkb::destroy_swapchain(swapchain);
    vkb::destroy_device(device);
    vkb::destroy_surface(instance, surface);
    vkb::destroy_instance(instance);
}

// Private functions
bool VulkanRenderer::init_device()
{
	vkb::InstanceBuilder instance_builder;
    auto instance_ret = instance_builder.set_app_name("Moss Engine")
						.set_engine_name("Moss Engine")
                        .request_validation_layers(enableValidationLayers)
                        .use_default_debug_messenger()
						.require_api_version(1, 3, 0)
                        .build();
    if (!instance_ret) 
	{
        std::cerr << "Failed to create Vulkan instance. Error: " << instance_ret.error().message() << "\n";
        return false;
    }
    instance = instance_ret.value();

	inst_disp = instance.make_table();

    if (SDL_Vulkan_CreateSurface(window, instance, &surface) != SDL_TRUE) 
	{
        std::cerr << "Failed to create Vulkan surface.\n";
        return false;
    }

	VkPhysicalDeviceVulkan13Features features13{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	features13.dynamicRendering = true;
	features13.synchronization2 = true;

	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;

	VkPhysicalDeviceVulkan11Features features11{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };
	// We don't need any features from Vulkan 1.1, so we don't need to set any fields in features11

	VkPhysicalDeviceFeatures features{};
	// Same as above, we don't need any features from Vulkan 1.0, so we don't need to set any fields in features

    vkb::PhysicalDeviceSelector phys_device_selector(instance);
    auto phys_device_ret = phys_device_selector.set_surface(surface)
                        .set_minimum_version(1, 3) // require a vulkan 1.3 capable device
						.set_required_features_13(features13)
						.set_required_features_12(features12)
						.set_required_features_11(features11)
						.set_required_features(features)
                        .require_dedicated_transfer_queue()
                        .select();
    if (!phys_device_ret) 
	{
        std::cerr << "Failed to select Vulkan physical device. Error: " << phys_device_ret.error().message() << "\n";
        return false;
    }
	vkb::PhysicalDevice physical_device = phys_device_ret.value();

    vkb::DeviceBuilder device_builder{ physical_device };
    auto device_ret  = device_builder.build();
    if (!device_ret ) 
	{
        std::cerr << "Failed to create Vulkan device. Error: " << device_ret .error().message() << "\n";
        return false;
    }
    device = device_ret.value();

	disp = device.make_table();

    VmaAllocatorCreateInfo allocator_info{};
    allocator_info.physicalDevice = physical_device.physical_device;
    allocator_info.device = device.device;
    allocator_info.instance = instance.instance;
    allocator_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    vmaCreateAllocator(&allocator_info, &allocator);

	return true;
}

bool VulkanRenderer::create_swapchain()
{
	vkb::SwapchainBuilder swapchain_builder{ device };
    auto swap_ret = swapchain_builder.set_old_swapchain(swapchain).set_desired_present_mode(VK_PRESENT_MODE_IMMEDIATE_KHR).build();
    if (!swap_ret) 
    {
        std::cerr << swap_ret.error().message() << " " << swap_ret.vk_result() << "\n";
        return false;
    }
    vkb::destroy_swapchain(swapchain);
    swapchain = swap_ret.value();
    swapchain_image_views = swapchain.get_image_views().value();
    swapchain_images = swapchain.get_images().value();
    return true;
}

bool VulkanRenderer::get_queues() 
{
    auto gq = device.get_queue(vkb::QueueType::graphics);
    if (!gq.has_value()) 
    {
        std::cerr << "Failed to get graphics queue: " << gq.error().message() << "\n";
        return false;
    }
    graphics_queue = gq.value();

    auto pq = device.get_queue(vkb::QueueType::present);
    if (!pq.has_value()) 
    {
        std::cerr << "Failed to get present queue: " << pq.error().message() << "\n";
        return false;
    }
    present_queue = pq.value();
    return true;
}

bool VulkanRenderer::create_graphics_pipeline() 
{
	VkPipelineRenderingCreateInfo rendering_info{};
	rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	rendering_info.colorAttachmentCount = 1;
	rendering_info.pColorAttachmentFormats = &swapchain.image_format;

	std::vector<char> vert_code;
	std::vector<char> frag_code;

	try
	{
		vert_code = readFile("assets/shaders/default_vert.spv");
    	frag_code = readFile("assets/shaders/default_frag.spv");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}

    VkShaderModule vert_module = createShaderModule(vert_code);
    VkShaderModule frag_module = createShaderModule(frag_code);
    if (vert_module == VK_NULL_HANDLE || frag_module == VK_NULL_HANDLE) 
	{
        std::cerr << "Failed to create shader module.\n";
        return false;
    }

    VkPipelineShaderStageCreateInfo vert_stage_info{};
    vert_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_stage_info.module = vert_module;
    vert_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_stage_info{};
    frag_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_stage_info.module = frag_module;
    frag_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = { vert_stage_info, frag_stage_info };

    VkPipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = 0;
    vertex_input_info.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo input_assembly{};
    input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapchain.extent.width;
    viewport.height = (float)swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapchain.extent;

    VkPipelineViewportStateCreateInfo viewport_state{};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo color_blending{};
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = VK_FALSE;
    color_blending.logicOp = VK_LOGIC_OP_COPY;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &colorBlendAttachment;
    color_blending.blendConstants[0] = 0.0f;
    color_blending.blendConstants[1] = 0.0f;
    color_blending.blendConstants[2] = 0.0f;
    color_blending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 0;
    pipeline_layout_info.pushConstantRangeCount = 0;

    if (disp.createPipelineLayout(&pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS) 
	{
        std::cerr << "Failed to create pipeline layout.\n";
        return false;
    }

    std::vector<VkDynamicState> dynamic_states = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamic_info{};
    dynamic_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dynamic_info.pDynamicStates = dynamic_states.data();

    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.pNext = &rendering_info;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.pDynamicState = &dynamic_info;
    pipeline_info.layout = pipeline_layout;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

    if (disp.createGraphicsPipelines(VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_pipeline) != VK_SUCCESS) 
    {
        std::cerr << "Failed to create pipline.\n";
        return false;
    }

    disp.destroyShaderModule(frag_module, nullptr);
    disp.destroyShaderModule(vert_module, nullptr);
    return true;
}

bool VulkanRenderer::create_command_pool() 
{
    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = device.get_queue_index(vkb::QueueType::graphics).value();
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (disp.createCommandPool(&pool_info, nullptr, &command_pool) != VK_SUCCESS) 
	{
        std::cerr << "Failed to create command pool.\n";
        return false;
    }
    return true;
}

bool VulkanRenderer::create_command_buffers() 
{
    command_buffers.resize(swapchain_image_views.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)command_buffers.size();

    if (disp.allocateCommandBuffers(&allocInfo, command_buffers.data()) != VK_SUCCESS) 
	{
        std::cerr << "Failed to allocate command buffers.\n";
        return false;
    }

    return true;
}

bool VulkanRenderer::create_sync_objects()
{
    available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    finished_semaphore.resize(MAX_FRAMES_IN_FLIGHT);
    in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
    image_in_flight.resize(swapchain.image_count, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
    {
        if (disp.createSemaphore(&semaphore_info, nullptr, &available_semaphores[i]) != VK_SUCCESS ||
            disp.createSemaphore(&semaphore_info, nullptr, &finished_semaphore[i]) != VK_SUCCESS ||
            disp.createFence(&fence_info, nullptr, &in_flight_fences[i]) != VK_SUCCESS) 
            {
            std::cout << "Failed to create sync objects.\n";
            return false;
        }
    }
    return true;
}

// Drawing functions
void VulkanRenderer::draw_geometry(VkCommandBuffer command_buffer, VkImageView image_view)
{
    VkClearValue clear_value = {{ 0.0f, 0.0f, 0.0f, 1.0f }};

    VkRenderingAttachmentInfo color_attachment_info{};
    color_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    color_attachment_info.imageView = image_view;
    color_attachment_info.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    color_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_info.clearValue = clear_value;

    VkRenderingInfo rendering_info{};
    rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
    rendering_info.pNext = VK_NULL_HANDLE;
    rendering_info.flags = 0;
    rendering_info.renderArea = VkRect2D{VkOffset2D{}, swapchain.extent};
    rendering_info.layerCount = 1;
    rendering_info.viewMask = 0;
    rendering_info.colorAttachmentCount = 1;
    rendering_info.pColorAttachments = &color_attachment_info;
    rendering_info.pDepthAttachment = VK_NULL_HANDLE;
    rendering_info.pStencilAttachment = VK_NULL_HANDLE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapchain.extent.width;
    viewport.height = (float)swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapchain.extent;

    disp.cmdSetViewport(command_buffer, 0, 1, &viewport);
    disp.cmdSetScissor(command_buffer, 0, 1, &scissor);

    disp.cmdBeginRendering(command_buffer, &rendering_info);

    disp.cmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

    disp.cmdDraw(command_buffer, 3, 1, 0, 0);

    disp.cmdEndRendering(command_buffer);
}

// Helper functions
bool VulkanRenderer::recreate_swapchain()
{
    disp.deviceWaitIdle();

    disp.destroyCommandPool(command_pool, nullptr);

    swapchain.destroy_image_views(swapchain_image_views);

    if (!create_swapchain()) return false;
    if (!create_command_pool()) return false;
    if (!create_command_buffers()) return false;

    return true;
}

VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (disp.createShaderModule(&create_info, nullptr, &shaderModule) != VK_SUCCESS) 
	{
        std::cerr << "Failed to create shader module.\n";
        return VK_NULL_HANDLE;
    }

    return shaderModule;
}

void VulkanRenderer::transition_image_layout(VkCommandBuffer command_buffer, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout)
{
    VkImageMemoryBarrier2 barrier
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = nullptr,
        .srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT | VK_ACCESS_2_MEMORY_WRITE_BIT,     
        .oldLayout = old_layout,
        .newLayout = new_layout,
        .image = image,
        .subresourceRange 
        {
            .aspectMask = (new_layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = VK_REMAINING_MIP_LEVELS,
            .baseArrayLayer = 0,
            .layerCount = VK_REMAINING_ARRAY_LAYERS
        }
    };

    VkDependencyInfo dependency_info
    {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nullptr,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &barrier
    };

    disp.cmdPipelineBarrier2(command_buffer, &dependency_info);
}