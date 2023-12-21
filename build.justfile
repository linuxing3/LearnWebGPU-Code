workspace_dir := "/home/vagrant/workspace/cxx/LearnWebGPU-Code/"

# --------------------- projects --------------------------
learnwebgpu-setup:
  #!/usr/bin/env bash
  set -euxo pipefail
  cd {{workspace_dir}}
  cmake -B build -DDEV_MODE=On -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DWEBGPU_BACKEND=WGPU

learnwebgpu-build:
  #!/usr/bin/env bash
  set -euxo pipefail
  cd {{workspace_dir}}
  cmake --build build

learnwebgpu-run:
  #!/usr/bin/env bash
  set -euxo pipefail
  cd {{workspace_dir}}
  ./build/App

learnwebgpu-watch:
  #!/usr/bin/env bash
  set -euxo pipefail
  cd {{workspace_dir}}
  watchexec -e cpp -- "cmake --build build"

learnwebgpu-start:
  #!/usr/bin/env bash
  set -euxo pipefail
  cd {{workspace_dir}}
  watchexec -w ./build/App  -- "./build/App"

learnwebgpu: learnwebgpu-build
  #!/usr/bin/env bash
  cd {{workspace_dir}}
  ./build/App

# vim: set ft=make :
