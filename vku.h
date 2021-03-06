#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

std::uint32_t vku_find_memory_type(
    const VkPhysicalDevice          physical_device,
    const std::uint32_t             type_filter,
    const VkMemoryPropertyFlags     properties);

VkFormat vku_find_supported_format(
    const VkPhysicalDevice          physical_device,
    const std::vector<VkFormat>&    candidates,
    const VkImageTiling             tiling,
    const VkFormatFeatureFlags      features);

VkFormat vku_find_depth_format(
    const VkPhysicalDevice          physical_device);

bool vku_has_stencil_component(
    const VkFormat                  format);

void vku_create_buffer(
    const VkPhysicalDevice          physical_device,
    const VkDevice                  device,
    const VkDeviceSize              size,
    const VkBufferUsageFlags        usage,
    const VkMemoryPropertyFlags     properties,
    VkBuffer&                       buffer,
    VkDeviceMemory&                 buffer_memory);

void vku_create_image(
    const VkPhysicalDevice          physical_device,
    const VkDevice                  device,
    const std::uint32_t             width,
    const std::uint32_t             height,
    const std::uint32_t             mip_levels,
    const VkSampleCountFlagBits     samples,
    const VkFormat                  format,
    const VkImageTiling             tiling,
    const VkImageUsageFlags         usage,
    const VkMemoryPropertyFlags     properties,
    VkImage&                        image,
    VkDeviceMemory&                 image_memory);

VkImageView vku_create_image_view(
    const VkDevice                  device,
    const VkImage                   image,
    const std::uint32_t             mip_levels,
    const VkFormat                  format,
    const VkImageAspectFlags        aspect_flags);

void vku_allocate_command_buffers(
    const VkDevice                  device,
    const VkCommandPool             command_pool,
    const std::uint32_t             command_buffer_count,
    VkCommandBuffer*                command_buffers);

VkCommandBuffer vku_begin_single_time_commands(
    const VkDevice                  device,
    const VkCommandPool             command_pool);

void vku_end_single_time_commands(
    const VkDevice                  device,
    const VkQueue                   queue,
    const VkCommandPool             command_pool,
    const VkCommandBuffer           command_buffer);

void vku_copy_host_to_device(
    const VkDevice                  device,
    const VkDeviceMemory            dest,
    const void*                     source,
    const VkDeviceSize              size);

void vku_copy_buffer_sync(
    const VkDevice                  device,
    const VkQueue                   queue,
    const VkCommandPool             command_pool,
    const VkBuffer                  dst_buffer,
    const VkBuffer                  src_buffer,
    const VkDeviceSize              size);

void vku_copy_buffer_to_image(
    const VkDevice                  device,
    const VkQueue                   queue,
    const VkCommandPool             command_pool,
    const VkImage                   image,
    const VkBuffer                  buffer,
    const std::uint32_t             width,
    const std::uint32_t             height);

void vku_transition_image_layout(
    const VkDevice                  device,
    const VkQueue                   queue,
    const VkCommandPool             command_pool,
    const VkImage                   image,
    const std::uint32_t             mip_levels,
    const VkFormat                  format,
    const VkImageLayout             old_layout,
    const VkImageLayout             new_layout);
