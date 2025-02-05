// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "transpiler/data/palisade_data.h"
#include "transpiler/tests/for_palisade.h"
#include "transpiler/tests/function_palisade.h"
#include "transpiler/tests/if_palisade.h"
#include "transpiler/tests/nested_for_palisade.h"
#include "transpiler/tests/palisade_test_util.h"
#include "transpiler/tests/switch_palisade.h"
#include "xls/common/status/matchers.h"

namespace fully_homomorphic_encryption {
namespace transpiler {
namespace {

using ::fully_homomorphic_encryption::transpiler::TranspilerTestBase;
using ::testing::TestParamInfo;
using ::testing::ValuesIn;
using ::testing::WithParamInterface;

template <typename InputType, typename OutputType>
struct TranspilerControlStructureTestCase {
  const std::string test_name;
  const InputType input;
  const OutputType expected_output;
};

using TranspilerControlStructureSwitchTestCase =
    TranspilerControlStructureTestCase<char, int>;
class TranspilerControlStructureSwitchTest
    : public TranspilerTestBase,
      public WithParamInterface<TranspilerControlStructureSwitchTestCase> {};

TEST_P(TranspilerControlStructureSwitchTest, TestSwitch) {
  const TranspilerControlStructureSwitchTestCase& test_case = GetParam();
  auto ciphertext = PalisadeChar::Encrypt(test_case.input, cc(), sk());
  PalisadeInt result(cc());
  XLS_ASSERT_OK(test_switch(result, ciphertext, cc()));
  EXPECT_EQ(result.Decrypt(sk()), test_case.expected_output);
}

INSTANTIATE_TEST_SUITE_P(
    TranspilerControlStructureTests, TranspilerControlStructureSwitchTest,
    ValuesIn<TranspilerControlStructureSwitchTestCase>({
        {"KnownCharacterA", 'a', 1},
        {"KnownCharacterB", 'b', 2},
        {"KnownCharacterC", 'c', 3},
        {"UnknownCharacterD", 'd', -1},
    }),
    [](const TestParamInfo<TranspilerControlStructureSwitchTest::ParamType>&
           info) { return info.param.test_name; });

struct IfTestCaseInput {
  const short lhs;
  const short rhs;
};

using TranspilerControlStructureIfTestCase =
    TranspilerControlStructureTestCase<IfTestCaseInput, int>;
class TranspilerControlStructureIfTest
    : public TranspilerTestBase,
      public WithParamInterface<TranspilerControlStructureIfTestCase> {};

TEST_P(TranspilerControlStructureIfTest, TestIf) {
  const TranspilerControlStructureIfTestCase& test_case = GetParam();
  auto lhs_ciphertext = PalisadeShort::Encrypt(test_case.input.lhs, cc(), sk());
  auto rhs_ciphertext = PalisadeShort::Encrypt(test_case.input.rhs, cc(), sk());
  PalisadeValue<char> result(cc());

  XLS_ASSERT_OK(test_if(result, lhs_ciphertext, rhs_ciphertext, cc()));
  EXPECT_EQ(result.Decrypt(sk()), test_case.expected_output);
}

INSTANTIATE_TEST_SUITE_P(
    TranspilerControlStructureTests, TranspilerControlStructureIfTest,
    ValuesIn<TranspilerControlStructureIfTestCase>({
        {"LessThan", {2, 5}, '<'},
        {"GreaterThan", {10, 8}, '>'},
        {"EqualTo", {4, 4}, '='},
    }),
    [](const TestParamInfo<TranspilerControlStructureIfTest::ParamType>& info) {
      return info.param.test_name;
    });

using TranspilerControlStructureForLoopTestCase =
    TranspilerControlStructureTestCase<short, short>;
class TranspilerControlStructureForLoopTest
    : public TranspilerTestBase,
      public WithParamInterface<TranspilerControlStructureForLoopTestCase> {};

TEST_P(TranspilerControlStructureForLoopTest, TestFor) {
  const TranspilerControlStructureForLoopTestCase& test_case = GetParam();
  auto ciphertext = PalisadeShort::Encrypt(test_case.input, cc(), sk());
  PalisadeShort result(cc());

  XLS_ASSERT_OK(test_for(result, ciphertext, cc()));
  EXPECT_EQ(result.Decrypt(sk()), test_case.expected_output);
}

TEST_P(TranspilerControlStructureForLoopTest, TestNestedFor) {
  const TranspilerControlStructureForLoopTestCase& test_case = GetParam();
  auto ciphertext = PalisadeShort::Encrypt(test_case.input, cc(), sk());
  PalisadeShort result(cc());

  XLS_ASSERT_OK(test_nested_for(result, ciphertext, cc()));
  EXPECT_EQ(result.Decrypt(sk()), test_case.expected_output);
}

INSTANTIATE_TEST_SUITE_P(
    TranspilerControlStructureTests, TranspilerControlStructureForLoopTest,
    ValuesIn<TranspilerControlStructureForLoopTestCase>({
        {"AddFourToPositive", 101, 105},
        {"AddFourToNegative", -101, -97},
        {"AddFourToZero", 0, 4},
    }),
    [](const TestParamInfo<TranspilerControlStructureForLoopTest::ParamType>&
           info) { return info.param.test_name; });

using TranspilerControlStructureFunctionTestCase =
    TranspilerControlStructureTestCase<int, int>;
class TranspilerControlStructureFunctionTest
    : public TranspilerTestBase,
      public WithParamInterface<TranspilerControlStructureFunctionTestCase> {};

TEST_P(TranspilerControlStructureFunctionTest, TestFunction) {
  const TranspilerControlStructureFunctionTestCase& test_case = GetParam();
  auto ciphertext = PalisadeInt::Encrypt(test_case.input, cc(), sk());
  PalisadeInt result(cc());

  XLS_ASSERT_OK(test_function(result, ciphertext, cc()));
  EXPECT_EQ(result.Decrypt(sk()), test_case.expected_output);
}

INSTANTIATE_TEST_SUITE_P(
    TranspilerControlStructureTests, TranspilerControlStructureFunctionTest,
    ValuesIn<TranspilerControlStructureFunctionTestCase>({
        {"ReturnOne", 1, 1},
        {"ReturnZero", 0, 0},
        {"ReturnNegativeOne", -1, -1},
    }),
    [](const TestParamInfo<TranspilerControlStructureFunctionTest::ParamType>&
           info) { return info.param.test_name; });

}  // namespace
}  // namespace transpiler
}  // namespace fully_homomorphic_encryption
