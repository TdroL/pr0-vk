#!/usr/bin/env node
'use strict';

const fs = require('fs');

const name = process.argv[2];
if ( ! name) {
	throw new Error('name is missing');
}

const ucName = name.replace(/^[a-z]/, (char) => char.toUpperCase());
const schema = require('./' + name + '.schema.json');

const save = process.argv[3];

const structName = name.replace(/^[a-z]/, (match) => match.toUpperCase());

const primitiveTypes = {
	'intmax_t': true,
	'uintmax_t': true,
	'int8_t': true,
	'uint8_t': true,
	'int16_t': true,
	'uint16_t': true,
	'int32_t': true,
	'uint32_t': true,
	'int64_t': true,
	'uint64_t': true,
	'int_least8_t': true,
	'uint_least8_t': true,
	'int_least16_t': true,
	'uint_least16_t': true,
	'int_least32_t': true,
	'uint_least32_t': true,
	'int_least64_t': true,
	'uint_least64_t': true,
	'int_fast8_t': true,
	'uint_fast8_t': true,
	'int_fast16_t': true,
	'uint_fast16_t': true,
	'int_fast32_t': true,
	'uint_fast32_t': true,
	'int_fast64_t': true,
	'uint_fast64_t': true,
	'intptr_t': true,
	'uintptr_t': true,
	'int': true,
	'long': true,
	'long int': true,
	'long long': true,
	'long long int': true,
	'short': true,
	'short int': true,
	'signed': true,
	'signed int': true,
	'signed long': true,
	'signed long int': true,
	'signed long long': true,
	'signed long long int': true,
	'signed short': true,
	'signed short int': true,
	'unsigned': true,
	'unsigned int': true,
	'unsigned long': true,
	'unsigned long int': true,
	'unsigned long long': true,
	'unsigned long long int': true,
	'unsigned short': true,
	'unsigned short int': true,
	'char': true,
	'signed char': true,
	'unsigned char': true,
	'char16_t': true,
	'char32_t': true,
	'wchar_t': true,
	'float': true,
	'double': true,
	'bool': true,
};

const padLines = (text, padding) => {
	text = text.toString();
	return text.split('\n')
		.map((line) => line ? padding + line : '')
		.join('\n');
};

const generateEnum = (type, mapping) => {
	const enumTemplate = `enum class {{type}} {
	{{values}}
};

std::string toString(const {{type}} &value);
{{type}} fromString(const std::string &name, const {{type}} &defaultValue);`;

	const model = {
		type: type,
		values: Object.keys(mapping).map((key) => mapping[key]).join(',\n'),
	};

	return enumTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateEnumCasters = (type, mapping) => {
	const enumCastersTemplate = `std::string toString(const {{type}} &value) {
	switch (value) {
		default:
		{{cases}}
	}
}

{{type}} fromString(const std::string &name, const {{type}} &defaultValue) {
	{{ifs}}
	return defaultValue;
}`;

	const model = {
		type: type,
		cases: Object.keys(mapping).map((key) => `case ${type}::${mapping[key]}: { return "${key}"; }`).join('\n'),
		ifs: Object.keys(mapping).map((key) => `if (name == "${key}") { return ${type}::${mapping[key]}; }`).join('\n'),
	};

	return enumCastersTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateSubStruct = (subName, child, schema) => {
	const subStructTemplate = `struct {{ucSubName}} {
	{{ucName}} &root;

	{{ucSubName}}({{ucName}} &root) : root(root) {}

	{{props}}
} {{subName}}{root};`;

	const props = Object.keys(child).map((propId) => {
		if (propId[0] == '#') {
			return generateSubStruct(propId.substring(1), child[propId], schema);
		}

		return generateAccessor(propId, child[propId], schema);
	});

	const model = {
		name: name,
		ucName: ucName,
		subName: subName,
		ucSubName: subName.replace(/^[a-z]/, (char) => char.toUpperCase()),
		props: props.join('\n\n'),
	};

	return subStructTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateAccessor = (subName, def, schema) => {
	const accessorTemplateGetter = `{{type}} prop_{{subName}}{{{defaultValue}}};
{{getterType}} {{subName}}() {
	return prop_{{subName}};
}`;

	const accessorTemplateSetter = `void {{subName}}({{setterType}}newValue) {
	prop_{{subName}} = newValue;
	root.dirty(true);
}`;

	let defaultValue = def.readOnly ? def.value : def.default;

	if (schema.enums.hasOwnProperty(def.type)) {
		defaultValue = `${def.type}::${schema.enums[def.type][def.default]}`;
	}

	const model = {
		type: def.type,
		getterType: primitiveTypes.hasOwnProperty(def.type) ? def.type : 'const ' + def.type + ' &',
		setterType: primitiveTypes.hasOwnProperty(def.type) ? def.type + ' ' : 'const ' + def.type + ' &',
		subName: subName,
		defaultValue: defaultValue,
	};

	var templates = [];

	templates.push(
		accessorTemplateGetter.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding))
	);

	if ( ! def.readOnly) {
		templates.push(
			accessorTemplateSetter.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding))
		);
	}

	return templates.join('\n\n');
};

const generateRootStruct = (schema) => {
	const structTemplate = `struct {{ucName}} {
	{{ucName}} &root{*this};

	{{ucName}}() { load(); }

	{{props}}

	bool dirty_value = false;
	void dirty(bool newValue) {
		dirty_value = newValue;
	}

	bool dirty() {
		return dirty_value;
	}

	std::string source_value{"{{directory}}{{name}}.json"};
	void source(const std::string &newValue) {
		source_value = newValue;
	}

	std::string & source() {
		return source_value;
	}

	void load();
	std::string dump(const int indent = 2);
	bool store();
};`;

	const props = Object.keys(schema.struct).map((propId) => {
		if (propId[0] == '#') {
			return generateSubStruct(propId.substring(1), schema.struct[propId], schema);
		}

		return generateAccessor(propId, schema.struct[propId], schema);
	});

	const model = {
		name: name,
		ucName: ucName,
		directory: schema.directory || '',
		props: props.join('\n\n'),
	};

	return structTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateHeader = (schema) => {
	const headerTemplate = `#pragma once

#include <string>

namespace ngn {

namespace config {

{{content}}

} // config

} // ngn
`;

	let content = [];

	const enumDefs = Object.keys(schema.enums).map((key) => generateEnum(key, schema.enums[key]));
	const struct = generateRootStruct(schema);

	if (enumDefs.length) {
		content.push(enumDefs.join('\n\n'));
	}

	if (struct) {
		content.push(struct);
	}

	const model = {
		content: content.join('\n\n')
	};

	return headerTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateLoader = (path, def, schema) => {
	const last = path[path.length - 1];
	if (last[0] === '#') {
		return Object.keys(def).reduce((acc, key) => {
			if (key[0] !== '#' && def[key].readOnly) {
				return acc;
			}
			return acc.concat(generateLoader(path.slice(0, path.length - 1).concat([last.substring(1), key]), def[key], schema));
		}, []);
	}

	const loaderTemplate = `try {
	{{getter}}
} catch (const std::out_of_range &/*e*/) {
	{{path}}({{defaultValue}});
} catch (const std::domain_error &/*e*/) {
	{{path}}({{defaultValue}});
} catch (const std::invalid_argument &/*e*/) {
	{{path}}({{defaultValue}});
}`;

	let defaultValue = 'defaultValue';
	let getter = 'getter';

	if (schema.enums.hasOwnProperty(def.type)) {
		defaultValue = `${def.type}::${schema.enums[def.type][def.default]}`;
		getter = `${path.join('.')}(fromString(body.at("/${path.join('/')}"_json_pointer).get<std::string>(), ${defaultValue}));`;
	} else {
		defaultValue = `${def.default}`;
		getter = `${path.join('.')}(body.at("/${path.join('/')}"_json_pointer).get<${def.type}>());`;
	}

	const model = {
		name: name,
		ucName: ucName,
		getter: getter,
		path: path.join('.'),
		defaultValue: defaultValue,
	};

	return loaderTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateValueStore = (path, def, schema) => {
	const last = path[path.length - 1];

	if (last[0] === '#') {
		const storeTemplate = `{"{{key}}", {
	{{stores}}
}}`;
		let stores = Object.keys(def).map((key) => generateValueStore(path.slice(0, path.length - 1).concat([last.substring(1), key]), def[key], schema));

		stores = stores.filter(stores => stores !== null);

		if ( ! stores.length) {
			return null;
		}

		const model = {
			key: last.substring(1),
			stores: stores.join(',\n')
		};

		return storeTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
	} else {
		if (def.readOnly) {
			return null;
		}

		const storeTemplate = `{"{{key}}", {{getter}}}`;

		let getter;
		if (schema.enums.hasOwnProperty(def.type)) {
			getter = `toString(${path.join('.')}())`;
		} else {
			getter = `${path.join('.')}()`;
		}

		const model = {
			key: last,
			getter: getter
		};

		return storeTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
	}
};

const generateLoad = (schema) => {
	const loadTemplate = `void {{ucName}}::load() {
	std::string contents = ngn::fs::contents(source(), false);
	if (contents.empty()) {
		contents = "{}";
		dirty(true);
	}

	json body = json::parse(contents);

	{{loaders}}
}`;

	const loaders = Object.keys(schema.struct).reduce((acc, key) => {
		if (schema.struct[key].readOnly) {
			return acc;
		}

		return acc.concat(generateLoader([key], schema.struct[key], schema));
	}, []);

	const model = {
		ucName: ucName,
		loaders: loaders.join('\n\n'),
	};

	return loadTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateDump = (schema) => {
	const storeTemplate = `std::string {{ucName}}::dump(const int indent) {
	json body = {
		{{stores}}
	};

	return body.dump(indent);
}`;

	let stores = Object.keys(schema.struct).map((key) => generateValueStore([key], schema.struct[key], schema));

	stores = stores.filter(store => store != null);

	const model = {
		ucName: ucName,
		stores: stores.join(',\n'),
	};

	return storeTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateStore = (schema) => {
	const storeTemplate = `bool {{ucName}}::store() {
	if (ngn::fs::contents(source(), dump(2), false)) {
		dirty(false);
		return true;
	} else {
		return false;
	}
}`;

	const model = {
		ucName: ucName,
	};

	return storeTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

const generateSource = (schema) => {
	const sourceTemplate = `#include "{{name}}.hpp"

#include <json.hpp>
#include "../fs.hpp"

using json = nlohmann::json;

namespace ngn {

namespace config {

{{content}}

} // config

} // ngn
`;

	let content = [];

	const enumCasters = Object.keys(schema.enums).map((key) => generateEnumCasters(key, schema.enums[key]));
	const load = generateLoad(schema);
	const dump = generateDump(schema);
	const store = generateStore(schema);

	if (enumCasters.length) {
		content.push(enumCasters.join('\n\n'));
	}

	if (load) {
		content.push(load);
	}

	if (dump) {
		content.push(dump);
	}

	if (store) {
		content.push(store);
	}

	const model = {
		name: name,
		content: content.join('\n\n')
	};

	return sourceTemplate.replace(/([\t ]*)\{\{([a-z]*)\}\}/ig, (match, padding, type) => padLines(model[type], padding));
};

if (save) {
	fs.writeFileSync('./' + name + '.hpp', generateHeader(schema));
	fs.writeFileSync('./' + name + '.cpp', generateSource(schema));
	console.log('done');
} else {
	console.log(generateHeader(schema));
	console.log(generateSource(schema));
}