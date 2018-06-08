#!/usr/bin/env node

const fs = require('fs');
const path = require('path');

const dirname = path.dirname(process.argv[1]);

let contents = fs.readFileSync(`${process.env.VK_SDK_PATH}/Include/vulkan/vulkan.hpp`, 'utf8');

// find Device class
const classStart = contents.match(/class Device\b(?!;)/);
const classEnd = contents.match(/VkDevice m_device;/);

if ( ! classStart) {
	console.error('Class start not found');
	return -1;
}

if ( ! classEnd) {
	console.error('Class end not found');
	return -1;
}

const fixStructureChain = {
	getBufferMemoryRequirements2: 'MemoryRequirements2',
	getBufferMemoryRequirements2KHR: 'MemoryRequirements2',
	getImageMemoryRequirements2: 'MemoryRequirements2',
	getImageMemoryRequirements2KHR: 'MemoryRequirements2',
	getDescriptorSetLayoutSupport: 'DescriptorSetLayoutSupport',
	getDescriptorSetLayoutSupportKHR: 'DescriptorSetLayoutSupport',
};

contents = contents.substring(classStart.index, contents.indexOf('};', classEnd.index) + 3)
	// remove \r
	.replace(/\r/g, '')
	// remove multiple new lines
	.replace(/\n{3,}/g, '\n\n')
	// replace spaces with tabs
	.replace(/[ ]{2}/g, '\t')
	// decrease indent by 1 level
	.replace(/\n\t/g, '\n')
	// remove template<typename Dispatch = DispatchLoaderStatic>
	.replace(/\ttemplate<typename Dispatch = DispatchLoaderStatic>\n/g, '')
	// shorten template <..., typename Dispatch = DispatchLoaderStatic>
	.replace(/template <(.+?), typename Dispatch = DispatchLoaderStatic> ?/g, 'template <$1>')
	// fix "( " and " )"
	.replace(/\(\s+/g, '(')
	.replace(/\s+\)/g, ')')
	// enhance methods
	.replace(/^\t(.+?)? (\w+)\((.*?)Dispatch const &d = Dispatch\(\)\)(\s+const|\s*)?;/gm, (match, type, name, params, isConst) => {
		const args = params
			// remove default values
			.replace(/ = [^,]+/g, '')
			// remove types
			.replace(/(^|,\s).+?(\b\w+\b)(?=,|$)/g, (match, start, name) => {
				return `${start}${name}`;
			});

		// trim last coma
		params = params.replace(/, $/, '');

		if (fixStructureChain.hasOwnProperty(name)) {
			if (type.indexOf('StructureChain') === 0) {
				return `\t${type} ${name}(${params})${isConst} {
		return device.${name}<T...>(${args}dispatch);
	}`;
			} else if (type === fixStructureChain[name]) {
				return `\t${type} ${name}(${params})${isConst} {
		return device.${name}<${fixStructureChain[name]}>(${args}dispatch).get<${fixStructureChain[name]}>();
	}`;
			}
		}

		return `\t${type} ${name}(${params})${isConst} {
		return device.${name}(${args}dispatch);
	}`;
	})
	// rename class
	.replace('class Device', 'class DeviceDispatch');

// find header
const headerStart = contents.indexOf('VULKAN_HPP_CONSTEXPR Device()');
const headerEnd = contents.indexOf('bool operator<(Device const & rhs) const', headerStart) >= 0 ? contents.indexOf('}', contents.indexOf('bool operator<(Device const & rhs) const', headerStart)) : -1;

if (headerStart < 0) {
	console.error('Header start not found');
	return -1;
}

if (headerEnd < 0) {
	console.error('Header end not found');
	return -1;
}

const customHeader = `DeviceDispatch() = default;

	explicit DeviceDispatch(Device &device, Instance &instance) noexcept :
		device{device},
		dispatch{instance, device}
	{}`;

// replace header
contents = contents.substring(0, headerStart) + customHeader + contents.substring(headerEnd + 1);

// find footer
const footerStart = contents.indexOf('VULKAN_HPP_TYPESAFE_EXPLICIT operator VkDevice() const');
const footerEnd = contents.indexOf(';', contents.indexOf('VkDevice m_device;', footerStart));

if (footerStart < 0) {
	console.error('Footer start not found');
	return -1;
}

if (footerEnd < 0) {
	console.error('Footer end not found');
	return -1;
}

const customFooter = `VULKAN_HPP_TYPESAFE_EXPLICIT operator VkDevice() const
	{
		return device;
	}

	explicit operator bool() const
	{
		return device;
	}

	bool operator!() const
	{
		return !device;
	}

private:
	Device device{};
	DispatchLoaderDynamic dispatch{};`

// replace footer
contents = contents.substring(0, footerStart) + customFooter + contents.substring(footerEnd + 1);

/**/fs.writeFileSync/*/console.log/**/(path.join(dirname, 'device.out'), `#pragma once

#include <cassert>
#include <vulkan/vulkan.hpp>

static_assert( VK_HEADER_VERSION ==  73 , "Wrong VK_HEADER_VERSION!" );

namespace vk {

${contents}

} // vk

namespace rn::vlk {

using Device = vk::DeviceDispatch;

} // rn::vlk
`);