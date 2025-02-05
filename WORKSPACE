""" Fully Homomorphic Encryption root worksapce """

workspace(name = "com_google_fully_homomorphic_encryption")

load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "new_git_repository",
)
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Install TFHE
http_archive(
    name = "rules_foreign_cc",
    sha256 = "e2753b15ca6384d8f3afbe3d92375dfbec3545caff91001d6713bbefeb1ca5fa",
    strip_prefix = "rules_foreign_cc-40b03b42eb2d3ac65b58e95e6c7fce7e8c902117",
    # We need to use a pre-release version of rules_foreign_cc, as our PALISADE
    # integration requires the `includes` parameter... which was added to
    # rules_foreign_cc just *after* the 0.7.0 release closed.
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/40b03b42eb2d3ac65b58e95e6c7fce7e8c902117.zip",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

http_archive(
    name = "tfhe",
    build_file = "//patches:tfhe.BUILD",
    sha256 = "7ad88b70b389bfdb871488a90372b0cecd9ba731183ba02c3cd0ce86c9adcc93",
    strip_prefix = "tfhe-a085efe91314f994285fcb06ab8bdae3d55e4505",
    url = "https://github.com/tfhe/tfhe/archive/a085efe91314f994285fcb06ab8bdae3d55e4505.tar.gz",
)

# Install PALISADE

new_git_repository(
    name = "palisade",
    build_file = "//patches:palisade.BUILD",
    remote = "https://gitlab.com/palisade/palisade-release.git",
    # tag: v1.11.5
    commit = "d76213499af44558170cca6c72c5314755fec23c",
    init_submodules = True,
    shallow_since = "1631909486 +0000",
)

# Install XLS with transitive dependencies.
# 2022-03-013-013-01
http_archive(
    name = "com_google_xls",
    patches = [
        "//patches:xls-visibility-file.patch",
        "//patches:xls-visibility-ir.patch",
        "//patches:xls-visibility-logging.patch",
        "//patches:xls-visibility-status.patch",
    ],
    sha256 = "48edfe8ca09059a389ce1031f835dd6fdf93d68592e8d86668e4cfa48f8dd0ec",
    strip_prefix = "xls-24dab38f9f70f474d751c768e2cd3a7de2f8a3a8",
    url = "https://github.com/google/xls/archive/24dab38f9f70f474d751c768e2cd3a7de2f8a3a8.tar.gz",
)

# Install dependencies for XLS
http_archive(
    name = "com_grail_bazel_toolchain",
    sha256 = "dd03374af7885d255eb735b9065a32463a1154d9de6eb47261a49c8acc1cd497",
    strip_prefix = "bazel-toolchain-0.6.3",
    urls = [
        "https://github.com/grailbio/bazel-toolchain/archive/0.6.3.zip",
    ],
)

load("@com_grail_bazel_toolchain//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@com_grail_bazel_toolchain//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    # Try using llvm version 10.0.1 if you are getting an "Unknown LLVM
    # release" error.  This is a known issue for Ubuntu 16.04 LTS.
    llvm_version = "10.0.0",
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

load("@com_google_xls//dependency_support:load_external.bzl", "load_external_repositories")

load_external_repositories()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

# Install ABC
http_archive(
    name = "abc",
    build_file = "//patches:abc.BUILD",
    sha256 = "7fa5a448a4309fb4d6cf856c3fe4cc4be46b09dd552a05d5cfacd75f8d9504ad",
    strip_prefix = "abc-eb44a80bf2eb8723231e72bb095c97d1e4834d56",
    urls = [
        "https://github.com/berkeley-abc/abc/archive/eb44a80bf2eb8723231e72bb095c97d1e4834d56.zip",
    ],
)

# Install Yosys
http_archive(
    name = "yosys",
    build_file = "//patches:yosys.BUILD",
    strip_prefix = "yosys-cdb57118758f855518ad416d12728d72bff58c10",
    sha256 = "ee4a78a4e321615c0f2228ad169c7fd39eb4d3b987ff8ba9b10d937f09366121",
    patch_args = [ "-p1" ],
    patches = [ "//patches:0001-Fetch-YOSYS_DATDIR-from-the-environment.patch" ],
    urls = [
        "https://github.com/YosysHQ/yosys/archive/cdb57118758f855518ad416d12728d72bff58c10.zip",
    ],
)

load("@com_google_xls//dependency_support:initialize_external.bzl", "initialize_external_repositories")

initialize_external_repositories()
