//
//
// Copyright 2016 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//

#include <grpcpp/impl/grpc_library.h>
#include <grpcpp/test/server_context_test_spouse.h>

#include <cstring>
#include <vector>

#include "gtest/gtest.h"

namespace grpc {
namespace testing {

const char key1[] = "metadata-key1";
const char key2[] = "metadata-key2";
const char val1[] = "metadata-val1";
const char val2[] = "metadata-val2";

bool ClientMetadataContains(const ServerContext& context,
                            const grpc::string_ref& key,
                            const grpc::string_ref& value) {
  const auto& client_metadata = context.client_metadata();
  for (auto iter = client_metadata.begin(); iter != client_metadata.end();
       ++iter) {
    if (iter->first == key && iter->second == value) {
      return true;
    }
  }
  return false;
}

TEST(ServerContextTestSpouseTest, ClientMetadataHandle) {
  ServerContext context;
  ServerContextTestSpouse spouse(&context);

  spouse.AddClientMetadata(key1, val1);
  ASSERT_TRUE(ClientMetadataContains(context, key1, val1));

  spouse.AddClientMetadata(key2, val2);
  ASSERT_TRUE(ClientMetadataContains(context, key1, val1));
  ASSERT_TRUE(ClientMetadataContains(context, key2, val2));
}

TEST(ServerContextTestSpouseTest, InitialMetadata) {
  ServerContext context;
  ServerContextTestSpouse spouse(&context);
  std::multimap<std::string, std::string> metadata;

  context.AddInitialMetadata(key1, val1);
  metadata.insert(std::pair<std::string, std::string>(key1, val1));
  ASSERT_EQ(metadata, spouse.GetInitialMetadata());

  context.AddInitialMetadata(key2, val2);
  metadata.insert(std::pair<std::string, std::string>(key2, val2));
  ASSERT_EQ(metadata, spouse.GetInitialMetadata());
}

TEST(ServerContextTestSpouseTest, ServerMetadataHandle) {
  ServerContext context;
  ServerContextTestSpouse spouse(&context);
  std::multimap<std::string, std::string> metadata;

  context.AddTrailingMetadata(key1, val1);
  metadata.insert(std::pair<std::string, std::string>(key1, val1));
  ASSERT_EQ(metadata, spouse.GetTrailingMetadata());

  context.AddTrailingMetadata(key2, val2);
  metadata.insert(std::pair<std::string, std::string>(key2, val2));
  ASSERT_EQ(metadata, spouse.GetTrailingMetadata());
}

}  // namespace testing
}  // namespace grpc

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
