![](C:\Users\qykj\Desktop\work_flow.png)

Qt有 QtTest framework

[microprofile](https://github.com/jonasmr/microprofile)

[doctest/doctest: The fastest feature-rich C++11/14/17/20/23 single-header testing framework (github.com)](https://github.com/doctest/doctest)

---

逻辑检查，Decision Coverage/Branch Coverage/Condition Coverage/Path Coverage

**一切没有单测覆盖的重构都是裸奔**，**TDD（Test-Driven Development）**

耦合度，逻辑检查

每次有个函数，就需要code review，UT CR是同一个级别，逻辑检查，最好在文档，写代码之前就弄清楚

---

[How to Conduct Code Reviews with GitLab's Merge Requests (howtogeek.com)](https://www.howtogeek.com/devops/how-to-conduct-code-reviews-with-gitlabs-merge-requests/)

[Code Review Guidelines | GitLab](https://docs.gitlab.com/ee/development/code_review.html)

---

## 针对CI/CD流程

设置docker kubernetes，GitLab runner

[Test with GitLab CI/CD and generate reports in merge requests | GitLab](https://docs.gitlab.com/ee/ci/testing/)

### Improve Code Quality performance with private runners [Code Quality | GitLab](https://docs.gitlab.com/ee/ci/testing/code_quality.html)

```
code_quality:
  services:
  variables:
    DOCKER_SOCKET_PATH: /run/user/997/docker.sock
  tags:
    - cq-rootless
```

`.codeclimate.yml`

### Unique rules to find Bugs, Vulnerabilities, Security Hotspots, and Code Smells in your C++ code

后面考虑使用python自动化

---

## 静态分析（除了插件，一般的静态分析不在makefile里定义）

`sonarLint->vscode`插件,[Running an analysis - VS Code (sonarsource.com)](https://docs.sonarsource.com/sonarlint/vs-code/getting-started/running-an-analysis/)

`cppcheck->vscode插件，但是要先安装cppcheck`   [Cppcheck Plug-in - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=NathanJ.cppcheck-plugin&ssr=false#overview)

```shell
cmake --build . --target runTestsAndSaveOutput   cd build .\runTests.exe > test_results.txt
cmake --build . --target cppcheck 
#不能一步到位，否则没法根据cppcheck结果输出对应的txt，不过先cppcheck更合适
```

clang-tools Docker images,   clang-format 和 clang-tidy

`cpp-linter`，**cpp-linter-hooks**也是集成到自动化流程，`pip install pre-commit`，`.pre-commit-config.yaml`，[cpp-linter-hooks · PyPI](https://pypi.org/project/cpp-linter-hooks/)

```yaml
lint:
  stage: lint
  image: your-custom-image:latest  # Replace with your custom image that has cpp-linter-hooks installed
  script:
    - cpp-linter-hooks install
    - cpp-linter-hooks run
```

## 单元测试 TDD

```
1. Level1：正常流程可用，即一个函数在输入正确的参数时，会有正确的输出
2. Level2：异常流程可抛出逻辑异常，即输入参数有误时，不能抛出系统异常，而是用自己定义的逻辑异常通知上层调用代码其错误之处
3. Level3：极端情况和边界数据可用，对输入参数的边界情况也要单独测试，确保输出是正确有效的
4. Level4：所有分支、循环的逻辑走通，不能有任何流程是测试不到的
5. Level5：输出数据的所有字段验证，对有复杂数据结构的输出，确保每个字段都是正确的
```

现在一个流行的趋势就是省去单元测试，多用面向需求的集成测试和端到端测试代替，只有贴合实际使用场景的测试才是有意义的，大量的单元测试很容易沦为虚假的happy path集合，实际意义其实很低，所以最好有详细的计划书

A **mock object** implements the same interface as a real object，gtest gmock，都隶属于google benchmark

构建编译gtest

```shell
mkdir build && cd build

cmake -G "MinGW Makefiles" ..

mingw32-make
```

```shell
#cmake -B ./build 是 mk build   cd build   cmake..   cd..  的简写，-S -B指定source build目录
```

然后复制有`libgmock.a libgmock_main.a libgtest.a libgtest_main.a`的lib到googletest，然后

```cmake
target_include_directories(runTests PRIVATE
  ${CMAKE_SOURCE_DIR}/${GTEST_ROOT}/googlemock/include
  ${CMAKE_SOURCE_DIR}/${GTEST_ROOT}/googletest/include
  ${CMAKE_SOURCE_DIR}/include #项目头文件目录
)

target_link_libraries(runTests
  ${CMAKE_SOURCE_DIR}/${GTEST_ROOT}/lib/libgtest_main.a
  ${CMAKE_SOURCE_DIR}/${GTEST_ROOT}/lib/libgtest.a
  )
```

可以`build/Testing/Temporary/*.log` 文件输出对应的txt，或者

```sh
PS D:\codebase\mycodetest\build> .\runTests.exe > gtest_result.txt
```

## 代码覆盖率和可追溯性

<img src="C:\Users\qykj\Desktop\gcov.jpeg" style="zoom:80%;" />

![](C:\Users\qykj\Desktop\sketch.png)

- **Function coverage:** It’s the number of functions that have been called.
- **Statement coverage:** The number of statements in a program you have executed.
- **Branch coverage:** The number of branches you execute from the control structure (if-else, while, do while, etc.)
- **Condition coverage:** The number of Boolean expressions you test in your code.
- **Line coverage:** The number of lines you test in your source code.

`.gcno & .gcda`都输出，用于HTML，lcov在win上运行不好用 `pip install gcovr`吧

| gcovr | C/C++ | `/^TOTAL.*\s+(\d+\%)$/` |
| :---: | :---: | :---------------------: |

（这一步可以集成到CI/CD流程）

## 运行时分析(性能测试)

性能测试`google-benchmark`

```shell
#google benchmark编译
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=install -DBENCHMARK_ENABLE_GTEST_TESTS=FALSE

cmake --build build --config Release -j4 --target install --verbose
```

https://github.com/DigitalInBlue/Celero这个也能进行基准测试

## 安全测试

availability confidentiality integrity reliability robustness performance

Fuzz Testing

  AFL（American Fuzzy Lop）针对Linux，

  Winafl弥补了这一空白，使用DynamoRIO来插桩&测量代码覆盖率，并使用Windows API进行内存和进程创建

SonarQube，Static Application  (SAST) Tools

---

流程：开发过程 -> 单元测试（白盒测试）-> 集成测试 -> 系统测试（包括黑盒测试、功能测试等）

CMake是配置工具，g++/gcc是具体的编译器，`CMakeLists.txt` 是定义如何使用CMake的脚本

source code->CMakeLists.txt->cmake->Makefile->make->executable

[Makefile基础 - Makefile教程 - 廖雪峰的官方网站 (liaoxuefeng.com)](https://liaoxuefeng.com/books/makefile/makefile-basic/)

my_project/
│
├── src/             # 源代码目录
│   ├── main.cpp
│   └── ...
│
├── include/         # 头文件目录
│   ├── my_header.h
│   └── ...
│
├── test/            # 测试代码目录
│   ├── test_main.cpp  # 测试主函数和Google Test初始化
│   ├── test_my_header.cpp  # 对my_header.h的测试
│   └── ...
│
├── .gitlab-ci.yml   # GitLab CI/CD配置文件
└── CMakeLists.txt   # CMake配置文件，或者直接写Makefile

---



```yaml
stages:
  - static_check
  - build
  - test
  - security_check
  - performance_test

variables:
  SONARQUBE_HOST_URL: "http://your-sonar-server.com" 
  SONARQUBE_PROJECT_KEY: "your-sonarqube-project-key"
  SONARQUBE_LOGIN: "your-sonarqube-token"

before_script:
  - apt-get update -y
  - apt-get install -y build-essential cmake clang-tidy cppcheck valgrind

static_check:
  stage: static_check
  script:
    - cmake .
    - make
    - clang-tidy src/*.cpp --warnings-as-errors=*

build:
  stage: build
  script:
    - cmake .
    - make

unit_test:
  stage: test
  script:
    - cmake .
    - make
    - ctest

security_check:
  stage: security_check
  script:
    - cppcheck --enable=all --inconclusive --quiet -i src/*.cpp

performance_test:
  stage: performance_test
  script:
    - valgrind --tool=callgrind ./my_executable

sonarqube_scan:
  stage: static_check
  script:
    - sonar-scanner -Dsonar.host.url=$SONARQUBE_HOST_URL -Dsonar.login=$SONARQUBE_LOGIN -Dsonar.projectKey=$SONARQUBE_PROJECT_KEY
  only:
    - master
```

确保你已经安装了 SonarQube Scanner。SonarQube Community 版本通常不包含 Scanner，你需要单独下载和安装。你可以从 SonarQube 的官方网站下载 Scanner。

此外，确保你的 SonarQube 服务器已经正确配置，并且你的项目已经在 SonarQube 中创建。你需要使用正确的 `SONARQUBE_PROJECT_KEY` 和 `SONARQUBE_LOGIN` 来确保 CI/CD 流程能够正确地与 SonarQube 交互。

---

### CMakeLists.txt示例

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加一个可执行文件
add_executable(my_executable src/main.cpp)

# 启用测试
enable_testing()

# 包含Google Test库
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 添加测试可执行文件
add_executable(tests test/test_main.cpp test/test_my_header.cpp)
target_link_libraries(tests gtest_main)

# 添加测试
add_test(NAME MyTests COMMAND tests)
```

### .gitlab-ci.yml示例

```yml
stages:
  - build
  - test

variables:
  CMAKE_BUILD_TYPE: "Release"

build_job:
  stage: build
  script:
    - cmake -S . -B build
    - cmake --build build

test_job:
  stage: test
  script:
    - cmake --build build --target tests
    - ./build/tests
  dependencies:
    - build_job
  allow_failure: false
```

### test_my_header.cpp示例

```c++

#include <gtest/gtest.h>
#include "my_header.h"

// 测试 add 函数
TEST(MyHeaderTest, AddTest) {
    EXPECT_EQ(add(1, 2), 3);
    EXPECT_EQ(add(-1, 1), 0);
    EXPECT_EQ(add(-1, -1), -2);
}

// 测试 minus 函数
TEST(MyHeaderTest, MinusTest) {
    EXPECT_EQ(minus(5, 3), 2);
    EXPECT_EQ(minus(-1, -1), 0);
    EXPECT_EQ(minus(1, 2), -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```



MakeFile与CI/CD

在CI/CD配置文件中添加静态代码分析工具的步骤与Makefile有以下关系：

1. **构建触发**：
   - 在Makefile中，您可以定义如何构建项目和运行测试。
   - 在CI/CD配置文件中，您可以设置在代码提交或拉取请求时自动触发Makefile中的构建和测试。

2. **扩展流程**：

   - Makefile通常不包含静态代码分析步骤，因为它主要关注构建过程。
   - CI/CD配置文件可以扩展构建流程，包括在构建前后运行额外的步骤，如静态代码分析。

3. **环境差异**：
   - Makefile在开发人员的本地机器上运行，可能无法访问CI/CD环境中的特定工具或服务。
   - CI/CD配置文件在服务器上执行，可以安装和配置所需的任何工具，包括静态代码分析工具。

4. **依赖管理**：
   - Makefile管理项目文件的依赖关系，确保正确的文件在正确的时间被重新构建。
   - CI/CD配置文件管理整个项目构建和部署过程中的依赖，包括依赖项的安装和配置。

   

### 1. CSD角度

持续软件开发是一种软件开发实践，强调在整个开发周期中持续地集成和测试代码。在C++项目中，自动化测试可以集成到CSD流程中，以确保代码质量。以下是一些关键点：

- **持续集成（CI）**: 自动化测试可以作为持续集成流程的一部分，每次代码提交后自动运行测试，快速发现集成问题。
- **测试覆盖率**: 使用工具（如gcov、lcov）来监控测试覆盖率，确保代码的关键部分得到充分测试。
- **自动化测试框架**: 集成如Google Test、Boost.Test等C++测试框架，编写和执行单元测试、集成测试。
- **代码质量分析**: 结合静态代码分析工具（如Cppcheck、Clang-Tidy）来自动检测代码质量问题。
- **测试驱动开发（TDD）**: 鼓励开发团队采用测试驱动的开发模式，先编写测试，再编写功能代码。
- **回归测试**: 自动化测试帮助维护一个健壮的回归测试套件，确保新代码不会破坏现有功能。

### 2.Makefile角度

Makefile是自动化构建系统中的核心组件，用于定义如何编译和链接程序。在C++项目中，Makefile可以配置自动化测试流程。以下是一些关键点：

- **构建规则**: 在Makefile中定义规则来编译测试代码和可执行文件。
- **依赖管理**: 明确指定源文件和头文件的依赖关系，确保在源文件更改时，相关的测试用例能够被重新编译。
- **测试执行**: 定义make目标来运行测试，可以是单个测试用例或整个测试套件。
- **并行测试**: 利用make的并行执行特性来同时运行多个测试，加快测试过程。
- **测试结果**: 收集和报告测试结果，可以在Makefile中集成工具来生成测试报告。
- **清理规则**: 提供清理构建和测试生成的文件的规则，保持工作目录的整洁。
- **自定义变量**: 使用Makefile中的变量来自定义测试配置，如选择不同的编译器标志或测试选项。
- Makefile 主要参与构建过程，意味着它定义了如何编译和链接程序，以及如何管理和执行构建过程中的各种任务。Makefile 通过指定依赖关系和规则来指导 `make` 工具按正确的顺序执行任务。

---

Makefile 主要参与构建过程，意味着它定义了如何编译和链接程序，以及如何管理和执行构建过程中的各种任务。Makefile 通过指定依赖关系和规则来指导 `make` 工具按正确的顺序执行任务。

### 构建过程的组成部分：

1. **编译源代码文件**：将 `.cpp` 文件编译成 `.o` 对象文件。
2. **链接对象文件**：将多个 `.o` 文件链接成一个可执行文件或库文件。
3. **生成文档**：从源代码中提取注释并生成文档。
4. **运行测试**：编译并执行单元测试或集成测试。
5. **清理构建产物**：删除所有编译生成的文件，以便重新构建。

### 举例说明 Makefile 的构建过程：

假设您有一个简单的 C++ 项目，包含以下文件：

- `src/main.cpp`：主程序源文件。
- `src/utils.cpp` 和 `src/utils.h`：辅助功能实现和声明。
- `include/app.h`：应用程序接口声明。

您的 Makefile 可能如下所示：

```makefile
# 定义编译器和链接器
CC=g++
CXXFLAGS=-Iinclude -Wall -Wextra -std=c++17
LDFLAGS=

# 定义目标应用程序名称
APP_NAME=app

# 收集所有源文件和对象文件
SRC=$(wildcard src/*.cpp)
OBJ=$(SRC:src/%.cpp=build/%.o)

# 默认目标
all: $(APP_NAME)

# 应用程序依赖于所有对象文件
$(APP_NAME): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# 每个对象文件依赖于其对应的源文件
build/%.o: src/%.cpp
	mkdir -p build
	$(CC) $(CXXFLAGS) -c $< -o $@

# 清理所有构建文件
clean:
	rm -rf build $(APP_NAME)

# 伪目标声明
.PHONY: all clean
```

在这个 Makefile 中：

- `all` 目标是默认目标，当您运行 `make` 时，它会构建应用程序。
- `$(APP_NAME)` 目标定义了如何从对象文件生成最终的可执行文件。
- `$(OBJ)` 是从 `src/` 目录下所有 `.cpp` 文件生成的对象文件列表。
- `build/%.o` 规则定义了如何编译单个源文件生成对应的对象文件。
- `clean` 目标用于清理构建产物，以便您可以从头开始构建。

当您在项目根目录下运行 `make` 命令时，Makefile 会根据定义的规则和依赖关系执行以下构建过程：

1. 如果 `build/` 目录不存在，`make` 会创建它。
2. `make` 会编译 `src/main.cpp` 和 `src/utils.cpp`，生成对应的 `.o` 文件。
3. `make` 会链接所有 `.o` 文件成为最终的 `app` 可执行文件。
4. 如果您运行 `make clean`，`make` 会删除 `build/` 目录和 `app` 可执行文件，让您可以进行一次干净的构建。

Makefile 通过这种方式管理构建过程，确保源代码正确编译和链接，同时提供清理和其他辅助功能。

---

### 测试工具与CI/CD的关系：

1. **测试覆盖率**：CI/CD流程中通常会监控测试覆盖率，确保代码更改不会降低项目的测试覆盖率。测试工具可以生成覆盖率报告，这些报告可以在CI/CD流程中被收集和分析。
2. **性能监控**：对于性能敏感的应用，可以使用Google Benchmark等工具在CI/CD流程中监控性能指标。如果性能下降超过某个阈值，CI/CD流程可以标记构建为失败，防止性能退化的代码被合并或部署。
3. **代码质量门禁**：CI/CD流程中可以设置代码质量门禁（Quality Gates），只有当代码满足一定的质量标准（如测试通过率、代码风格检查等）时，代码才能被合并或部署。测试工具的输出可以作为质量门禁的判断依据。
4. **持续改进**：CI/CD流程鼓励持续改进。通过集成测试工具，团队可以持续收集测试结果和性能数据，分析趋势，并根据这些信息进行代码和测试的优化。
5. **环境一致性**：CI/CD流程通常在一致的环境中运行测试，这有助于减少“它在我机器上可以运行”的问题。测试工具可以在这个标准化的环境中运行，确保测试结果的可靠性。

---

# CSD中，Makefile集成到CI流程

### 1. CSD的CI流程，触发Makefile编译和运行测试

**CI流程触发**:
- 当代码被推送到版本控制系统（如Git）时，CI服务器（如Jenkins, Travis CI, GitHub Actions等）自动触发构建流程。

**Makefile集成**:
- 在项目的根目录下，存在一个Makefile文件，它定义了项目的构建规则和目标。
- 一个典型的Makefile会包含编译源代码、生成可执行文件、运行测试等规则。

### 2. 用Makefile的规则调用CSD中的代码质量分析工具

**代码质量分析工具**:
- 工具如Cppcheck、Clang-Tidy、Coverity等，可以集成到Makefile中。

**Makefile集成代码分析**:
- 在Makefile中添加规则来运行代码质量分析工具，并生成报告。

**示例Makefile规则**:
```makefile
analyze:
	clang-tidy $(SRC_FILES) --checks=*,-google-*,-cppcoreguidelines-*
```

- `analyze`目标运行Clang-Tidy工具对源文件进行静态代码分析。

### 3. 通过Makefile生成的测试报告反馈到CSD的报告系统

**测试报告生成**:

- 测试框架（如Google Test）可以生成测试结果的详细报告。

**集成到Makefile**:
- 在Makefile中添加规则来收集测试结果，并可能使用工具（如Allure, ReportGenerator）生成更易读的报告。

**示例Makefile规则**:
```makefile
test_report:
	./test_runner
	generate_test_report.sh test_output.xml > test_report.html

send_report:
	curl -X POST -F "file=@test_report.html" https://your-csd-report-system.com/upload
```

- `test_report`目标运行测试并生成测试报告。
- `send_report`目标将测试报告上传到CSD的报告系统。

---

### 静态代码分析工具

1. **Clang Static Analyzer**：
   - **描述**：Clang Static Analyzer 是一个用于 C 和 C++ 代码的静态分析工具，可以发现潜在的错误和代码中的逻辑问题。
   - **特色**：检查条件分支的覆盖情况，发现未达到的代码路径。
   - **链接**：[Clang Static Analyzer](https://clang-analyzer.llvm.org/)

2. **Cppcheck**：
   - **描述**：Cppcheck 是一个专注于检测 C 和 C++ 代码的静态分析工具，不依赖预处理器或额外的构建步骤。
   - **特色**：容易集成到 CI 管道中，能够发现可能被忽视的代码路径。
   - **链接**：[Cppcheck](http://cppcheck.sourceforge.net/)

### 运行时代码分析工具

1. **Gcov (结合GCC使用)**：
   - **描述**：Gcov 是 GCC 工具链的一部分，用于代码覆盖率分析。通过Gcov，您可以找出哪些条件语句和分支运行得较多，哪些从未运行。
   - **特色**：与GCC紧密集成，能够生成详细的代码覆盖率报告。
   - **链接**：[Gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)

2. **LLVM's llvm-cov (结合Clang使用)**：
   - **描述**：llvm-cov 提供源代码覆盖率报告，适用于 Clang 和 LLVM 项目。
   - **特色**：能够与 Clang/LLVM 工具链整合，生成友好的HTML覆盖率报告。
   - **链接**：[llvm-cov](https://llvm.org/docs/CommandGuide/llvm-cov.html)

### 逻辑检查和测试工具

1. **Google Test (gtest)**：
   - **描述**：Google Test 是一个流行的 C++ 测试框架，广泛用于单元测试和集成测试。
   - **特色**：能够编写和运行单元测试，检查代码逻辑的正确性和功能的正确性。
   - **链接**：[Google Test](https://github.com/google/googletest)

2. **Catch2**：
   - **描述**：Catch2 是一个现代的 C++ 测试框架，使用方便，设计紧凑，并且提供丰富的测试报告。
   - **特色**：与现代C++兼容，容易集成到现有项目中。
   - **链接**：[Catch2](https://github.com/catchorg/Catch2)

### CI/CD 集成 (Gitlab)

- 可以在Gitlab CI/CD管道中集成上述工具，自动执行静态分析和测试。
- **示例**：
  ```yaml
  stages:
    - build
    - test
    - analyze
  
  build-job:
    stage: build
    script:
      - mkdir build
      - cd build
      - cmake ..
      - make
  
  test-job:
    stage: test
    script:
      - ./build/your_test_executable
  
  analyze-job:
    stage: analyze
    script:
      - cppcheck --enable=all --inconclusive --xml --xml-version=2 src 2> cppcheck.xml
      - gcov build/your_project_file
  ```

通过这些工具和平台，您可以实现代码的静态检查、条件语句覆盖情况分析、逻辑检查以及自动化测试。

---

### 1. 定义基础设施需求

首先，使用 Terraform 定义你的基础设施，包括VPS、数据库实例等。

**Terraform 配置（例如 `main.tf`）**：

```hcl
provider "aws" {
  region = "us-west-2"
}

resource "aws_instance" "example" {
  ami           = "ami-0c55b159cbfafe1f0"
  instance_type = "t2.micro"
}

resource "aws_db_instance" "default" {
  allocated_storage    = 20
  storage_type         = "gp2"
  engine               = "mysql"
  engine_version       = "5.7"
  instance_class       = "db.t2.micro"
  name                 = "mydb"
  username             = var.db_username
  password             = var.db_password
  parameter_group_name = "default.mysql5.7"
}

variable "db_username" {}
variable "db_password" {}
```

确保包含敏感信息的变量在 `terraform.tfvars`（非版本控制下）中定义：

```hcl
db_username = "yourusername"
db_password = "yourpassword"
```

### 2. 编写代码

在你的本地开发环境中编写 C++ 代码，并确保代码符合预期的 IaC 基础架构配置需求。

### 3. 提交到 GitLab

将代码和 Terraform 配置文件提交到 GitLab 仓库：

```bash
git add .
git commit -m "Initial commit with C++ code and Terraform configuration"
git push origin main
```

### 4. GitLab Runner 触发 CI/CD 流程

在 GitLab CI/CD 流程中使用 Terraform 来管理基础设施。

**`.gitlab-ci.yml` 配置**：

```yaml
stages:
  - setup_infrastructure
  - build
  - test
  - build_docker
  - deploy

variables:
  TF_STACK: "my-stack"

setup_infrastructure:
  stage: setup_infrastructure
  image: hashicorp/terraform:latest
  before_script:
    - terraform --version
  script:
    - terraform init
    - terraform plan -out=tfplan
    - terraform apply -input=false tfplan

build:
  stage: build
  script:
    - mkdir build
    - cd build
    - cmake ..
    - make

test:
  stage: test
  script:
    - cd build
    - ctest

build_docker:
  stage: build_docker
  image: docker:latest
  services:
    - docker:dind
  script:
    - docker build -t myapp:latest .
    - docker tag myapp:latest registry.gitlab.com/yourusername/yourrepo:latest
    - docker push registry.gitlab.com/yourusername/yourrepo:latest

deploy:
  stage: deploy
  image: bitnami/kubectl:latest
  script:
    - kubectl apply -f k8s/deployment.yaml
```

### 5. 编译

在 GitLab CI 的 `build` 阶段使用 `cmake` 和 `make` 编译你的 C++ 项目。

### 6. 单元测试、性能测试、代码质量分析

在 `test` 阶段运行单元测试，可以扩展到包含性能测试和代码质量分析。

**单元测试的 `.gitlab-ci.yml` 部分**：

```yaml
test:
  stage: test
  script:
    - cd build
    - ctest
```

### 7. 构建 Docker 镜像

在 `build_docker` 阶段构建并推送 Docker 镜像。

**示例 `Dockerfile`**：

```Dockerfile
FROM ubuntu:20.04
COPY . /app
WORKDIR /app
RUN apt-get update && apt-get install -y cmake make g++
RUN mkdir build && cd build && cmake .. && make
CMD ["./build/your_project_executable"]
```

### 8. 部署到 Kubernetes

使用 Kubernetes 的 YAML 配置文件来定义Pod、Service等，并在 `deploy` 阶段中应用这些配置。

**示例 Kubernetes 配置 `k8s/deployment.yaml`**：

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: myapp-deployment
spec:
  replicas: 2
  selector:
    matchLabels:
      app: myapp
  template:
    metadata:
      labels:
        app: myapp
    spec:
      containers:
      - name: myapp
        image: registry.gitlab.com/yourusername/yourrepo:latest
        ports:
        - containerPort: 8080
```

### 9. 配置管理

使用 Terraform 或其他 IaC 工具来管理和保持 Kubernetes YAML 文件的一致性。

### 10. 持续监控和调整

利用 IaC 工具来监控基础设施状态，并根据CI/CD流程中收集的反馈自动进行调整。

**例如使用 Prometheus 和 Grafana进行监控**：

在Terraform配置中包含这些监控工具的部署，也可以独立管理它们。

例如，如果你使用 Terraform 作为 IaC 工具，你的 CI/CD 流程中可能会加入如下步骤：

- 在 GitLab 的 `.gitlab-ci.yml` 文件中添加 Terraform 初始化和部署的步骤：

  ```yaml
  deploy:
    script:
      - terraform init
      - terraform plan
      - terraform apply -auto-approve
  ```

- 确保 `deploy` 步骤在代码提交后自动执行，同时也确保 Terraform 配置文件已经包含了创建和管理 Kubernetes 资源的定义。

为了实现高度自动化的CI/CD流程，减少人为干预，我们可以加入更多的细节，包括：

1. 自动化的基础设施配置和管理，使用 Terraform
2. 详细的代码质量和测试集成，包括 Google Test, Google Mock 和 SonarQube
3. 完整的Docker镜像构建和Kubernetes部署
4. 灾难恢复和监控

以下是具体的实现步骤和细节：

### 1. 基础设施即代码 (IaC) - 使用 Terraform

确保Terraform脚本可以自动化创建所有需要的基础设施。这包括配置VPS、数据库实例等。

**`main.tf`** 例子:

```hcl
provider "aws" {
  region = "us-west-2"
}

resource "aws_instance" "example" {
  ami           = "ami-0c55b159cbfafe1f0"
  instance_type = "t2.micro"
  tags = {
    Name = "MyInstance"
  }
}

resource "aws_db_instance" "default" {
  allocated_storage    = 20
  storage_type         = "gp2"
  engine               = "mysql"
  engine_version       = "5.7"
  instance_class       = "db.t2.micro"
  name                 = "mydb"
  username             = var.db_username
  password             = var.db_password
  parameter_group_name = "default.mysql5.7"
}

variable "db_username" {}
variable "db_password" {}

```

**变量文件 `terraform.tfvars`** (这个文件应该不被版本控制):

```hcl
db_username = "yourusername"
db_password = "yourpassword"
```

### 2. C++ 项目配置

#### CMakeLists.txt

确保项目能够使用CMake进行构建，并且集成Google Test和Google Mock。

```cmake
cmake_minimum_required(VERSION 3.10)

project(YourProject)

set(CMAKE_CXX_STANDARD 14)

# 添加源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(YourProjectLibrary ${SOURCES})

# GoogleTest 和 GoogleMock 的配置
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(YourProjectTests ${TEST_SOURCES})
target_link_libraries(YourProjectTests gtest gmock gtest_main)
add_test(NAME YourProjectTests COMMAND YourProjectTests)
```

#### Google Test 和 Google Mock 示例

**`tests/test_your_project_code.cpp`**:

```cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "your_project_code.h"

class MockYourClass : public YourClass {
public:
    MOCK_METHOD(int, add, (int a, int b), (override));
};

TEST(YourTestClass, YourTestFunctionWithMock) {
    MockYourClass mock;
    EXPECT_CALL(mock, add(1, 1)).WillOnce(::testing::Return(2));

    EXPECT_EQ(mock.add(1, 1), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

### 3. GitLab CI/CD 配置

以下是一个更完整的 `.gitlab-ci.yml` 配置，包含自动化代码质量分析、测试和部署。

```yaml
stages:
  - setup_infrastructure
  - build
  - test
  - code_quality
  - build_docker
  - deploy

variables:
  TF_STACK: "my-stack"
  SONAR_HOST_URL: "http://your-sonarqube-url"
  SONAR_TOKEN: "${SONAR_TOKEN}"  # 将 SonarQube 的 token 设置为 GitLab CI 的 Secret

cache:
  paths:
    - build-wrapper-output

setup_infrastructure:
  stage: setup_infrastructure
  image: hashicorp/terraform:latest
  before_script:
    - terraform --version
  script:
    - terraform init
    - terraform plan -out=tfplan
    - terraform apply -input=false tfplan

build:
  stage: build
  script:
    - mkdir -p build
    - cd build
    - cmake ..
    - make

test:
  stage: test
  script:
    - cd build
    - ./YourProjectTests

code_quality:
  stage: code_quality
  image: sonarsource/sonar-scanner-cli
  script:
    - sonar-scanner
      -Dsonar.projectKey=your_project_key
      -Dsonar.sources=src
      -Dsonar.tests=tests
      -Dsonar.language=cpp
      -Dsonar.sourceEncoding=UTF-8
      -Dsonar.host.url=$SONAR_HOST_URL
      -Dsonar.login=$SONAR_TOKEN
    artifacts:
      paths:
        - .scannerwork

build_docker:
  stage: build_docker
  image: docker:latest
  services:
    - docker:dind
  before_script:
    - docker login -u ${CI_REGISTRY_USER} -p ${CI_REGISTRY_PASSWORD} ${CI_REGISTRY}
  script:
    - docker build -t ${CI_REGISTRY_IMAGE}:latest .
    - docker push ${CI_REGISTRY_IMAGE}:latest

deploy:
  stage: deploy
  image: bitnami/kubectl:latest
  script:
    - kubectl apply -f k8s/deployment.yaml
```

#### Dockerfile

配置 Dockerfile 以便于在 GitLab CI 中进行构建和推送。

**`Dockerfile`**:

```Dockerfile
FROM ubuntu:20.04

# 安装必备的构建工具
RUN apt-get update && apt-get install -y \
    cmake \
    make \
    g++ \
    libgtest-dev \
    && apt-get clean

# 安装 GoogleTest
RUN cd /usr/src/gtest && cmake . && make && cp *.a /usr/lib

COPY . /app
WORKDIR /app

RUN mkdir -p build && cd build && cmake .. && make

CMD ["./build/your_project_executable"]
```

### 4. Kubernetes 配置

**示例 Kubernetes 配置 `k8s/deployment.yaml`**:

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: myapp-deployment
spec:
  replicas: 2
  selector:
    matchLabels:
      app: myapp
  template:
    metadata:
      labels:
        app: myapp
    spec:
      containers:
      - name: myapp
        image: registry.gitlab.com/yourusername/yourrepo:latest
        ports:
        - containerPort: 8080
---
apiVersion: v1
kind: Service
metadata:
  name: myapp-service
spec:
  selector:
    app: myapp
  ports:
    - protocol: TCP
      port: 80
      targetPort: 8080
  type: LoadBalancer
```

### 5. 灾难恢复和监控

你可以使用 Prometheus 和 Grafana 进行持续监控，也可以使用Terraform管理这些监控服务。

**Prometheus Terraform 配置**:

```hcl
resource "helm_release" "prometheus" {
  name       = "prometheus"
  repository = "https://prometheus-community.github.io/helm-charts"
  chart      = "prometheus"

  values = [
    <<EOF
server:
  resources:
    limits:
      cpu: 0.5
      memory: 512Mi
    requests:
      cpu: 0.1
      memory: 128Mi
EOF
  ]
}
```

**Grafana Terraform 配置**:

```hcl
resource "helm_release" "grafana" {
  name       = "grafana"
  repository = "https://grafana.github.io/helm-charts"
  chart      = "grafana"

  values = [
    <<EOF
adminUser: "admin"
adminPassword: "admin"
EOF
  ]
}
```
