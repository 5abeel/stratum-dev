# Copyright 2018 Google LLC
# Copyright 2018-present Open Networking Foundation
# SPDX-License-Identifier: Apache-2.0

load("@rules_pkg//:pkg.bzl", "pkg_deb", "pkg_tar")
load("//bazel:rules.bzl", "HOST_ARCHES", "stratum_cc_binary")

licenses(["notice"])  # Apache v2

package(
    default_visibility = ["//visibility:public"],
)

stratum_bf_common_deps = [
    "//stratum/glue:init_google",
    "//stratum/glue:logging",
    "//stratum/hal/lib/common:hal",
    "//stratum/hal/lib/phal:phal_sim",
    "//stratum/hal/lib/phal",
    "//stratum/lib/security:auth_policy_checker",
    "//stratum/lib/security:credentials_manager",
    "@com_github_google_glog//:glog",
]

stratum_cc_binary(
    name = "bf_pipeline_builder",
    srcs = [
        "bf_pipeline_builder.cc",
    ],
    arches = HOST_ARCHES,
    deps = [
        "//stratum/glue:init_google",
        "//stratum/glue:logging",
        "//stratum/glue/status",
        "//stratum/glue/status:status_macros",
        "//stratum/hal/lib/barefoot:bf_cc_proto",
        "//stratum/lib:utils",
        "@com_github_nlohmann_json//:json",
    ],
)

stratum_cc_binary(
    name = "stratum_bf",
    srcs = [
        "main.cc",
    ],
    arches = HOST_ARCHES,
    data = [
        "tofino_skip_p4.conf",
        "tofino_skip_p4_no_bsp.conf",
    ],
    deps = [
        "//stratum/hal/lib/barefoot:bf_chassis_manager",
        "//stratum/hal/lib/barefoot:bf_sde_wrapper",
        "//stratum/hal/lib/barefoot:bf_switch",
        "//stratum/hal/lib/pi:pi_node_bf",
    ] + stratum_bf_common_deps,
)

stratum_cc_binary(
    name = "stratum_bfrt",
    srcs = [
        "main_bfrt.cc",
    ],
    arches = HOST_ARCHES,
    data = [
        "tofino_skip_p4.conf",
        "tofino_skip_p4_no_bsp.conf",
    ],
    deps = [
        "//stratum/hal/lib/barefoot:bf_chassis_manager",
        "//stratum/hal/lib/barefoot:bf_sde_wrapper",
        "//stratum/hal/lib/barefoot:bfrt_switch",
        "//stratum/hal/lib/barefoot:bfrt_node",
        "//stratum/hal/lib/barefoot:bfrt_action_profile_manager",
        "//stratum/hal/lib/barefoot:bfrt_pre_manager",
        "//stratum/hal/lib/barefoot:bfrt_table_manager",
        "//stratum/hal/lib/barefoot:bfrt_counter_manager",
    ] + stratum_bf_common_deps,
)

# Rules for Debian package
pkg_tar(
    name = "stratum_binaries",
    srcs = [
        "deb/attach-bf-shell.sh",
        "deb/start-stratum.sh",
        ":bf_pipeline_builder",
        ":stratum_bf",
    ],
    mode = "0755",
    package_dir = "/usr/bin",
)

pkg_tar(
    name = "stratum_bfrt_binaries",
    # TODO(bocon): Consider refactoring common start script and builder
    srcs = [
        "deb/attach-bf-shell.sh",
        "deb/start-stratum.sh",
        ":bf_pipeline_builder",
        ":stratum_bfrt",
    ],
    mode = "0755",
    package_dir = "/usr/bin",
    symlinks = {"./usr/bin/stratum_bf": "/usr/bin/stratum_bfrt"},
)

pkg_tar(
    name = "systemd_service",
    srcs = [
        "deb/stratum_bf.service",
    ],
    mode = "0644",
    package_dir = "/usr/lib/systemd/system",
)

pkg_tar(
    name = "stratum_shareable_files",
    srcs = [
        "tofino_skip_p4.conf",
        "tofino_skip_p4_no_bsp.conf",
    ],
    mode = "0644",
    package_dir = "/usr/share/stratum",
)

pkg_tar(
    name = "stratum_configs",
    srcs = [
        "deb/stratum.flags",
        "deb/stratum_bf.env",
        "//stratum/hal/lib/common:gnmi_caps.pb.txt",
    ],
    mode = "0644",
    package_dir = "/etc/stratum",
    deps = [
        "//stratum/hal/config:platform_configs_tar",
    ],
)

pkg_tar(
    name = "stratum_common_data",
    deps = [
        ":stratum_configs",
        ":stratum_shareable_files",
        ":systemd_service",
        "@local_barefoot_bin//:bf_library_files",
        "@local_barefoot_bin//:bf_shareable_files",
        "@local_barefoot_bin//:kernel_module",
    ],
)

pkg_tar(
    name = "stratum_bf_data",
    extension = "tar.bz2",
    deps = [
        ":stratum_binaries",
        ":stratum_common_data",
    ],
)

pkg_tar(
    name = "stratum_bfrt_data",
    extension = "tar.bz2",
    deps = [
        ":stratum_bfrt_binaries",
        ":stratum_common_data",
    ],
)

pkg_deb(
    name = "stratum_bf_deb",
    architecture = "amd64",
    conflicts = ["stratum-bfrt"],
    data = ":stratum_bf_data",
    depends = [
        "kmod",
        "libedit2",
        "systemd",
        "telnet",
    ],
    description = "The Stratum package for Barefoot Tofino-based platform",
    homepage = "https://stratumproject.org/",
    maintainer = "The Stratum Project",
    package = "stratum-bf",
    postinst = "deb/postinst",
    prerm = "deb/prerm",
    recommends = ["systemd"],
    version = "0.0.1",
)

pkg_deb(
    name = "stratum_bfrt_deb",
    architecture = "amd64",
    conflicts = ["stratum-bf"],
    data = ":stratum_bfrt_data",
    depends = [
        "kmod",
        "libedit2",
        "libssl1.1",
        "systemd",
        "telnet",
    ],
    description = "The Stratum package for Barefoot Tofino-based platform",
    homepage = "https://stratumproject.org/",
    maintainer = "The Stratum Project",
    package = "stratum-bfrt",
    postinst = "deb/postinst",
    prerm = "deb/prerm",
    version = "0.0.1",
)
