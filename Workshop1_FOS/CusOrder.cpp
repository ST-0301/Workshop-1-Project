#include "pch.h"
#include "CusOrder.h"
#if __has_include("CusOrder.g.cpp")
#include "CusOrder.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::Workshop1FOS::implementation
{
    int32_t CusOrder::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void CusOrder::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void CusOrder::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
