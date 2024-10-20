#include "moss_renderer.hpp"

#include "util.hpp"
#include "mesh.hpp"

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

const int MAX_FRAMES_IN_FLIGHT = 3;
const int MAX_INSTANCES = 1000;

// Public functions
bool VulkanRenderer::init(SDL_Window* window)
{
	this->window = window;

	if (!init_device()) { return false; }
    if (!get_queues()) { return false; }
    if (!create_command_pool()) { return false; }
	if (!create_swapchain()) { return false; }
    if (!prepare_images()) { return false; }
    if (!create_mesh_buffers()) { return false; }
    if (!create_uniform_buffers()) { return false; }
    if (!create_descriptor_sets()) { return false; }
    if (!create_command_buffers()) { return false; }
    if (!create_sync_objects()) { return false; }
    if (!create_graphics_pipeline()) { return false; }

	return true;
}

void VulkanRenderer::drawEntity(Entity* entity)
{
    entities.push_back(entity);
}

bool VulkanRenderer::drawFrame()
{
    disp.waitForFences(1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

    uint32_t image_index = 0;
    VkResult result = disp.acquireNextImageKHR(swapchain, UINT64_MAX, available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return recreateSwapchain();
    } 
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        std::cerr << "Failed to acquire swapchain image. Error: " << result << "\n";
        return false;
    }

    if (image_in_flight[image_index] != VK_NULL_HANDLE) 
    {
        disp.waitForFences(1, &image_in_flight[image_index], VK_TRUE, UINT64_MAX);
    }
    image_in_flight[image_index] = in_flight_fences[current_frame];

    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (disp.beginCommandBuffer(command_buffers[image_index], &begin_info) != VK_SUCCESS) 
    {
        std::cerr << "Failed to begin recording command buffer.\n";
        return false;
    }

    transitionImageLayout(command_buffers[image_index], swapchain_images[image_index], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    draw_geometry(command_buffers[image_index], swapchain_image_views[image_index]);

    transitionImageLayout(command_buffers[image_index], swapchain_images[image_index], VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    if (disp.endCommandBuffer(command_buffers[image_index]) != VK_SUCCESS) 
    {
        std::cerr << "Failed to record command buffer.\n";
        return false;
    }

    entities.clear();

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

    disp.resetFences(1, &in_flight_fences[current_frame]);

    if (disp.queueSubmit2(graphics_queue, 1, &submit_info, in_flight_fences[current_frame]) != VK_SUCCESS) 
    {
        std::cerr << "Failed to submit draw command buffer.\n";
        return false;
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &finished_semaphore[current_frame];
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain.swapchain;
    present_info.pImageIndices = &image_index;    

    result = disp.queuePresentKHR(present_queue, &present_info);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) 
    {
        return recreateSwapchain();
    } 
    else if (result != VK_SUCCESS) 
    {
        std::cerr << "Failed to present swapchain image.\n";
        return false;
    }

    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;

    return true;
}

void VulkanRenderer::cleanup()
{
    disp.deviceWaitIdle();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
        disp.destroySemaphore(finished_semaphore[i], nullptr);
        disp.destroySemaphore(available_semaphores[i], nullptr);
        disp.destroyFence(in_flight_fences[i], nullptr);
        destroyBuffer(uniform_buffers[i]);
    }

    destroyBuffer(vertex_buffer);
    destroyBuffer(index_buffer);
    destroyImage(depth_image);
    vmaDestroyAllocator(allocator);

    disp.destroyCommandPool(command_pool, nullptr);

    disp.destroyDescriptorSetLayout(descriptor_set_layout, nullptr);
    disp.destroyDescriptorPool(descriptor_pool, nullptr);
    disp.destroyPipeline(graphics_pipeline, nullptr);
    disp.destroyPipelineLayout(pipeline_layout, nullptr);

    swapchain.destroy_image_views(swapchain_image_views);
    destroyImageView(depth_image);

    vkb::destroy_swapchain(swapchain);
    vkb::destroy_device(device);
    vkb::destroy_surface(instance, surface);
    vkb::destroy_instance(instance);
}

// Initialization functions
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

	VkPhysicalDeviceVulkan11Features features11{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };

	VkPhysicalDeviceFeatures features{};

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
    vmaCreateAllocator(&allocator_info, &allocator);

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

bool VulkanRenderer::create_swapchain()
{
	vkb::SwapchainBuilder swapchain_builder{ device };
    auto swap_ret = swapchain_builder.set_old_swapchain(swapchain)
                        .set_desired_present_mode(VK_PRESENT_MODE_IMMEDIATE_KHR)
                        .set_desired_min_image_count(swapchain_builder.DOUBLE_BUFFERING)
                        .build();
    if (!swap_ret) 
    {
        std::cerr << swap_ret.error().message() << " " << swap_ret.vk_result() << "\n";
        return false;
    }
    vkb::destroy_swapchain(swapchain);
    swapchain = swap_ret.value();
    swapchain_image_views = swapchain.get_image_views().value();
    swapchain_images = swapchain.get_images().value();

    VkResult result = createImage(swapchain.extent.width, 
                        swapchain.extent.height, 
                        VK_FORMAT_D32_SFLOAT, 
                        VK_IMAGE_TILING_OPTIMAL, 
                        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                        VMA_MEMORY_USAGE_GPU_ONLY, 
                        depth_image);
    if (result != VK_SUCCESS) 
    {
        std::cerr << "Failed to create depth image.\n";
        return false;
    }

    result = createImageView(VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT, depth_image);
    if (result != VK_SUCCESS) 
    {
        std::cerr << "Failed to create depth image view.\n";
        return false;
    }

    return true;
}

bool VulkanRenderer::prepare_images()
{
    VkCommandBuffer command_buffer;
    if (!beginSingleTimeCommands(command_buffer)) { return false; }

    for (size_t i = 0; i < swapchain_images.size(); i++) 
    {
        transitionImageLayout(command_buffer, swapchain_images[i], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    }

    transitionImageLayout(command_buffer, depth_image.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

    if (!endSingleTimeCommands(command_buffer, graphics_queue)) { return false; }

    return true;
}

bool VulkanRenderer::create_mesh_buffers() 
{
    std::vector<Mesh> meshes;

    //meshes.push_back(mesh::createSquare());
    meshes.push_back(mesh::loadGltf("assets/models/test.glb"));
    //meshes.push_back(mesh::loadGltf("assets/models/torus.glb"));
    //meshes.push_back(mesh::loadGltf("assets/models/cube.glb"));
    //meshes.push_back(mesh::loadGltf("assets/models/icosphere.glb"));
    //meshes.push_back(mesh::loadGltf("assets/models/cylinder.glb"));
    //meshes.push_back(mesh::loadGltf("assets/models/cone.glb"));
    //meshes.push_back(mesh::loadGltf("assets/models/uvsphere.glb"));
    
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    
    size_t vertex_offset = 0;

    // Push all vertices and indices into a single mesh
    for (size_t i = 0; i < meshes.size(); i++)
    {
        vertices.insert(vertices.end(), meshes[i].vertices.begin(), meshes[i].vertices.end());
        
        size_t current_index_offset = indices.size();

        for (Index index : meshes[i].indices) 
        {
            indices.push_back(index + vertex_offset);
        }

        MeshRegion region;
        region.index_offset = current_index_offset * sizeof(Index);
        region.index_count = meshes[i].indices.size();

        mesh_regions.insert({ i, region });

        vertex_offset += meshes[i].vertices.size();
    }

    // Destroy previous buffers
    if (vertex_buffer.buffer != VK_NULL_HANDLE) { destroyBuffer(vertex_buffer); }
    if (index_buffer.buffer != VK_NULL_HANDLE) { destroyBuffer(index_buffer); }

    VkDeviceSize vertex_buffer_size = sizeof(Vertex) * vertices.size();
    VkDeviceSize index_buffer_size = sizeof(Vertex) * indices.size();

    // Create vertex buffer
    VkResult result = createBuffer(vertex_buffer_size, 
                    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                    VMA_MEMORY_USAGE_GPU_ONLY, vertex_buffer);
    if (result != VK_SUCCESS) 
    {
        std::cerr << "Failed to create vertex buffer.\n";
        return false;
    }

    // Create index buffer
    result = createBuffer(index_buffer_size, 
                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                    VMA_MEMORY_USAGE_GPU_ONLY, index_buffer);
    if (result != VK_SUCCESS)
    {
        std::cerr << "Failed to create index buffer.\n";
        return false;
    }

    // Create staging buffer
    AllocatedBuffer staging_buffer;
    result = createBuffer(vertex_buffer_size + index_buffer_size, 
                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                    VMA_MEMORY_USAGE_CPU_ONLY, staging_buffer);
    if (result != VK_SUCCESS)
    {
        std::cerr << "Failed to create staging buffer.\n";
        return false;
    }

    // Copy data to staging buffer
    void* data;
    vmaMapMemory(allocator, staging_buffer.allocation, &data);
    memcpy(data, vertices.data(), vertex_buffer_size);
    memcpy(reinterpret_cast<char*>(data) + vertex_buffer_size, indices.data(), index_buffer_size);
    vmaUnmapMemory(allocator, staging_buffer.allocation);

    // Copy data to vertex and index buffers
    VkCommandBuffer command_buffer;
    if (!beginSingleTimeCommands(command_buffer)) { return false; }

    VkBufferCopy copy_region{};
    copy_region.dstOffset = 0;
    copy_region.srcOffset = 0;
    copy_region.size = vertex_buffer_size;

    disp.cmdCopyBuffer(command_buffer, staging_buffer.buffer, vertex_buffer.buffer, 1, &copy_region);

    copy_region.dstOffset = 0;
    copy_region.srcOffset = vertex_buffer_size;
    copy_region.size = index_buffer_size;

    disp.cmdCopyBuffer(command_buffer, staging_buffer.buffer, index_buffer.buffer, 1, &copy_region);

    if (!endSingleTimeCommands(command_buffer, graphics_queue)) { return false; }

    destroyBuffer(staging_buffer);

    return true;
}

bool VulkanRenderer::create_uniform_buffers()
{
    dynamic_alignment = sizeof(UniformBufferObject);
    VkDeviceSize  min_ubo_alignment = device.physical_device.properties.limits.minUniformBufferOffsetAlignment;
    if (min_ubo_alignment > 0) 
    {
        dynamic_alignment = (min_ubo_alignment + dynamic_alignment - 1) & ~(min_ubo_alignment - 1);
    }

    VkDeviceSize buffer_size = dynamic_alignment * MAX_INSTANCES;

    uniform_buffers.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkResult result = createBuffer(buffer_size, 
                        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                        VMA_MEMORY_USAGE_CPU_TO_GPU, uniform_buffers[i]);
        if (result != VK_SUCCESS)
        {
            std::cerr << "Failed to create uniform buffer.\n";
            return false;
        }
    }

    return true;
}

bool VulkanRenderer::create_descriptor_sets()
{
    VkDescriptorSetLayoutBinding ubo_layout_binding{};
    ubo_layout_binding.binding = 0;
    ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    ubo_layout_binding.descriptorCount = 1;
    ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    ubo_layout_binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;
    layout_info.pBindings = &ubo_layout_binding;

    if (disp.createDescriptorSetLayout(&layout_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
    {
        std::cerr << "Failed to create descriptor set layout.\n";
        return false;
    }

    VkDescriptorPoolSize pool_size{};
    pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    pool_size.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes = &pool_size;
    pool_info.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (disp.createDescriptorPool(&pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
    {
        std::cerr << "Failed to create descriptor pool.\n";
        return false;
    }

    // putting the descriptor sets here for now
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptor_set_layout);

    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = descriptor_pool;
    alloc_info.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    alloc_info.pSetLayouts = layouts.data();

    descriptor_sets.resize(MAX_FRAMES_IN_FLIGHT);

    if (disp.allocateDescriptorSets(&alloc_info, descriptor_sets.data()) != VK_SUCCESS)
    {
        std::cerr << "Failed to allocate descriptor sets.\n";
        return false;
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
    {
        VkDescriptorBufferInfo buffer_info{};
        buffer_info.buffer = uniform_buffers[i].buffer;
        buffer_info.offset = 0;
        buffer_info.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = descriptor_sets[i];
        descriptor_write.dstBinding = 0;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptor_write.descriptorCount = 1;
        descriptor_write.pBufferInfo = &buffer_info;

        disp.updateDescriptorSets(1, &descriptor_write, 0, nullptr);
    }

    return true;
}

bool VulkanRenderer::create_command_buffers() 
{
    command_buffers.resize(swapchain_image_views.size());

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = (uint32_t)command_buffers.size();

    if (disp.allocateCommandBuffers(&alloc_info, command_buffers.data()) != VK_SUCCESS) 
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
            std::cerr << "Failed to create sync objects.\n";
            return false;
        }
    }
    return true;
}

bool VulkanRenderer::create_graphics_pipeline() 
{
    VkPipelineRenderingCreateInfo rendering_info{};
	rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	rendering_info.colorAttachmentCount = 1;
	rendering_info.pColorAttachmentFormats = &swapchain.image_format;
    rendering_info.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;

	std::vector<char> vert_code;
	std::vector<char> frag_code;

	try
	{
		vert_code = util::readFile("assets/shaders/default_vert.spv");
    	frag_code = util::readFile("assets/shaders/default_frag.spv");
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

    auto bindingDescription = getBindingDescription();
    auto attributeDescriptions = getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertex_input_info.pVertexBindingDescriptions = &bindingDescription;
    vertex_input_info.pVertexAttributeDescriptions = attributeDescriptions.data();

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
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depth_stencil{};
    depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil.depthTestEnable = VK_TRUE;
    depth_stencil.depthWriteEnable = VK_TRUE;
    depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depth_stencil.depthBoundsTestEnable = VK_FALSE;
    depth_stencil.minDepthBounds = 0.0f;
    depth_stencil.maxDepthBounds = 1.0f;
    depth_stencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState color_blend_attachment{};
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo color_blending{};
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = VK_FALSE;
    color_blending.logicOp = VK_LOGIC_OP_COPY;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &color_blend_attachment;
    color_blending.blendConstants[0] = 0.0f;
    color_blending.blendConstants[1] = 0.0f;
    color_blending.blendConstants[2] = 0.0f;
    color_blending.blendConstants[3] = 0.0f;

    VkPushConstantRange push_constant_range{};
    push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    push_constant_range.offset = 0;
    push_constant_range.size = sizeof(glm::mat4) * 2; // View and projection matrices

    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 1;
    pipeline_layout_info.pSetLayouts = &descriptor_set_layout;
    pipeline_layout_info.pushConstantRangeCount = 1;
    pipeline_layout_info.pPushConstantRanges = &push_constant_range;

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
    pipeline_info.pDepthStencilState = &depth_stencil;
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

    VkRenderingAttachmentInfo depth_attachment_info{};
    depth_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    depth_attachment_info.imageView = depth_image.image_view;
    depth_attachment_info.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    depth_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment_info.clearValue = VkClearValue{ 1.0f, 0 };

    VkRenderingInfo rendering_info{};
    rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
    rendering_info.pNext = VK_NULL_HANDLE;
    rendering_info.flags = 0;
    rendering_info.renderArea = VkRect2D{VkOffset2D{}, swapchain.extent};
    rendering_info.layerCount = 1;
    rendering_info.viewMask = 0;
    rendering_info.colorAttachmentCount = 1;
    rendering_info.pColorAttachments = &color_attachment_info;
    rendering_info.pDepthAttachment = &depth_attachment_info;
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

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), swapchain.extent.width / (float)swapchain.extent.height, 0.1f, 10.0f);
    projection[1][1] *= -1; // Flip the y-axis

    disp.cmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &view);
    disp.cmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), sizeof(glm::mat4), &projection);

    VkDeviceSize buffer_offset = 0;
    disp.cmdBindVertexBuffers(command_buffer, 0, 1, &vertex_buffer.buffer, &buffer_offset);

    size_t mesh_index = 0;

    std::vector<UniformBufferObject> ubos;
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (i >= MAX_INSTANCES) { break; }

        UniformBufferObject ubo{};
        ubo.model = entities[i]->transform.getMatrix();
        ubos.push_back(ubo);

        if (mesh_regions.find(mesh_index) != mesh_regions.end())
        {
            UniformBufferObject ubo{};
            ubo.model = entities[i]->transform.getMatrix();
            
            uint32_t dynamic_offset = i * dynamic_alignment;
            disp.cmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_sets[current_frame], 1, &dynamic_offset);

            disp.cmdBindIndexBuffer(command_buffer, index_buffer.buffer, mesh_regions[mesh_index].index_offset, VK_INDEX_TYPE_UINT32);
            disp.cmdDrawIndexed(command_buffer, mesh_regions[mesh_index].index_count, 1, 0, 0, 0);
        }
    }

    updateDynamicUniformBuffer(ubos);

    disp.cmdEndRendering(command_buffer);
}

// Helper functions
void VulkanRenderer::updateUniformBuffer(UniformBufferObject &ubo)
{
    void* data = nullptr;
    vmaMapMemory(allocator, uniform_buffers[current_frame].allocation, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vmaUnmapMemory(allocator, uniform_buffers[current_frame].allocation);
}

void VulkanRenderer::updateDynamicUniformBuffer(std::vector<UniformBufferObject>& ubos)
{
    void* data;
    vmaMapMemory(allocator, uniform_buffers[current_frame].allocation, &data);

    for (size_t i = 0; i < ubos.size(); i++) 
    {
        void* dst = (char*)data + i * dynamic_alignment;
        memcpy(dst, &ubos[i], sizeof(UniformBufferObject));
    }

    vmaUnmapMemory(allocator, uniform_buffers[current_frame].allocation);
}

bool VulkanRenderer::recreateSwapchain()
{
    disp.deviceWaitIdle();

    disp.destroyCommandPool(command_pool, nullptr);

    swapchain.destroy_image_views(swapchain_image_views);

    destroyImage(depth_image);
    destroyImageView(depth_image);

    if (!create_command_pool()) return false;
    if (!create_swapchain()) return false;
    if (!prepare_images()) return false;
    if (!create_command_buffers()) return false;

    return true;
}

void VulkanRenderer::transitionImageLayout(VkCommandBuffer command_buffer, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout)
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

bool VulkanRenderer::beginSingleTimeCommands(VkCommandBuffer& command_buffer)
{
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    if (disp.allocateCommandBuffers(&alloc_info, &command_buffer) != VK_SUCCESS) 
    {
        std::cerr << "Failed to allocate single time command buffer.\n";
        return false;
    }

    VkCommandBufferBeginInfo begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (disp.beginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) 
    {
        std::cerr << "Failed to begin recording single time command buffer.\n";
        return false;
    }

    return true;
}

bool VulkanRenderer::endSingleTimeCommands(VkCommandBuffer& command_buffer, VkQueue queue)
{
    if (disp.endCommandBuffer(command_buffer) != VK_SUCCESS) 
    {
        std::cerr << "Failed to record single time command buffer.\n";
        return false;
    }

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    if (disp.queueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) 
    {
        std::cerr << "Failed to submit single time command buffer.\n";
        return false;
    }

    disp.queueWaitIdle(queue);

    disp.freeCommandBuffers(command_pool, 1, &command_buffer);

    return true;
}

VkResult VulkanRenderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage, AllocatedBuffer& buffer)
{
    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo alloc_info{};
    alloc_info.usage = memory_usage;

    return vmaCreateBuffer(allocator, &buffer_info, &alloc_info, &buffer.buffer, &buffer.allocation, nullptr);
}

void VulkanRenderer::destroyBuffer(AllocatedBuffer& buffer)
{
    vmaDestroyBuffer(allocator, buffer.buffer, buffer.allocation);
}

VkResult VulkanRenderer::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaMemoryUsage memory_usage, AllocatedImage& image)
{
    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo alloc_info{};
    alloc_info.usage = memory_usage;

    return vmaCreateImage(allocator, &image_info, &alloc_info, &image.image, &image.allocation, nullptr);
}

void VulkanRenderer::destroyImage(AllocatedImage& image)
{
    vmaDestroyImage(allocator, image.image, image.allocation);
}

VkResult VulkanRenderer::createImageView(VkFormat format, VkImageAspectFlags aspect, AllocatedImage& image_view)
{
    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image_view.image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.subresourceRange.aspectMask = aspect;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    return disp.createImageView(&view_info, nullptr, &image_view.image_view);
}

void VulkanRenderer::destroyImageView(AllocatedImage& image_view)
{
    disp.destroyImageView(image_view.image_view, nullptr);
}

VkVertexInputBindingDescription VulkanRenderer::getBindingDescription()
{
    VkVertexInputBindingDescription binding_description{};
    binding_description.binding = 0;
    binding_description.stride = sizeof(Vertex);
    binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return binding_description;
}

std::array<VkVertexInputAttributeDescription, 4> VulkanRenderer::getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 4> attribute_descriptions{};

    attribute_descriptions[0].binding = 0;
    attribute_descriptions[0].location = 0;
    attribute_descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[0].offset = offsetof(Vertex, position);

    attribute_descriptions[1].binding = 0;
    attribute_descriptions[1].location = 1;
    attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attribute_descriptions[1].offset = offsetof(Vertex, normal);

    attribute_descriptions[2].binding = 0;
    attribute_descriptions[2].location = 2;
    attribute_descriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_descriptions[2].offset = offsetof(Vertex, tex_coord);

    attribute_descriptions[3].binding = 0;
    attribute_descriptions[3].location = 3;
    attribute_descriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attribute_descriptions[3].offset = offsetof(Vertex, color);

    return attribute_descriptions;
}