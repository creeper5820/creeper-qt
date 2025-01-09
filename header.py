import os

root_dir = os.path.dirname(os.path.abspath(__file__))

widget_dir = os.path.join(root_dir, "include/creeper-qt/widget/")
module_dir = os.path.join(root_dir, "include/creeper-qt/module/")
container_dir = os.path.join(root_dir, "include/creeper-qt/container/")

setting_dir = os.path.join(root_dir, "include/creeper-qt/setting/")
utility_dir = os.path.join(root_dir, "include/creeper-qt/utility/")


def simplify_path(full_path, base_path):
    if full_path.startswith(base_path):
        return full_path[len(base_path) :]
    else:
        return full_path


def get_headers(dir):
    headers = []
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(".hh"):
                headers.append(file)
    return headers


setting_headers = get_headers(setting_dir)
utility_headers = get_headers(utility_dir)

widget_headers = get_headers(widget_dir)
module_headers = get_headers(module_dir)
container_headers = get_headers(container_dir)

common_header = os.path.join(root_dir, "include/creeper-qt/common.hh")
widget_header = os.path.join(root_dir, "include/creeper-qt/widget.hh")
module_header = os.path.join(root_dir, "include/creeper-qt/module.hh")
container_header = os.path.join(root_dir, "include/creeper-qt/container.hh")

# write headers include to header
with open(common_header, "w") as f:
    f.write("#pragma once\n\n")
    for header in setting_headers:
        f.write(f"#include <creeper-qt/setting/{header}>\n")
    for header in utility_headers:
        f.write(f"#include <creeper-qt/utility/{header}>\n")

with open(widget_header, "w") as f:
    f.write("#pragma once\n\n")
    f.write("#include <creeper-qt/common.hh>\n")
    for header in widget_headers:
        f.write(f"#include <creeper-qt/widget/{header}>\n")

with open(module_header, "w") as f:
    f.write("#pragma once\n\n")
    f.write("#include <creeper-qt/common.hh>\n")
    for header in module_headers:
        f.write(f"#include <creeper-qt/module/{header}>\n")

with open(container_header, "w") as f:
    f.write("#pragma once\n\n")
    f.write("#include <creeper-qt/common.hh>\n")
    for header in container_headers:
        f.write(f"#include <creeper-qt/container/{header}>\n")
