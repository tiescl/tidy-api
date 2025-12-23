#!/usr/bin/env python3

import yaml
import re
from pathlib import Path
from collections import OrderedDict


def represent_ordereddict(dumper, data):
    return dumper.represent_dict(data.items())


yaml.add_representer(OrderedDict, represent_ordereddict)


def fix_ref(ref):
    if isinstance(ref, str):
        match = re.match(r'^([a-zA-Z0-9_-]+\.yaml)?(#.+)$', ref)
        if match:
            return match.group(2)
    return ref


def process_refs(obj):
    if isinstance(obj, dict):
        new_dict = {}
        for key, value in obj.items():
            if key == '$ref':
                new_dict[key] = fix_ref(value)
            else:
                new_dict[key] = process_refs(value)
        return new_dict
    elif isinstance(obj, list):
        return [process_refs(item) for item in obj]
    else:
        return obj


def merge_openapi_files():
    docs_dir = Path('docs')
    output_file = 'openapi.yaml'

    with open(docs_dir / 'header.yaml', 'r') as f:
        merged = yaml.safe_load(f)

    if 'paths' not in merged:
        merged['paths'] = {}
    if 'components' not in merged:
        merged['components'] = {}
    if 'schemas' not in merged['components']:
        merged['components']['schemas'] = {}
    if 'parameters' not in merged['components']:
        merged['components']['parameters'] = {}

    for yaml_file in sorted(docs_dir.glob('*.yaml')):
        if yaml_file.name == 'header.yaml':
            continue

        with open(yaml_file, 'r') as f:
            data = yaml.safe_load(f)

        if not data:
            continue

        if 'paths' in data:
            merged['paths'].update(data['paths'])

        if 'components' in data:
            if 'schemas' in data['components']:
                merged['components']['schemas'].update(
                    data['components']['schemas'])
            if 'parameters' in data['components']:
                merged['components']['parameters'].update(
                    data['components']['parameters'])

    merged = process_refs(merged)

    if isinstance(merged, dict) and merged.get('paths'):
        merged['paths'] = dict(sorted(merged['paths'].items()))
    if isinstance(merged, dict) and merged['components'].get('schemas'):
        merged['components']['schemas'] = dict(
            sorted(merged['components']['schemas'].items())
        )
    if isinstance(merged, dict) and merged['components'].get('parameters'):
        merged['components']['parameters'] = dict(
            sorted(merged['components']['parameters'].items())
        )

    with open(output_file, 'w') as f:
        yaml.dump(merged, f, default_flow_style=False,
                  sort_keys=False, allow_unicode=True)


if __name__ == '__main__':
    merge_openapi_files()
