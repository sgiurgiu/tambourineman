#include <gtest/gtest.h>
#include <protofileloader.h>

TEST(ProtoFileLoader, loadFile)
{
    ProtoFileLoader loader;
    loader.loadFile("test/data/","test/data/addressbook.proto");

    //ASSERT_TRUE(loader);
    //ASSERT_TRUE(loader.HasMember("source"));
//    auto source = (*doc)["source"].GetString();
//    ASSERT_STREQ("file", source);
}

/*
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/
