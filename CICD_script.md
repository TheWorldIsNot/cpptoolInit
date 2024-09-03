目录树

```
my_cpp_project/
│
├── .gitignore                   # Git 忽略文件
├── .gitlab-ci.yml               # CI/CD 配置文件
├── LICENSE                      # 许可证文件
├── README.md                    # 项目说明
├── CMakeLists.txt               # 根 CMake 配置文件
|
├── build/                       # 构建目录，存放构建产物
├── cmake/                       # 自定义 CMake 模块或脚本
│
├── include/                     # 公共头文件
│   └── operations/              # 操作相关的头文件
│       ├── add_oper.h           # 加法操作声明
│       ├── minus_oper.h         # 减法操作声明
│       └── ...
├── src/                         # 源代码目录
│   ├── main.cpp                 # 主程序入口
│   ├── operations/              # 操作相关的源文件
│   │   ├── add_oper.cpp         # 加法操作实现
│   │   ├── minus_oper.cpp       # 减法操作实现
│   │   └── ...
│   └── ...
├── test/                        # 测试代码目录
│   ├── CMakeLists.txt           # 测试的 CMake 配置
│   ├── test_main.cpp            # 测试主函数
│   ├── operations/              # 操作相关的测试文件
│   │   ├── test_add_oper.cpp    # 加法操作测试
│   │   ├── test_minus_oper.cpp  # 减法操作测试
│   │   └── ...
│   └── googletest/             # Google Test 框架作为子模块
│       ├── CMakeLists.txt       # Google Test 的 CMake 配置
│       ├── src/                 # Google Test 源代码
│       ├── include/             # Google Test 头文件
│       └── ...
├── lib/                         # 第三方库和项目库文件
│   ├── third_party/             # 第三方库
│   │   └── ...                  # 其他第三方库文件
│   └── project_lib/             # 项目自己的库文件
│       └── ...
│
├── tools/                       # 静态分析和自动化工具脚本
│   ├── cppcheck/                # Cppcheck 配置和脚本
│   ├── format_code.sh           # 代码格式化脚本
│   └── ...
│
├── docs/                        # 项目文档
│   ├── design/                  # 设计文档
│   ├── user_guide.md            # 用户手册
│   └── developer_guide.md       # 开发者指南
│
└── config/                      # 配置文件
    └── default_config.json      # 默认配置文件
```

Makefile更多的是开发者本地，.gitlab-ci.yml是每次提交时的检查

在 `.gitlab-ci.yml` 中，可以定义如何使用 CMake 来构建项目，比如运行 `cmake` 命令来生成构建系统，并使用 `make` 或 `ctest` 来构建和测试项目。但实际的构建逻辑和配置是由 `CMakeLists.txt` 文件提供的。

- 尽管 `.gitlab-ci.yml` 可以自动化构建过程，但它本身并不包含如何构建项目的具体指令。它需要 `CMakeLists.txt` 来提供这些指令。
- 另一方面，没有 `.gitlab-ci.yml`，仍然可以在本地使用 CMake 手动构建项目，但失去了自动化带来的便利性，比如自动运行测试和部署。

```
run tests:
  stage: test
  script:
    - cd build
    - make test
    - gcovr --xml-pretty --exclude-unreachable-branches --print-summary -o coverage.xml --root ${CI_PROJECT_DIR}
  artifacts:
    name: ${CI_JOB_NAME}-${CI_COMMIT_REF_NAME}-${CI_COMMIT_SHA}
    expire_in: 2 days
    reports:
      coverage_report:
        coverage_format: cobertura
        path: build/coverage.xml
```

cmake -G "MinGW Makefiles" ..  或者   

mingw32-make.exe



D:\MinGW\mingw64\bin

D:\CMake\bin

D:\codebase\mycodetest\.vscode\launch.json

D:\codebase\mycodetest\src\main.cpp

D:\codebase\mycodetest\include\sample_head.cpp

D:\codebase\mycodetest\include\sample_head.h

D:\codebase\mycodetest\build\main.exe



---

Merge触发配置

### 设置保护规则

在 GitLab 的项目设置中，你可以为分支设置保护规则，以防止不合格的代码被合并。

- 进入你的项目设置页面。
- 选择 "Repository" > "Protected Branches"。
- 选择你想要保护的分支（通常是 `main` 或 `master`）。
- 启用 "Allow only merge requests to be merged" 选项，这样只有通过 Merge Request 才能合并代码。

### 配置 Merge Request 规则

在 GitLab 的项目设置中，你可以设置 Merge Request 规则，要求所有 MR 必须通过所有配置的 CI 测试。

- 进入 "Settings" > "Merge Requests"。
- 找到 "Pipeline must succeed" 选项并启用它。
- 选择 "test_job" 作为必须成功的作业。

---

在 `CMakeLists.txt` 和 `.gitlab-ci.yml` 中写入 `cppcheck` 和 `clang-tidy` 的配置确实有不同目的，它们并不重复，而是相辅相成的。下面解释两者的区别和联系：

### CMakeLists.txt 中的配置：

- **目的**: 使开发者在本地构建项目时能够执行静态代码分析。
- **作用范围**: 影响本地开发环境，确保开发者在编写代码时可以即时发现问题。
- **执行时机**: 当开发者在本地运行 CMake 构建系统时，例如通过 `make` 或 `cmake --build .` 命令构建项目时，如果配置了相应的目标，可以一并运行这些检查。
- **自定义性**: 可以为不同的构建类型或条件编译配置不同的检查选项。

### .gitlab-ci.yml 中的配置：

- **目的**: 在持续集成/持续部署（CI/CD）流程中自动化代码质量检查，确保代码在合并到主分支前满足一定的质量标准。
- **作用范围**: 影响 CI/CD 流程，确保所有提交都会经过标准化的检查。
- **执行时机**: 在代码推送到仓库后，由 GitLab 运行定义在 `.gitlab-ci.yml` 中的作业来执行。
- **自动化**: 自动化检查过程，无需人工干预，可以及时发现问题并给予反馈。

### 区别和联系：

- **重复性**: 两者并不重复，而是在不同环境（本地开发环境和 CI/CD 环境）中提供了一致的代码质量保证。
- **互补性**: 通过在两个地方配置，可以确保代码在开发过程中及早发现问题，并在合并到主分支前进行最终的质量检查。
- **效率**: 在 `CMakeLists.txt` 中配置可以提高开发者的工作效率，因为它们可以在本地快速迭代和修复问题。而在 `.gitlab-ci.yml` 中配置则确保了代码质量不会因为人为因素而降低。

### 是否只在一个地方写：

- **最佳实践**: 通常建议在两个地方都进行配置，以实现本地开发和 CI/CD 流程中的双重保障。
- **简化流程**: 想要减少合并冲突，可能更倾向于在 CI/CD 流程中严格检查。本地的配置仍然有助于提高开发效率

---

在 GitLab CI/CD 的 `.gitlab-ci.yml` 配置文件中，`ctest` 是 CMake 测试驱动程序的命令行接口。CMake 测试是 CMake 构建系统的一部分，它允许你自动化测试过程。

当在 CMakeLists.txt 文件中添加了 `enable_testing()` 命令并定义了测试（使用 `add_test()` 命令），你可以使用 `ctest` 来运行这些测试。`ctest` 可以执行以下功能：

- 自动发现并运行在 CMakeLists.txt 中定义的测试。
- 提供测试的进度和结果。
- 可以配置测试选项，如测试的并行级别、测试超时、测试过滤等。

在 CI/CD 配置文件中提到的 `ctest` 命令通常不带任何参数，意味着它将运行所有在构建过程中定义的测试。如果测试通过，`ctest` 命令将以 0 退出，表示成功；如果任何测试失败，它将以非零退出，表示有测试未通过。

``.codeclimate.yml` `

```
version: "2"
plugins:
  cppcheck:
    enabled: true
    config:
      standard: "c++11"
  clangtidy:
    enabled: true
    config:
      checks:
        - "*"
  eslint:
    enabled: false
exclude_patterns:
  - "tests/"
  - "vendor/"
```

---

**`git push`** **`git merge`**触发

### 4. `CI/CD:.gitlab-ci.yml`：

[CI/CD YAML syntax reference | GitLab](https://docs.gitlab.com/ee/ci/yaml/)

`.gitlab-ci.yml`是一个配置文件，定义了GitLab CI/CD的流程。它告诉GitLab如何构建、测试、分析和部署代码。这个文件通常包含以下几个部分：

```yaml
image: "debian:bullseye"

variables:
  PROJECT_NAME: "my_cpp_project"
  CC: "gcc"
  CXX: "g++"
  CXXFLAGS: "-Wall -Wextra -pedantic -std=c++17"
  COVERAGE_FLAGS: "-fprofile-arcs -ftest-coverage"
  DOCS_DIR: "$CI_PROJECT_DIR/docs"
  BUILD_DIR: "$CI_PROJECT_DIR/build"
  THIRD_PARTY_DIR: "$CI_PROJECT_DIR/third_party"
  CACHE_KEY: "cache-$CI_COMMIT_REF_SLUG"

cache:
  key: ${CACHE_KEY}
  paths:
    - third_party/
    - .cache/
  policy: pull-push
  untracked: true

stages:
  - install
  - analyze
  - sast
  - build
  - test_modified
  - test_regression
  - deploy
  - cleanup

.common_script_template: &common_script_template
  script:
    - echo "Determining modified files in src/ and include/ directories..."
    - MODIFIED_FILES=$(git diff --name-only HEAD^ | grep -E '(src|include)/.*\.(cpp|hpp|h)$')
    - echo "Modified files to process: $MODIFIED_FILES"


install_dependencies:
  stage: install
  script:
    - apt-get update -y
    - apt-get install -y $CC $CXX cmake make cppcheck clang-tidy lcov valgrind

analyze_job:
  stage: analyze
  script:
    - cppcheck --enable=all --inconclusive --verbose src/ include/
    - clang-tidy $((find src/ include/ -name '*.cpp' -o -name '*.hpp')) \
      --checks=* --extra-arg=-std=c++17 \
      --extra-arg=-Iinclude
  allow_failure: false
  
  
sast_job:
  stage: sast
  include:  
    - template: Security/SAST.gitlab-ci.yml
  variables:  
    SECURE_ANALYZERS_PREFIX: "registry.gitlab.com/security-products".
  artifacts:  # 定义 SAST 报告的路径和格式
    reports:
      sast: gl-sast-report.json
  allow_failure: false
  
# 构建项目作业，使用 CMake 和 make 构建项目
build_job:
  stage: build
  before_script:  
    - mkdir -p cmake_build && cd cmake_build  # 创建一个名为 cmake_build 的目录，如果目录已存在则忽略，然后进入该目录
  script:
    - cmake .. -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DCMAKE_CXX_FLAGS="${CXXFLAGS}"  
    - make -j$(nproc)  # 使用 make 工具并行构建项目，-j 参数后面跟着的 $(nproc) 是一个 shell 命令，用于获取当前系统的 CPU 核心数，以实现并行构建
  after_script:
    - cd ..  # 从 cmake_build 目录返回到上一级目录
  artifacts:  # 定义了构建产物，即构建过程中生成的文件
    paths:
      - cmake_build/  # 指定 cmake_build 目录下的所有文件作为构建产物
    expire_in: 1 day 

test_modified_proj_func:
  stage: test_modified
  <<: *common_script_template
  script:
    - set -e
    - echo "Running tests for modified files..."
    - for FILE in $MODIFIED_FILES; do
        BASENAME=$(basename "$FILE" .cpp)
        TEST_FILE="tests/test_${BASENAME}"
        if [ -f "$TEST_FILE" ]; then
          cd "$(dirname "$TEST_FILE")" && mkdir -p build && cd build
          cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
          make -j$(nproc) && ctest --output-on-failure -R "^test_${BASENAME}$"
        else
          echo "No test file found for $(basename "$FILE")"
        fi
      done
  rules:
    - if: '$CI_MERGE_REQUEST_DIFF_ID || $CI_COMMIT_BRANCH'
      changes:
        - "src/**/*.cpp"
        - "include/**/*.h"
      when: on_success
      allow_failure: false
  dependencies:
    - build_job


test_modified_valgrind:
  stage: test_modified
  <<: *common_script_template
  script:
    - set -e
    - echo "Running Valgrind on modified source files..."
      # 循环遍历修改过的源文件
      for MODIFIED_FILE in $MODIFIED_FILES; do
        # 确定可执行文件的路径，这里假设它与源文件同名且位于同一目录下的 build 目录中
        EXECUTABLE="./$(basename "${MODIFIED_FILE}" .cpp)" # 这里假设源文件是 .cpp 文件
        if [ -f "$EXECUTABLE" ]; then
          # 如果可执行文件存在，则运行 Valgrind
          cd "$(dirname "$EXECUTABLE")"
          valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./"$(basename "$EXECUTABLE")" > "../valgrind_${BASENAME}.txt"
          echo "Valgrind output for $(basename "$MODIFIED_FILE"):"
          cat "../valgrind_${BASENAME}.txt"
        else
          echo "No executable found for $(basename "$MODIFIED_FILE")"
        fi
      done
  rules:
    - if: '$CI_MERGE_REQUEST_DIFF_ID || $CI_COMMIT_BRANCH'
      changes:
        - "src/**/*.cpp"
      when: on_success
      allow_failure: false
  dependencies:
    - build_job
  artifacts:
    reports:
      valgrind: "valgrind_*.txt"
    expire_in: 1 day


# 测试项目作业，编译并运行测试
test_proj_func:
  stage: test_regression
  script:
    - set -e
    - cd tests && mkdir -p build && cd build  # 进入测试目录并创建构建目录
    - cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug  # 配置 CMake 以构建测试
    - make -j$(nproc)  # 并行构建测试
    - ctest --output-on-failure  # 运行测试
  dependencies:
    - build_job  # 此作业依赖于构建作业
  allow_failure: false
  artifacts:
    paths:
      - tests/build/  # 保存测试构建产物
    expire_in: 1 day
    
 
# Valgrind 作业，运行内存检测
test_proj_valgrind:
  stage: test_regression
  script:
    - set -e
    - cd cmake_build/  # 进入构建目录
    - valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./${PROJECT_NAME} > valgrind_output.txt  # 运行 Valgrind 内存检测
  dependencies:
    - build_job  # 此作业依赖于构建作业
  allow_failure: false
  artifacts:
    reports:
      valgrind: valgrind_output.txt  # 保存 Valgrind 报告
    expire_in: 1 day


# 生成代码覆盖率报告的作业
test_proj_coverage:
  stage: test_regression
  variables:
    BUILD_DIR: "cmake_build_coverage"  # 定义一个新的构建目录变量
  script:
    - set -e
    - mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR}  # 使用新的构建目录
    - cmake .. -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CXX_FLAGS="${CXXFLAGS} ${COVERAGE_FLAGS}"
    - make -j$(nproc)
    - ./$(basename ${PROJECT_NAME})
    - lcov --capture --directory . --output-file coverage.info
    - lcov --remove coverage.info '/usr/*' --output-file coverage.info
    - lcov --list coverage.info
    - genhtml coverage.info --output-directory coverage_report
  artifacts:
    paths:
      - ${BUILD_DIR}/coverage_report/
    expire_in: 1 day
  dependencies:
    - build_job
  allow_failure: false


# 部署项目作业，将构建产物部署到服务器
deploy_project:
  stage: deploy
  script:
    - echo "Starting deployment process..."
    # 敏感信息不应直接写在配置文件中，应使用 GitLab CI/CD 变量
    - ssh -i $DEPLOY_PRIVATE_KEY user@yourserver.com "cd /path/to/your/app && git pull && ./stop.sh"
    - rsync -avz -e ssh --delete cmake_build/ user@yourserver.com:/path/to/remote/destination
    - ssh -i $DEPLOY_PRIVATE_KEY user@yourserver.com "cd /path/to/your/app && ./start.sh"
    - echo "Deployment process finished."
  dependencies:
    - test_proj_func  # 此作业依赖于测试作业
  only:
    - master  # 仅在 master 分支上执行部署作业

cleanup_job:
  stage: cleanup
  when: always
  script:
    - echo "Starting cleanup process..."
    - set +e  # 在执行删除操作时不立即退出脚本
    # 定义需要清理的目录列表，排除不应删除的目录
    - BUILD_DIRS="${CI_PROJECT_DIR}/build ${CI_PROJECT_DIR}/cmake_build ${CI_PROJECT_DIR}/tests/build"
    # 使用 find 命令安全地删除目录
    - for dir in $BUILD_DIRS; do
        echo "Checking directory: $dir"
        if [ -d "$dir" ]; then
          echo "Cleaning up directory: $dir"
          find "$dir" -mindepth 1 -delete  # 从内向外删除，避免删除目录本身
          rmdir "$dir"  # 删除空目录
          echo "Directory cleaned up: $dir"
        else
          echo "Directory not found, skipping: $dir"
        fi
      done
    - set -e  # 恢复脚本在出现错误时退出的行为
  allow_failure: true  # 清理作业失败不影响流水线状态
  artifacts:
    when: always
    expire_in: 1 day
    paths:
      - cleanup.log  # 假设清理过程中的日志被写入 cleanup.log
  
  
rules:
  # 安装依赖的规则：当源代码或测试代码发生变更时执行
  - changes:
      - "src/**"
      - "include/**"
      - "tests/**"
    when: on_success
    allow_failure: false
    exists:
      - "src/**"
      - "include/**"
      - "tests/**"

  # 后续步骤的规则：依赖于 install 步骤的结果
  - if: '$CI_JOB_STAGE == "install"'
    when: on_success
    allow_failure: false

  # 测试、部署和清理阶段的规则：自动执行，依赖于前一个步骤的结果
  - if: '$CI_JOB_STAGE == "test" || $CI_JOB_STAGE == "deploy" || $CI_JOB_STAGE == "cleanup"'
    when: on_success
    allow_failure: false

  # 缓存规则：适用于所有作业
  - key: cache-rules
    when: always
    cache:
      key: ${CI_COMMIT_REF_SLUG}
      paths:
        - third_party/
        - .cache/
```



### 5. Makefile: 编译和构建：

Makefile是一个构建自动化工具，它定义了一系列的规则和依赖关系来编译和链接源代码。一个典型的Makefile可能包含：

- **目标**：通常是构建产物，如可执行文件或库文件。
- **依赖**：目标文件依赖的源文件或其他目标。
- **命令**：生成目标所需的shell命令。

简单的Makefile可能如下所示：

```makefile
# 定义项目名称
PROJECT_NAME := MyCppProject

# 设置C++编译器
CXX := g++

# 设置C++标准
CXX_STANDARD := -std=c++17

# 启用额外的警告信息
CXXFLAGS := $(CXX_STANDARD) -Wall -Wextra -pedantic

# 定义源文件变量
SOURCE_FILES := src/main.cpp src/operations/add_oper.cpp src/operations/minus_oper.cpp

# 定义头文件搜索路径
INCLUDE_DIRS := -I$(PROJECT_SOURCE_DIR)/include

# 定义目标可执行文件
TARGET := $(PROJECT_NAME)

# 链接第三方库，这里需要替换成实际的库名
LIBS := -lsome_lib_target_name

# 定义GoogleTest头文件路径
GTEST_INCLUDE := -I$(PROJECT_SOURCE_DIR)/test/googletest/include -I$(PROJECT_SOURCE_DIR)/test/googletest

# 定义测试源文件变量
TEST_SOURCES := test/*.cpp

# 默认目标
all: $(TARGET)

# 创建可执行文件
$(TARGET): $(SOURCE_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o $@ $^ $(LIBS)

# 包含测试目标
tests: $(addsuffix _test, $(notdir $(basename $(TEST_SOURCES))))

%_test: test/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(GTEST_INCLUDE) -o $@ $< -lgtest_main -lgmock_main $(LIBS)

# 清理构建产物
clean:
	rm -f $(TARGET) $(addsuffix _test, $(notdir $(basename $(TEST_SOURCES))))

# 伪目标：帮助信息
.PHONY: all clean tests

# 包含GoogleTest的Makefile，这需要GoogleTest已经作为子模块被检出
include test/googletest/CMakeLists.txt
```

#### 根目录下的 `CMakeLists.txt`

```cmake
# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和版本，同时指定使用的语言
project(MyCppProject VERSION 1.0 LANGUAGES CXX)

# 设置C++标准为17，并要求编译器必须支持此标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加编译选项以启用额外的警告信息
add_compile_options(-Wall -Wextra -pedantic)

# 为目标MyCppProject添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加子目录
add_subdirectory(lib/third_party)
add_subdirectory(cmake)

# 包含自定义CMake模块
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# 包含GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://gitee.com/mirrors/googletest.git
)
FetchContent_MakeAvailable(googletest)

# 启用测试
enable_testing()

# 定义源文件变量
set(SOURCE_FILES
    src/main.cpp
    src/operations/add_oper.cpp
    src/operations/minus_oper.cpp
    # 其他源文件路径
)

# 创建可执行文件
add_executable(MyCppProject ${SOURCE_FILES})

# 链接第三方库
target_link_libraries(MyCppProject PRIVATE some_lib_target_name) # 根据实际情况替换

# 包含头文件目录
target_include_directories(MyCppProject PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 包含GoogleTest头文件
target_include_directories(MyCppProject PRIVATE
    "${PROJECT_SOURCE_DIR}/test/googletest/include"
    "${PROJECT_SOURCE_DIR}/test/googletest"
)

# 为GoogleTest设置C++标准
set_target_properties(gtest gtest_main gmock gmock_main
    PROPERTIES CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# 添加测试源文件
file(GLOB TEST_SOURCES "test/*.cpp")

# 为每个测试文件创建测试可执行文件和测试案例
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME}
        PRIVATE
            MyCppProject
            gtest_main
            gmock_main
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# 启用Clang-Tidy
find_package(clang-tidy QUIET)
if(clang-tidy_FOUND)
    set_target_properties(MyCppProject PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE};-p=${CMAKE_BINARY_DIR};-header-filter=.*;-checks=*")
    message(STATUS "clang-tidy found and enabled for MyCppProject.")
else()
    message(WARNING "clang-tidy not found, Clang-Tidy checks will be skipped.")
endif()

# 启用Cppcheck
find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)
if(CPPCHECK_EXECUTABLE)
    add_custom_target(
        cppcheck_analysis 
        COMMAND ${CPPCHECK_EXECUTABLE} 
        --enable=all 
        --inconclusive 
        --verbose 
        --project=${CMAKE_BINARY_DIR}/compile_commands.json 
        ${PROJECT_SOURCE_DIR}/src
    )
    message(STATUS "cppcheck found: ${CPPCHECK_EXECUTABLE}, target for analysis created.")
else()
    message(WARNING "cppcheck not found, static analysis with cppcheck cannot be performed.")
endif()

# 启用Valgrind内存检查
find_program(VALGRIND_EXECUTABLE NAMES valgrind)
if(VALGRIND_EXECUTABLE)
    file(GLOB TEST_EXECUTABLES "${CMAKE_BINARY_DIR}/tests/test_*")
    foreach(TEST_EXECUTABLE ${TEST_EXECUTABLES})
        get_filename_component(TEST_NAME ${TEST_EXECUTABLE} NAME)
        add_custom_target(
            ${TEST_NAME}_valgrind 
            COMMAND ${VALGRIND_EXECUTABLE} 
            --tool=memcheck 
            --leak-check=full 
            ${TEST_EXECUTABLE}
        )
    endforeach()
    message(STATUS "Valgrind found: ${VALGRIND_EXECUTABLE}, memory checking targets created for each test.")
else()
    message(WARNING "Valgrind not found, memory checking cannot be performed.")
endif()

# 生成compile_commands.json文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 安装目标（如果需要）
# install(TARGETS MyCppProject RUNTIME DESTINATION bin)
```

### 6. analyze: 静态代码（转到yml）：

静态代码分析是在不运行代码的情况下检查代码质量的工具。它可以检测出潜在的错误、代码风格问题、复杂度过高的函数等。在`.gitlab-ci.yml`中，可以添加一个步骤来运行静态分析工具，例如cppcheck,clang-tidy：

- `cppcheck` 是一个轻量级的工具，专注于查找常见的编程错误，如内存泄漏、越界访问、使用未初始化的变量等。它不需要编译器生成的抽象语法树（AST），因此它可以快速运行并且易于使用。
- `clang-tidy` 是基于 `Clang` 编译器的静态分析工具，提供了更全面的代码检查功能，包括编码规范、性能、安全性、现代C++用法等。`clang-tidy` 可以利用编译器生成的AST进行深入分析。

请注意，为了运行 `clang-tidy`，您可能需要安装 `clang` 工具链，并且确保 `clang-tidy` 命令可用。同样，`cppcheck` 也需要安装在 CI/CD 运行环境中。您可以使用 Docker 镜像或在 `.gitlab-ci.yml` 中添加安装步骤来满足这些要求。

- `script`定义了要执行的命令。
- `artifacts`定义了如何保存分析报告。

### 7. test：单元测试/功能测试

```c++
#ifndef SAMPLE_HEAD_H
#define SAMPLE_HEAD_H

#include <vector>

int add(int a, int b);
int processVector(const std::vector<int>& vec);

#endif
```

```cpp
#include "sample_head.h"
#include <iostream>
#include <vector>

int add(int a, int b) {
    return a + b;
}

int processVector(const std::vector<int>& c) {
    int sum=0;
    for(auto & tmpc: c)
        sum+=tmpc;
    return sum;
}
```

```c++
#include "sample_head.h"
#include "gtest/gtest.h"

// 测试 add 函数
TEST(AddTest, HandlesPositiveNumbers) {
    EXPECT_EQ(5, add(2, 3));
}

TEST(AddTest, HandlesNegativeNumbers) {
    EXPECT_EQ(-5, add(-2, -3));
}

TEST(AddTest, ZeroTest) {
    EXPECT_EQ(0, add(0, 0));
}

// 测试 processVector 函数
TEST(ProcessVectorTest, HandlesEmptyVector) {
    std::vector<int> empty;
    EXPECT_EQ(0, processVector(empty));
}

TEST(ProcessVectorTest, HandlesNonEmptyVector) {
    std::vector<int> vec = {1, 2, 3, 4};
    EXPECT_EQ(10, processVector(vec));
}

// 可以添加更多的测试用例来检查不同的输入情况
TEST(ProcessVectorTest, HandlesNegativeNumbers) {
    std::vector<int> vec = {-1, -2, -3};
    EXPECT_EQ(-6, processVector(vec));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```



