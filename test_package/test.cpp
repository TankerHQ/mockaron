#include <mockaron/mockaron.hpp>

struct Test{};

struct MockTest : public mockaron::mock_impl
{
};

int main()
{
  mockaron::mock<Test, MockTest> mockTest;
}
